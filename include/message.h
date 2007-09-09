#ifndef MINION_MESSAGE_H
#define MINION_MESSAGE_H

struct State;
struct Object;

unsigned IMessage_isMessage(struct Object *o);
void Message_register(struct State *state);
struct Object *Message_clone(struct Object *o);
void Message_free(struct Object *o);
struct Object *Message_eval(struct Object *o, struct Object *locals, struct Object *message);

#endif
