#include "integer.h"
#include "object.h"
#include "state.h"
#include "hash.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

//Integer Slots

struct Object *Integer_bound_add(struct State *state, struct Object *o, struct Object *env, struct Object *message)
{
	struct Object *ret = State_cloneProto(state, "Integer");
	struct Object *params_list;
	struct Object *param;
	struct Object *res;

	params_list = Object_getSlot(state, message, PARAMS);
	param = Object_getSlot(state, params_list, CHILD);
	res = State_doSeq(state, o, env, param);

	ret->data.ival = o->data.ival + res->data.ival;

	return ret;
}

struct Object *Integer_bound_sub(struct State *state, struct Object *o, struct Object *env, struct Object *message)
{
	struct Object *ret = State_cloneProto(state, "Integer");
	struct Object *params_list;
	struct Object *param;
	struct Object *res;

	params_list = Object_getSlot(state, message, PARAMS);
	param = Object_getSlot(state, params_list, CHILD);
	res = State_doSeq(state, o, env, param);

	ret->data.ival = o->data.ival - res->data.ival;

	return ret;
}

struct Object *Integer_bound_mul(struct State *state, struct Object *o, struct Object *env, struct Object *message)
{
	struct Object *ret = State_cloneProto(state, "Integer");
	struct Object *params_list;
	struct Object *param;
	struct Object *res;

	params_list = Object_getSlot(state, message, PARAMS);
	param = Object_getSlot(state, params_list, CHILD);
	res = State_doSeq(state, o, env, param);

	ret->data.ival = o->data.ival * res->data.ival;

	return ret;
}

struct Object *Integer_bound_div(struct State *state, struct Object *o, struct Object *env, struct Object *message)
{
	struct Object *ret = State_cloneProto(state, "Integer");
	struct Object *params_list;
	struct Object *param;
	struct Object *res;

	params_list = Object_getSlot(state, message, PARAMS);
	param = Object_getSlot(state, params_list, CHILD);
	res = State_doSeq(state, o, env, param);

	ret->data.ival = o->data.ival / res->data.ival;

	return ret;
}

struct Object *Integer_bound_print(struct State *state, struct Object *o, struct Object *env, struct Object *message)
{
	printf("%d", o->data.ival);
	return o;
}

// Interface

unsigned Integer_isInteger(struct State *state, struct Object *o)
{
	return o->clone_func == &Integer_clone;
}

void Integer_register(struct State *state)
{
	struct Object *o = Object_new(state);
	struct Object *obj = State_getObject(state);
	Object_init(state, o);
	Object_appendProto(state, o, obj);

	o->clone_func = &Integer_clone;
	o->free_func = &Integer_free;
	o->eval_func = &Integer_eval;
	o->isLiteral = 1;

	Object_registerFunction(state, o, "add", &Integer_bound_add);
	Object_registerFunction(state, o, "sub", &Integer_bound_sub);
	Object_registerFunction(state, o, "mul", &Integer_bound_mul);
	Object_registerFunction(state, o, "div", &Integer_bound_div);
	Object_registerFunction(state, o, "print", &Integer_bound_print);

	State_registerProto(state, o, "Integer");
}

struct Object *Integer_clone(struct State *state, struct Object *o)
{
	struct Object *ret = Object_clone(state, o);
	ret->data.ival = o->data.ival;

	return ret;
}

void Integer_free(struct State *state, struct Object *o)
{
}

struct Object *Integer_eval(struct State *state, struct Object *o, struct Object *env, struct Object *message)
{
	return o;
}
