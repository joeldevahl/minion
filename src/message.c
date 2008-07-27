/* message.c
 * Copyright (C) 2007-2008 Joel de Vahl.
 * For conditions of distribution and use, see copyright notice in LICENSE
 */

#include "message.h"
#include "object.h"
#include "state.h"
#include "hash.h"

#include <string.h>
#include <stdlib.h>

unsigned Message_isMessage(struct State *state, struct Object *o)
{
	return o->clone_func == &Message_clone;
}


void Message_register(struct State *state)
{
	struct Object *o = State_newObject(state);
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

struct Object *Message_eval(struct State *state, struct Object *o, struct Object *env, struct Object *message)
{
	unsigned hash = Hash_genHashVal(message->data.ptr);
	struct Object *target = Object_getSlot(state, o, hash);

	//printf("\033[0;40;32mMessage_eval:\033[0m\n");
	//printf("message %s\t", message->data.ptr);
	//printf(" o = %x", o);
	//printf(" e = %x\n", env);

	if(target)
	{
		struct Object *local_env = State_newObject(state);
		struct Object *src_name_plist = Object_getSlot(state, target, PARAMS);
		struct Object *src_val_plist = Object_getSlot(state, message, PARAMS);

		Object_init(state, local_env);
		Object_appendProto(state, local_env, o);

		while(src_val_plist)
		{
			struct Object *src_name = Object_getSlot(state, src_name_plist, CHILD);
			struct Object *src_val = Object_getSlot(state, src_val_plist, CHILD);

			src_val = State_doSeq(state, env, env, src_val);

			//printf("name %s val %d\n", src_name->data.ptr, src_val->data.ival);

			hash = Hash_genHashVal(src_name->data.ptr);
			Object_createSlot(state, local_env, hash);
			Object_setSlot(state, local_env, src_val, hash);

			src_name_plist = Object_getSlot(state, src_name_plist, NEXT);
			src_val_plist = Object_getSlot(state, src_val_plist, NEXT);
		}

		return State_doSeq(state, o, local_env, Object_getSlot(state, src_name_plist, CHILD));
	}

	return 0x0;
}
