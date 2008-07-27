/* list.c
 * Copyright (C) 2007-2008 Joel de Vahl.
 * For conditions of distribution and use, see copyright notice in LICENSE
 */

#include "list.h"
#include "object.h"
#include "state.h"

#include <string.h>
#include <stdlib.h>

void List_register(struct State *state)
{
	struct Object *o = State_newObject(state);
	Object_init(state, o);

	o->clone_func = &List_clone;
	o->free_func = &List_free;
	o->eval_func = &List_eval;

	State_registerProto(state, o, "List");
}

struct Object *List_clone(struct State *state, struct Object *o)
{
	struct Object *ret = Object_clone(state, o);

	return ret;
}

void List_free(struct State *state, struct Object *o)
{
}

struct Object *List_eval(struct State *state, struct Object *o, struct Object *env, struct Object *message)
{
	return o;
}
