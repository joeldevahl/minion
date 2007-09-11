#include "object.h"
#include "state.h"
#include "hash.h"
#include "function.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <memory.h>

// Object Slots

struct Object *Object_if(struct Object *o, struct Object *locals, struct Object *message)
{
	struct Object *params_list;
	struct Object *param;
	struct Object *res;

	params_list = Object_getSlot(message, PARAMS);
	param = Object_getSlot(params_list, CHILD);
	res = State_doSeq(locals, locals, param);

	params_list = Object_getSlot(params_list, NEXT);

	if(!res->data.ptr)
		params_list = Object_getSlot(params_list, NEXT);

	param = Object_getSlot(params_list, CHILD);
	res = State_doSeq(locals, locals, param);

	return res;
}

struct Object *Object_do(struct Object *o, struct Object *locals, struct Object *message)
{
	struct Object *params_list;
	struct Object *param;
	struct Object *res;

	params_list = Object_getSlot(message, PARAMS);
	param = Object_getSlot(params_list, CHILD);
	res = State_doSeq(o, o, param);

	return res;
}

// Interface

struct Object *Object_new()
{
	return calloc(1, sizeof(struct Object));
}

void Object_delete(struct Object *o)
{
	o->free_func(o);
	Hash_delete(o->hash);
	free(o);
}

void Object_init(struct Object *o)
{
	o->hash = calloc(1, sizeof(struct Hash));
	Hash_init(o->hash, 2);
	o->clone_func = &Object_clone;
	o->free_func = &Object_free;
	o->eval_func = &Object_eval;
}

void Object_register(struct State *state)
{
	struct Object *o = State_getObject(state);

	Object_registerFunction(state, o, "if", &Object_if);
	Object_registerFunction(state, o, "do", &Object_do);

	State_registerProto(state, o, "Object");
}

void Object_registerFunction(struct State *state, struct Object *o, const char *name, void *ptr)
{
	unsigned hash = Hash_genHashVal(name);
	struct Object *fun = State_cloneProto(state, "Function");
	Function_setTarget(fun, ptr);
	Object_setSlot(o, fun, hash);
}

struct Object *Object_clone(struct Object *o)
{
	struct Object *ret = Object_new();
	Object_init(ret);
	Hash_copy(ret->hash, o->hash);

	ret->clone_func = o->clone_func;
	ret->free_func = o->free_func;
	ret->eval_func = o->eval_func;
	ret->isLiteral = o->isLiteral;

	return ret;
}

void Object_free(struct Object *o)
{
}

struct Object *Object_eval(struct Object *o, struct Object *locals, struct Object *message)
{
	return o;
}

struct Object *Object_evalExpression(struct Object *o, struct Object *locals, struct Object *message)
{
	unsigned hash = Hash_genHashVal(message->data.ptr);
	struct Object *target = Object_getSlot(o, hash);

	if(target)
		return target->eval_func(o, locals, message);

	return 0x0;
}

void Object_createSlot(struct Object *o, unsigned name_hash)
{
	struct Object *val = Object_new();
	Object_init(val);
	Hash_set_h(o->hash, name_hash, val);
}

void Object_setSlot(struct Object *o, struct Object *val, unsigned name_hash)
{
	Hash_set_h(o->hash, name_hash, val);
}

struct Object *Object_getSlot(struct Object *o, unsigned name_hash)
{
	struct Object *ret = Hash_get_h(o->hash, name_hash);

	if(ret)
		return ret;
	else
	{
		unsigned proto_hash = Hash_genHashVal("proto");
		struct Object *proto = Hash_get_h(o->hash, proto_hash);
		if(proto)
			return Object_getSlot(proto, name_hash);
	}

	return 0x0;
}

void print_indent(unsigned i)
{
	while(i--)
		printf("\t");
}

void Object_deepPrint(struct Object *o, unsigned indent, unsigned first)
{
	struct Object *child, *params, *next;
	if(!o)
		return;

	if(first)
		print_indent(indent);

	if(String_isString(o) || Message_isMessage(o))
		printf("%s ", o->data.ptr);
	else if(Integer_isInteger(o))
		printf("%d ", o->data.val);
	else
		*((int*)(0x0)) = 0;

	params = Object_getSlot(o, PARAMS);
	if(params)
	{
		struct Object *p = params;
		printf("(\n");
		while(p)
		{
			child = Object_getSlot(p, CHILD);
			Object_deepPrint(child, indent + 1, 1);

			p = Object_getSlot(p, NEXT);
			if(p)
			{
				print_indent(indent + 1);
				printf(",\n");
			}
		}
		print_indent(indent);
		printf(") ");
	}

	child = Object_getSlot(o, CHILD); 
	if(child)
		Object_deepPrint(child, indent, 0);
	else
		printf("\n");

	next = Object_getSlot(o, NEXT);
	Object_deepPrint(next, indent, 1);
}

void Object_appendProto(struct Object *o, struct Object *proto)
{
	unsigned proto_hash = Hash_genHashVal("proto");
	Object_setSlot(o, proto, proto_hash);
}
