/* message.h
 * Copyright (C) 2007-2008 Joel de Vahl.
 * For conditions of distribution and use, see copyright notice in LICENSE
 */

#ifndef MINION_MESSAGE_H
#define MINION_MESSAGE_H

struct State;
struct Object;

unsigned Message_isMessage(struct State *state, struct Object *o);
void Message_register(struct State *state);
struct Object *Message_clone(struct State *state, struct Object *o);
void Message_free(struct State *state, struct Object *o);
struct Object *Message_eval(struct State *state, struct Object *o, struct Object *env, struct Object *message);

#endif
