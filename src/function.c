#include "function.h"
#include "object.h"
#include "state.h"

void Function_register(struct State *state)
{
	struct Object *o = Object_new(state);
	Object_init(state, o);

	o->clone_func = &Function_clone;
	o->eval_func = &Function_eval;

	State_registerProto(state, o, "Function");
}

struct Object *Function_clone(struct State *state, struct Object *o)
{
	struct Object *ret = Object_clone(state, o);

	ret->data.ptr = o->data.ptr;

	return ret;
}

struct Object *Function_eval(struct State *state, struct Object *o, struct Object *locals, struct Object *message)
{
	unsigned hash = Hash_genHashVal(message->data.ptr);
	struct Object * target = Object_getSlot(state, o, hash);

	if(target && target->data.ptr)
	{
		EvalFunc function = (EvalFunc)target->data.ptr;
		o = function(state, o, locals, message);
	}

	return o;
}

void Function_setTarget(struct State *state, struct Object *o, EvalFunc function)
{
	o->data.ptr = (void*)function;
}
