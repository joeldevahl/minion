#include "state.h"
#include "object.h"
#include "lexer.h"
#include "parser.h"
#include "ast.h"

#include "hash.h"

#include <stdio.h>

void State_init(struct State *state)
{
	state->root = Object_new();
	Object_init(state->root);
}

void State_delete(struct State *state)
{
	Object_delete(state->root);
}

void State_registerProto(struct State *state, struct Object *o, const char* name)
{
	unsigned hash = Hash_genHashVal(name);
	Object_setSlot(state->root, o, hash);
}

struct Object *State_getObject(struct State *state)
{
	return state->root;
}

struct Object *State_getProto(struct State *state, const char* name)
{
	unsigned hash = Hash_genHashVal(name);
	return Object_getSlot(state->root, hash);
}

struct Object *State_cloneProto(struct State *state, const char* name)
{
	struct Object *o = State_getProto(state, name);
	if(o)
		return o->clone_func(o);

	return 0x0;
}

struct Object *State_doFile(struct State *state, const char *str)
{
	FILE *file;
	unsigned len;
	char *data;
	struct Object *ret = 0x0;
	
	file = fopen(str, "rb");
	if(!file)
		return 0x0;

	fseek(file, 0, SEEK_END);
	len = ftell(file);
	fseek(file, 0, SEEK_SET);

	data = (char*)malloc((len+2) * sizeof(char));
	fread(data, sizeof(char), len, file);
	data[len] = '\n';
	data[len+1] = '\0';

	fclose(file);

	ret = State_doString(state, data);
	free(data);

	return ret;
}

struct Object *State_doString(struct State *state, const char *str)
{

	struct Parser p;
	struct Lexer l;
	struct AST ast;

	Lexer_init(&l);
	Lexer_setStream(&l, str);

	AST_init(&ast, state);

	Parser_init(&p);
	Parser_setLexer(&p, &l);
	Parser_setAST(&p, &ast);

	Parser_parse(&p);

	//AST_deepPrint(&ast, 0, 0);

	return State_doAST(state, &ast);
}

struct Object *State_doAST(struct State *state, struct AST *ast)
{
	struct Object *o = State_getProto(state, "Object");
	struct Object *msg = AST_getMessageRoot(ast);

	return State_doSeq(o, o, msg);
}

struct Object *State_doSeq(struct Object *o, struct Object *locals, struct Object *message)
{
	struct Object *res;
	
	while(message)
	{
		struct Object *orig_message = message;
		if(message->isLiteral)
		{
			res = o = message;
			message = Object_getSlot(message, CHILD);
		}

		if(message)
		{
			res = State_doChildSeq(o, locals, message);
			message = Object_getSlot(orig_message, NEXT);
		}
	}

	return res;
}

struct Object *State_doChildSeq(struct Object *o, struct Object *locals, struct Object *message)
{
	while(o && message)
	{
		o = Object_evalExpression(o, locals, message);
		message = Object_getSlot(message, CHILD);
	}

	return o;
}
