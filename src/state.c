#include "state.h"
#include "object.h"
#include "lexer.h"
#include "parser.h"
#include "ast.h"

#include "hash.h"

#include <stdio.h>
#include <stdlib.h>

void State_init(struct State *state)
{
	state->root = Object_new(state);
	Object_init(state, state->root);
}

void State_delete(struct State *state)
{
	Object_delete(state, state->root);
}

void State_registerProto(struct State *state, struct Object *o, const char* name)
{
	unsigned hash = Hash_genHashVal(name);
	Object_setSlot(state, state->root, o, hash);
}

struct Object *State_getObject(struct State *state)
{
	return state->root;
}

struct Object *State_getProto(struct State *state, const char* name)
{
	unsigned hash = Hash_genHashVal(name);
	return Object_getSlot(state, state->root, hash);
}

struct Object *State_cloneProto(struct State *state, const char* name)
{
	struct Object *o = State_getProto(state, name);
	if(o)
		return o->clone_func(state, o);

	return 0x0;
}

struct Object *State_doFile(struct State *state, const char *str)
{
	FILE *file;
	unsigned len;
	char *data;
	struct Object *ret = 0x0;

	//printf("\033[0;40;32mState_doFile\033[0m\t\t%s\n", str);
	
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

	//printf("\033[0;40;32mState_doString\033[0m\t\t%s", str);

	Lexer_init(&l);
	Lexer_setStream(&l, str);

	AST_init(&ast, state);

	Parser_init(&p);
	Parser_setLexer(&p, &l);
	Parser_setAST(&p, &ast);

	Parser_parse(&p);


	return State_doAST(state, &ast);
}

struct Object *State_doAST(struct State *state, struct AST *ast)
{
	struct Object *o = State_getProto(state, "Object");
	struct Object *msg = AST_getMessageRoot(ast);

	//printf("\033[0;40;32mState_doAST:\033[0m\n");
	//AST_deepPrint(ast, 0, 0);

	return State_doSeq(state, o, o, msg);
}

struct Object *State_doSeq(struct State *state, struct Object *o, struct Object *env, struct Object *message)
{
	struct Object *res = 0x0;
	
	//printf("\033[0;40;32mState_doSeq:\033[0m\n");
	//if(message && message->data.ival > 1000)
	//	printf("message %s\t", message->data.ptr);
	//printf(" o = %x", o);
	//printf(" e = %x\n", env);
	
	while(message)
	{
		struct Object *orig_message = message;
		if(message->isLiteral)
		{
			//printf("literal %d\t", message->data.ptr);
			//printf(" o = %x", o);
			//printf(" e = %x\n", env);
			res = o = message;
			message = Object_getSlot(state, message, CHILD);
		}

		if(message)
		{
			res = State_doChildSeq(state, o, env, message);
			message = Object_getSlot(state, orig_message, NEXT);
		}
	}

	return res;
}

struct Object *State_doChildSeq(struct State *state, struct Object *o, struct Object *env, struct Object *message)
{
	//printf("\033[0;40;32mState_doChildSeq:\033[0m\n");
	while(o && message)
	{
		//printf("message %s\t", message->data.ptr);
		//printf(" o = %x", o);
		//printf(" e = %x\n", env);
		o = Object_evalExpression(state, o, env, message);
		message = Object_getSlot(state, message, CHILD);
	}

	return o;
}
