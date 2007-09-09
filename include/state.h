#ifndef MINION_STATE_H
#define MINION_STATE_H

struct Object;
struct AST;

struct Object;
struct AST;

struct State
{
	struct Object *root;
};

void State_init(struct State *state);
void State_delete(struct State *state);
void State_registerProto(struct State *state, struct Object *o, const char* name);
struct Object *State_getObject(struct State *state);
struct Object *State_getProto(struct State *state, const char* name);
struct Object *State_cloneProto(struct State *state, const char* name);
struct Object *State_doFile(struct State *state, const char *str);
struct Object *State_doString(struct State *state, const char *str);
struct Object *State_doAST(struct State *state, struct AST *ast);
struct Object *State_doSeq(struct Object *o, struct Object *locals, struct Object *message);
struct Object *State_doChildSeq(struct Object *o, struct Object *locals, struct Object *message);

#endif
