#ifndef MINION_LIST_H
#define MINION_LIST_H

struct State;
struct Object;

void List_register(struct State *state);
struct Object *List_clone(struct Object *o);
void List_free(struct Object *o);
struct Object *List_eval(struct Object *o, struct Object *locals, struct Object *message);

#endif
