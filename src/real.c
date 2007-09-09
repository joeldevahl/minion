#include "real.h"
#include "object.h"
#include "state.h"

#include <string.h>
#include <stdlib.h>

void Real_register(struct State *state)
{
	struct Object *o = Object_new();
	Object_init(o);

	o->clone_func = &Real_clone;
	o->free_func = &Real_free;
	o->eval_func = &Real_eval;

	State_registerProto(state, o, "Real");
}

struct Object *Real_clone(struct Object *o)
{
	struct Object *ret = Object_clone(o);
	ret->data.val = o->data.val;

	return ret;
}

void Real_free(struct Object *o)
{
}

struct Object *Real_eval(struct Object *o, struct Object *locals, struct Object *Real)
{
	return o;
}
