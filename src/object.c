/* object.c
 * Copyright (C) 2007-2008 Joel de Vahl.
 * For conditions of distribution and use, see copyright notice in LICENSE
 */

#include "object.h"
#include "state.h"
#include "hash.h"
#include "function.h"
#include "message.h"
#include "integer.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <memory.h>

// Object Slots

struct Object *Object_bound_method(struct State *state, struct Object *o, struct Object *env, struct Object *message)
{
	struct Object *ret = State_cloneProto(state, "Message");
	ret->isLiteral = 0;
	Object_createSlot(state, ret, PARAMS);
	Object_setSlot(state, ret, Object_getSlot(state, message, PARAMS), PARAMS);

	return ret;
}

struct Object *Object_bound_createSlot(struct State *state, struct Object *o, struct Object *env, struct Object *message)
{
	struct Object *params_list;
	struct Object *name;
	struct Object *value;
	unsigned hash;

	params_list = Object_getSlot(state, message, PARAMS);
	name = Object_getSlot(state, params_list, CHILD);
	params_list = Object_getSlot(state, params_list, NEXT);
	value = Object_getSlot(state, params_list, CHILD);

	value = State_doSeq(state, o, env, value);

	hash = Hash_genHashVal(name->data.ptr);
	Object_createSlot(state, o, hash);
	Object_setSlot(state, o, value, hash);

	return value;
}

struct Object *Object_bound_setSlot(struct State *state, struct Object *o, struct Object *env, struct Object *message)
{
	struct Object *params_list;
	struct Object *name;
	struct Object *value;
	unsigned hash;

	params_list = Object_getSlot(state, message, PARAMS);
	name = Object_getSlot(state, params_list, CHILD);
	params_list = Object_getSlot(state, params_list, NEXT);
	value = Object_getSlot(state, params_list, CHILD);

	value = State_doSeq(state, env, env, value);

	hash = Hash_genHashVal(name->data.ptr);
	Object_setSlot(state, o, value, hash);

	return value;
}

struct Object *Object_bound_clone(struct State *state, struct Object *o, struct Object *env, struct Object *message)
{
	return Object_clone(state, o);
}

struct Object *Object_bound_if(struct State *state, struct Object *o, struct Object *env, struct Object *message)
{
	struct Object *params_list;
	struct Object *param;
	struct Object *res;

	params_list = Object_getSlot(state, message, PARAMS);
	param = Object_getSlot(state, params_list, CHILD);
	res = State_doSeq(state, env, env, param);

	params_list = Object_getSlot(state, params_list, NEXT);

	if(!res->data.ptr)
		params_list = Object_getSlot(state, params_list, NEXT);

	param = Object_getSlot(state, params_list, CHILD);
	res = State_doSeq(state, env, env, param);

	return res;
}

struct Object *Object_bound_do(struct State *state, struct Object *o, struct Object *env, struct Object *message)
{
	struct Object *params_list;
	struct Object *param;
	struct Object *res;

	params_list = Object_getSlot(state, message, PARAMS);
	param = Object_getSlot(state, params_list, CHILD);
	res = State_doSeq(state, o, o, param);

	return res;
}

// Interface

struct Object *Object_new(struct State *state)
{
	return calloc(1, sizeof(struct Object));
}

void Object_delete(struct State *state, struct Object *o)
{
	o->free_func(state, o);
	Hash_delete(o->hash);
	free(o);
}

void Object_init(struct State *state, struct Object *o)
{
	o->hash = calloc(1, sizeof(struct Hash));
	Hash_init(o->hash, 2);
	o->clone_func = &Object_clone;
	o->free_func = &Object_free;
	o->eval_func = &Object_eval;
	o->isLiteral = 1;
}

void Object_register(struct State *state)
{
	struct Object *o = State_getObject(state);

	Object_registerFunction(state, o, "if", &Object_bound_if);
	Object_registerFunction(state, o, "do", &Object_bound_do);
	Object_registerFunction(state, o, "createSlot", &Object_bound_createSlot);
	Object_registerFunction(state, o, "setSlot", &Object_bound_setSlot);
	Object_registerFunction(state, o, "clone", &Object_bound_clone);
	Object_registerFunction(state, o, "method", &Object_bound_method);

	State_registerProto(state, o, "Object");
}

void Object_registerFunction(struct State *state, struct Object *o, const char *name, void *ptr)
{
	unsigned hash = Hash_genHashVal(name);
	struct Object *fun = State_cloneProto(state, "Function");
	Function_setTarget(state, fun, ptr);
	Object_setSlot(state, o, fun, hash);
}

struct Object *Object_clone(struct State *state, struct Object *o)
{
	struct Object *ret = Object_new(state);
	Object_init(state, ret);
	Hash_copy(ret->hash, o->hash);

	ret->clone_func = o->clone_func;
	ret->free_func = o->free_func;
	ret->eval_func = o->eval_func;
	ret->isLiteral = o->isLiteral;

	return ret;
}

void Object_free(struct State *state, struct Object *o)
{
}

struct Object *Object_eval(struct State *state, struct Object *o, struct Object *env, struct Object *message)
{
	return o;
}

struct Object *Object_evalExpression(struct State *state, struct Object *o, struct Object *env, struct Object *message)
{
	unsigned hash = Hash_genHashVal(message->data.ptr);
	struct Object *target = Object_getSlot(state, o, hash);

	if(!target)
		target = Object_getSlot(state, env, hash);

	if(target)
	{
		if(target->isLiteral)
			return target;
		else
			return target->eval_func(state, o, env, message);
	}

	return 0x0;
}

void Object_createSlot(struct State *state, struct Object *o, unsigned name_hash)
{
	struct Object *val = Object_new(state);
	Object_init(state, val);
	Hash_set_h(o->hash, name_hash, val);
}

void Object_setSlot(struct State *state, struct Object *o, struct Object *val, unsigned name_hash)
{
	Hash_set_h(o->hash, name_hash, val);
}

struct Object *Object_getSlot(struct State *state, struct Object *o, unsigned name_hash)
{
	struct Object *ret = Hash_get_h(o->hash, name_hash);

	if(ret)
		return ret;
	else
	{
		unsigned proto_hash = Hash_genHashVal("proto");
		struct Object *proto = Hash_get_h(o->hash, proto_hash);
		if(proto)
			return Object_getSlot(state, proto, name_hash);
	}

	return 0x0;
}
/*
struct Object *Object_lookupSlot(struct State *state, struct Object *o, struct Object *env, unsigned name_hash)
{
	struct Object *res = Object_getSlot(state, o, name_hash);
	if(!res)
		res = Object_getSlot(state, env, name_hash);
	return res;
}
*/
void print_indent(unsigned i)
{
	while(i--)
		printf("\t");
}

void Object_deepPrint(struct State *state, struct Object *o, unsigned indent, unsigned first)
{
	struct Object *child, *params, *next;
	if(!o)
		return;

	if(first)
		print_indent(indent);

	if(String_isString(state, o) || Message_isMessage(state, o))
		printf("%s ", o->data.ptr);
	else if(Integer_isInteger(state, o))
		printf("%d ", o->data.ival);
	else
		printf("Object_<#%x> ", o);

	params = Object_getSlot(state, o, PARAMS);
	if(params)
	{
		struct Object *p = params;
		printf("(\n");
		while(p)
		{
			child = Object_getSlot(state, p, CHILD);
			Object_deepPrint(state, child, indent + 1, 1);

			p = Object_getSlot(state, p, NEXT);
			if(p)
			{
				print_indent(indent + 1);
				printf(",\n");
			}
		}
		print_indent(indent);
		printf(") ");
	}

	child = Object_getSlot(state, o, CHILD); 
	if(child)
		Object_deepPrint(state, child, indent, 0);
	else
		printf("\n");

	next = Object_getSlot(state, o, NEXT);
	Object_deepPrint(state, next, indent, 1);
}

void Object_appendProto(struct State *state, struct Object *o, struct Object *proto)
{
	unsigned proto_hash = Hash_genHashVal("proto");
	Object_setSlot(state, o, proto, proto_hash);
}
