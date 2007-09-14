#ifndef MINION_LIST_H
#define MINION_LIST_H

struct State;
struct Object;

void List_register(struct State *state);
struct Object *List_clone(struct State *state, struct Object *o);
void List_free(struct State *state, struct Object *o);
struct Object *List_eval(struct State *state, struct Object *o, struct Object *locals, struct Object *message);

#endif
