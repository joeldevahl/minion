#ifndef MINION_FUNCTION_H
#define MINION_FUNCTION_H

#include "object.h"

struct State;

void Function_register(struct State *state);
struct Object *Function_clone(struct State *state, struct Object *o);
struct Object *Function_eval(struct State *state, struct Object *o, struct Object *locals, struct Object *message);
void Function_setTarget(struct State *state, struct Object *o, EvalFunc function);

#endif
