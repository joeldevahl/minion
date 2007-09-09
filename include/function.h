#ifndef MINION_FUNCTION_H
#define MINION_FUNCTION_H

struct State;
struct Object;

void Function_register(struct State *state);
struct Object *Function_clone(struct Object *o);
struct Object *Function_eval(struct Object *o, struct Object *locals, struct Object *message);
void Function_setTarget(struct Object *o, struct Object *(*function)(struct Object*, struct Object *, struct Object *));

#endif
