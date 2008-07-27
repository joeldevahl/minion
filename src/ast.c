/* ast.c
 * Copyright (C) 2007-2008 Joel de Vahl.
 * For conditions of distribution and use, see copyright notice in LICENSE
 */

#include "ast.h"
#include "hash.h"
#include "object.h"
#include "str.h"
#include "state.h"
#include "config.h"

#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

void AST_init(struct AST *ast, struct State *state)
{
	ast->state = state;
	ast->root = State_newObject(state);
	Object_init(ast->state, ast->root);
	ast->obj = ast->root;
	AST_stepChild(ast);
}

void AST_deepPrint(struct AST *ast, unsigned indent, unsigned first)
{
	Object_deepPrint(ast->state, Object_getSlot(ast->state, ast->root, CHILD), indent, first);
}

void AST_addMsg(struct AST *ast, const char *name, unsigned length)
{
	AST_createAndStep(ast, "Message");
	ast->obj->data.ptr = calloc(length + 1, sizeof(char));
	memcpy(ast->obj->data.ptr, name, length * sizeof(char));
}

void AST_addString(struct AST *ast, const char *name, unsigned length)
{
	AST_createAndStep(ast, "String");
	ast->obj->data.ptr = calloc(length - 1, sizeof(char));
	memcpy(ast->obj->data.ptr, name+1, length * sizeof(char) - 2);
	String_unescape(ast->state, ast->obj, 0x0, 0x0);
}

void AST_addNumber(struct AST *ast, const char *val, unsigned length)
{
	AST_createAndStep(ast, "Integer");
	ast->obj->data.ival = atoi(val);
}

void AST_createAndStep(struct AST *ast, const char *proto)
{
	if(!Object_getSlot(ast->state, ast->obj, ast->targetHash))
	{
		struct Object *o = State_cloneProto(ast->state, proto);
		Object_setSlot(ast->state, ast->obj, o, ast->targetHash);
	}
	AST_step(ast);
}

void AST_step(struct AST *ast)
{
	ast->obj = Object_getSlot(ast->state, ast->obj, ast->targetHash);
}

void AST_stepChild(struct AST *ast)
{
	ast->targetHash = CHILD;
}

void AST_stepNext(struct AST *ast)
{
	ast->targetHash = NEXT;
}

void AST_stepParamlist(struct AST *ast)
{
	ast->targetHash = PARAMS;
}

void AST_stepParam(struct AST *ast)
{
	AST_addMsg(ast, "PARAM", 5);
	AST_stepChild(ast);
}

void AST_mark(struct AST *ast, struct AST_State *mark)
{
	mark->obj = ast->obj;
	mark->targetHash = ast->targetHash;
}

void AST_revert(struct AST *ast, struct AST_State *mark)
{
	ast->obj = mark->obj;
	ast->targetHash = mark->targetHash;
	AST_step(ast);
}

struct Object *AST_getMessageRoot(struct AST *ast)
{
	return Object_getSlot(ast->state, ast->root, CHILD);
}
