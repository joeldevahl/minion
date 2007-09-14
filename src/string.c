#include "string.h"
#include "object.h"
#include "state.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// String Slots

struct Object *String_print(struct State *state, struct Object *o, struct Object *locals, struct Object *message)
{
	printf("%s", o->data.ptr);
	return o;
}

// Interface

struct Object *String_length(struct State *state, struct Object *o, struct Object *locals, struct Object *message)
{
	struct Object *len = State_cloneProto(state, "Integer");

	if (!o || !o->data.str)
		len->data.val = 0;
	else
		len->data.val = strlen(o->data.str);

	return len;
}

struct Object *String_unescape(struct State *state, struct Object *o, struct Object *locals, struct Object *message)
{
	char *src;
	char *dst;

	if(!o || !o->data.ptr)
		return;

	src = dst = o->data.ptr;

	while(*src)
	{
		if(*src == '\\')
		{
			src++;
			switch(*src)
			{
				case 'n':
					*dst = '\n';
					break;
				default:
					*dst = *src;
					break;
			}
		}
		else
			*dst = *src;

		dst++;
		src++;
	}

	*dst = '\0';
}

unsigned String_isString(struct State *state, struct Object *o)
{
	return o->clone_func == (CloneFunc)&String_clone;
}

void String_register(struct State *state)
{
	struct Object *o = Object_new(state);
	struct Object *obj = State_getObject(state);
	Object_init(state, o);
	Object_appendProto(state, o, obj);

	o->clone_func = &String_clone;
	o->free_func = &String_free;
	o->eval_func = &String_eval;
	o->isLiteral = 1;

	Object_registerFunction(state, o, "print", &String_print);
	Object_registerFunction(state, o, "length", &String_length);

	State_registerProto(state, o, "String");
}

struct Object *String_clone(struct State *state, struct Object *o)
{
	struct Object *ret = Object_clone(state, o);

	if(o->data.ptr){
		unsigned length = (unsigned)strlen((char*)o->data.ptr);
		ret->data.ptr = (void *)calloc(length + 1, sizeof(char));
		strcpy((char*)ret->data.ptr, (char*)o->data.ptr);
	}

	return ret;
}

void String_free(struct State *state, struct Object *o)
{
	free(o->data.ptr);
}

struct Object *String_eval(struct State *state, struct Object *o, struct Object *locals, struct Object *message)
{
	return o;
}
