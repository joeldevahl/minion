#ifndef MINION_REAL_H
#define MINION_REAL_H

struct State;
struct Object;

void Real_register(struct State *state);
struct Object *Real_clone(struct Object *o);
void Real_free(struct Object *o);
struct Object *Real_eval(struct Object *o, struct Object *locals, struct Object *Real);

#endif
