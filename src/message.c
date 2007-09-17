#include "message.h"
#include "object.h"
#include "state.h"

#include <string.h>
#include <stdlib.h>

unsigned Message_isMessage(struct State *state, struct Object *o)
{
	return o->clone_func == &Message_clone;
}


void Message_register(struct State *state)
{
	struct Object *o = Object_new(state);
	Object_init(state, o);

	o->clone_func = &Message_clone;
	o->free_func = &Message_free;
	o->eval_func = &Message_eval;
	o->isLiteral = 0;

	State_registerProto(state, o, "Message");
}

struct Object *Message_clone(struct State *state, struct Object *o)
{
	struct Object *ret = Object_clone(state, o);

	if(o->data.ptr){
		unsigned length = (unsigned)strlen(o->data.ptr);
		ret->data.ptr = (void *)calloc(length + 1, sizeof(char));
		strcpy(ret->data.ptr, o->data.ptr);
	}

	return ret;
}

void Message_free(struct State *state, struct Object *o)
{
	free(o->data.ptr);
}

struct Object *Message_eval(struct State *state, struct Object *o, struct Object *locals, struct Object *message)
{
	return o;
}
