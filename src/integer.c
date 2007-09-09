#include "integer.h"
#include "object.h"
#include "state.h"

#include <string.h>
#include <stdlib.h>

//Integer Slots

struct Object *Integer_add(struct Object *o, struct Object *locals, struct Object *message)
{
	struct Object *ret = Integer_clone(o);
	struct Object *params_list;
	struct Object *param;
	struct Object *res;

	params_list = Object_getSlot(message, PARAMS);
	param = Object_getSlot(params_list, CHILD);
	res = State_doSeq(o, o, param);

	ret->data.val += res->data.val;

	return ret;
}

struct Object *Integer_sub(struct Object *o, struct Object *locals, struct Object *message)
{
	struct Object *ret = Integer_clone(o);
	struct Object *params_list;
	struct Object *param;
	struct Object *res;

	params_list = Object_getSlot(message, PARAMS);
	param = Object_getSlot(params_list, CHILD);
	res = State_doSeq(o, o, param);

	ret->data.val -= res->data.val;

	return ret;
}

struct Object *Integer_mul(struct Object *o, struct Object *locals, struct Object *message)
{
	struct Object *ret = Integer_clone(o);
	struct Object *params_list;
	struct Object *param;
	struct Object *res;

	params_list = Object_getSlot(message, PARAMS);
	param = Object_getSlot(params_list, CHILD);
	res = State_doSeq(o, o, param);

	ret->data.val *= res->data.val;

	return ret;
}

struct Object *Integer_div(struct Object *o, struct Object *locals, struct Object *message)
{
	struct Object *ret = Integer_clone(o);
	struct Object *params_list;
	struct Object *param;
	struct Object *res;

	params_list = Object_getSlot(message, PARAMS);
	param = Object_getSlot(params_list, CHILD);
	res = State_doSeq(o, o, param);

	ret->data.val /= res->data.val;

	return ret;
}

struct Object *Integer_print(struct Object *o, struct Object *locals, struct Object *message)
{
	printf("%d", o->data.val);
	return o;
}

// Interface

unsigned Integer_isInteger(struct Object *o)
{
	return o->clone_func == &Integer_clone;
}

void Integer_register(struct State *state)
{
	struct Object *o = Object_new();
	struct Object *obj = State_getObject(state);
	Object_init(o);
	Object_appendProto(o, obj);

	o->clone_func = &Integer_clone;
	o->free_func = &Integer_free;
	o->eval_func = &Integer_eval;
	o->isLiteral = 1;

	Object_registerFunction(state, o, "add", &Integer_add);
	Object_registerFunction(state, o, "sub", &Integer_sub);
	Object_registerFunction(state, o, "mul", &Integer_mul);
	Object_registerFunction(state, o, "div", &Integer_div);
	Object_registerFunction(state, o, "print", &Integer_print);

	State_registerProto(state, o, "Integer");
}

struct Object *Integer_clone(struct Object *o)
{
	struct Object *ret = Object_clone(o);
	ret->data.val = o->data.val;

	return ret;
}

void Integer_free(struct Object *o)
{
}

struct Object *Integer_eval(struct Object *o, struct Object *locals, struct Object *Integer)
{
	return o;
}
