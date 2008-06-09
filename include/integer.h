/* integer.h
 * Copyright (C) 2007-2008 Joel de Vahl.
 * For conditions of distribution and use, see copyright notice in LICENSE
 */

#ifndef MINION_INTEGER_H
#define MINION_INTEGER_H

struct State;
struct Object;

unsigned Integer_isInteger(struct State *state, struct Object *o);
void Integer_register(struct State *state);
struct Object *Integer_clone(struct State *state, struct Object *o);
void Integer_free(struct State *state, struct Object *o);
struct Object *Integer_eval(struct State *state, struct Object *o, struct Object *env, struct Object *message);

#endif
