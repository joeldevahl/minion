/* function.c
 * Copyright (C) 2007-2008 Joel de Vahl.
 * For conditions of distribution and use, see copyright notice in LICENSE
 */

#include "function.h"
#include "object.h"
#include "state.h"
#include "hash.h"

void Function_register(struct State *state)
{
	struct Object *o = Object_new(state);
	Object_init(state, o);

	o->clone_func = &Function_clone;
	o->eval_func = &Function_eval;
	o->isLiteral = 0;

	State_registerProto(state, o, "Function");
}

struct Object *Function_clone(struct State *state, struct Object *o)
{
	struct Object *ret = Object_clone(state, o);

	ret->data.ptr = o->data.ptr;

	return ret;
}

struct Object *Function_eval(struct State *state, struct Object *o, struct Object *env, struct Object *message)
{
	unsigned hash = Hash_genHashVal(message->data.ptr);
	struct Object * target = Object_getSlot(state, o, hash);

	if(target && target->data.ptr)
	{
		EvalFunc function = (EvalFunc)target->data.ptr;
		o = function(state, o, env, message);
	}

	return o;
}

void Function_setTarget(struct State *state, struct Object *o, EvalFunc function)
{
	o->data.ptr = (void*)function;
}
