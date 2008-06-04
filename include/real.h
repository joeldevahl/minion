#ifndef MINION_REAL_H
#define MINION_REAL_H

void Real_register(struct State *state);
struct Object *Real_clone(struct State *state, struct Object *o);
void Real_free(struct State *state, struct Object *o);
struct Object *Real_eval(struct State *state, struct Object *o, struct Object *basenv, struct Object *env, struct Object *message);

#endif
