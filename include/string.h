#ifndef MINION_STRING_H
#define MINION_STRING_H

struct State;
struct Object;

struct Object *String_length(struct State *state, struct Object *o, struct Object *locals, struct Object *message);
struct Object *String_unescape(struct State *state, struct Object *o, struct Object *locals, struct Object *message);
unsigned String_isString(struct State *state, struct Object *o);
void String_register(struct State *state);
struct Object *String_clone(struct State *state, struct Object *o);
void String_free(struct State *state, struct Object *o);
struct Object *String_eval(struct State *state, struct Object *o, struct Object *basenv, struct Object *env, struct Object *message);

#endif
