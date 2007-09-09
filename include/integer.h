#ifndef MINION_INTEGER_H
#define MINION_INTEGER_H

struct State;
struct Object;

unsigned Integer_isInteger(struct Object *o);
void Integer_register(struct State *state);
struct Object *Integer_clone(struct Object *o);
void Integer_free(struct Object *o);
struct Object *Integer_eval(struct Object *o, struct Object *locals, struct Object *message);

#endif
