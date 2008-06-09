/* list.h
 * Copyright (C) 2007-2008 Joel de Vahl.
 * For conditions of distribution and use, see copyright notice in LICENSE
 */

#ifndef MINION_LIST_H
#define MINION_LIST_H

struct State;
struct Object;

void List_register(struct State *state);
struct Object *List_clone(struct State *state, struct Object *o);
void List_free(struct State *state, struct Object *o);
struct Object *List_eval(struct State *state, struct Object *o, struct Object *env, struct Object *message);

#endif
