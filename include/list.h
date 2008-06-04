#ifndef MINION_LIST_H
#define MINION_LIST_H

void List_register(struct State *state);
struct Object *List_clone(struct State *state, struct Object *o);
void List_free(struct State *state, struct Object *o);
struct Object *List_eval(struct State *state, struct Object *o, struct Object *basenv, struct Object *env, struct Object *message);

#endif
