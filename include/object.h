#ifndef MINION_OBJECT_H
#define MINION_OBJECT_H

#define CHILD Hash_genHashVal("child")
#define NEXT Hash_genHashVal("next")
#define PARAMS Hash_genHashVal("params")
#define NAME Hash_genHashVal("name")

typedef struct Object *(*CloneFunc)(struct State *, struct Object *);
typedef void (*FreeFunc)(struct State *, struct Object *);
typedef struct Object *(*EvalFunc)(struct State *, struct Object *, struct Object *, struct Object *, struct Object *);

struct Object
{
	union
	{
		void *ptr;
		char *str;
		unsigned ival;
		float rval;
	} data;

	unsigned isLiteral : 1;

	struct Hash *hash;

	CloneFunc clone_func;
	FreeFunc free_func;
	EvalFunc eval_func;
};

struct Object *Object_new();
void Object_delete(struct State *state, struct Object *o);
void Object_init(struct State *state, struct Object *o);
void Object_register(struct State *state);
void Object_registerFunction(struct State *state, struct Object *o, const char *name, void *ptr);
struct Object *Object_clone(struct State *state, struct Object *o);
void Object_free(struct State *state, struct Object *o);
struct Object *Object_eval(struct State *state, struct Object *o, struct Object *basenv, struct Object *env, struct Object *message);
struct Object *Object_evalExpression(struct State *state, struct Object *o, struct Object *basenv, struct Object *env, struct Object *message);
void Object_deepPrint(struct State *state, struct Object *o, unsigned indent, unsigned first);
void Object_createSlot(struct State *state, struct Object *o, unsigned name_hash);
void Object_setSlot(struct State *state, struct Object *o, struct Object *val, unsigned name_hash);
struct Object *Object_getSlot(struct State *state, struct Object *o, unsigned name_hash);
void Object_appendProto(struct State *state, struct Object *o, struct Object *proto);

#endif
