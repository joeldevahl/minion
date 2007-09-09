#ifndef MINION_OBJECT_H
#define MINION_OBJECT_H

#define CHILD Hash_genHashVal("child")
#define NEXT Hash_genHashVal("next")
#define PARAMS Hash_genHashVal("params")
#define NAME Hash_genHashVal("name")

struct State;
struct Hash;

struct Object
{
	union
	{
		void *ptr;
		char *str;
		unsigned val;
	} data;

	unsigned isLiteral : 1;

	struct Hash *hash;

	struct Object *(*clone_func)(struct Object *);
	void (*free_func)(struct Object *);
	struct Object *(*eval_func)(struct Object *, struct Object *, struct Object *);
};

struct Object *Object_new();
void Object_delete(struct Object *o);
void Object_init(struct Object *o);
void Object_register(struct State *state);
void Object_registerFunction(struct State *state, struct Object *o, const char *name, void *ptr);
struct Object *Object_clone(struct Object *o);
void Object_free(struct Object *o);
struct Object *Object_eval(struct Object *o, struct Object *locals, struct Object *message);
struct Object *Object_evalExpression(struct Object *o, struct Object *locals, struct Object *message);
void Object_deepPrint(struct Object *o, unsigned indent, unsigned first);
void Object_createSlot(struct Object *o, unsigned name_hash);
void Object_setSlot(struct Object *o, struct Object *val, unsigned name_hash);
struct Object *Object_getSlot(struct Object *o, unsigned name_hash);
void Object_appendProto(struct Object *o, struct Object *proto);

#endif
