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
	unsigned hash = Hash_genHashVal(message->data.ptr);
	struct Object *target = Object_getSlot(state, o, hash);

	if(target)
	{
		struct Object *env = Object_new(state);
		struct Object *src_name_plist = Object_getSlot(state, target, PARAMS);
		struct Object *src_val_plist = Object_getSlot(state, message, PARAMS);

		Object_init(state, env);

		while(src_val_plist)
		{
			struct Object *src_name = Object_getSlot(state, src_name_plist, CHILD);
			struct Object *src_val = Object_getSlot(state, src_val_plist, CHILD);

			hash = Hash_genHashVal(src_name->data.ptr);
			Object_createSlot(state, env, hash);
			Object_setSlot(state, env, src_val, hash);

			src_name_plist = Object_getSlot(state, src_name_plist, NEXT);
			src_val_plist = Object_getSlot(state, src_val_plist, NEXT);
		}

		return State_doSeq(state, o, env, Object_getSlot(state, src_name_plist, CHILD));
	}

	return 0x0;
}
