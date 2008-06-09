/* real.h
 * Copyright (C) 2007-2008 Joel de Vahl.
 * For conditions of distribution and use, see copyright notice in LICENSE
 */

#ifndef MINION_REAL_H
#define MINION_REAL_H

struct State;
struct Object;

void Real_register(struct State *state);
struct Object *Real_clone(struct State *state, struct Object *o);
void Real_free(struct State *state, struct Object *o);
struct Object *Real_eval(struct State *state, struct Object *o, struct Object *env, struct Object *message);

#endif
