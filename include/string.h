#ifndef MINION_STRING_H
#define MINION_STRING_H

struct State;
struct Object;

void String_unescape(struct Object *o);
unsigned String_isString(struct Object *o);
void String_register(struct State *state);
struct Object *String_clone(struct Object *o);
void String_free(struct Object *o);
struct Object *String_eval(struct Object *o, struct Object *locals, struct Object *message);

#endif
