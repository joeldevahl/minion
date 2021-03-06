/* bool.h
 * Copyright (C) 2007-2008 Joel de Vahl.
 * For conditions of distribution and use, see copyright notice in LICENSE
 */

#ifndef MINION_BOOL_H
#define MINION_BOOL_H

struct State;
struct Object;

void Bool_register(struct State *state);
struct Object *Bool_clone(struct State *state, struct Object *o);
void Bool_free(struct State *state, struct Object *o);
struct Object *Bool_eval(struct State *state, struct Object *o, struct Object *env, struct Object *message);

#endif
