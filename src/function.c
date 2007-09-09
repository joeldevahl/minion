#include "function.h"
#include "object.h"
#include "state.h"

void Function_register(struct State *state)
{
	struct Object *o = Object_new();
	Object_init(o);

	o->clone_func = &Function_clone;
	o->eval_func = &Function_eval;

	State_registerProto(state, o, "Function");
}

struct Object *Function_clone(struct Object *o)
{
	struct Object *ret = Object_clone(o);

	ret->data.ptr = o->data.ptr;

	return ret;
}

struct Object *Function_eval(struct Object *o, struct Object *locals, struct Object *message)
{
	unsigned hash = Hash_genHashVal(message->data.ptr);
	struct Object * target = Object_getSlot(o, hash);

	if(target && target->data.ptr)
	{
		struct Object *(*function)(struct Object *, struct Object *, struct Object *) = (struct Object *(*)(struct Object *, struct Object *, struct Object *))target->data.ptr;
		o = function(o, locals, message);
	}

	return o;
}

void Function_setTarget(struct Object *o, struct Object *(*function)(struct Object *, struct Object *, struct Object *))
{
	o->data.ptr = (void*)function;
}
