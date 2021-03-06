/* bool.c
 * Copyright (C) 2007-2008 Joel de Vahl.
 * For conditions of distribution and use, see copyright notice in LICENSE
 */

#include "bool.h"
#include "object.h"
#include "state.h"

#include <string.h>
#include <stdlib.h>

void Bool_register(struct State *state)
{
	struct Object *o = State_newObject(state);
	Object_init(state, o);

	o->clone_func = &Bool_clone;
	o->free_func = &Bool_free;
	o->eval_func = &Bool_eval;
	o->isLiteral = 1;

	State_registerProto(state, o, "Bool");
}

struct Object *Bool_clone(struct State *state, struct Object *o)
{
	struct Object *ret = Object_clone(state, o);
	ret->data.ival = o->data.ival;

	return ret;
}

void Bool_free(struct State *state, struct Object *o)
{
}

struct Object *Bool_eval(struct State *state, struct Object *o, struct Object *env, struct Object *message)
{
	return o;
}
