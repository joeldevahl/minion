/* ast.h
 * Copyright (C) 2007-2008 Joel de Vahl.
 * For conditions of distribution and use, see copyright notice in LICENSE
 */

#ifndef MINION_AST_H
#define MINION_AST_H

struct AST
{
	struct State *state;
	struct Object *root;
	struct Object *obj;
	unsigned targetHash;
};

struct AST_State
{
	struct Object *obj;
	unsigned targetHash;
};

void AST_init(struct AST *ast, struct State *state);
void AST_deepPrint(struct AST *ast, unsigned indent, unsigned first);
void AST_addMsg(struct AST *ast, const char *name, unsigned length);
void AST_addString(struct AST *ast, const char *name, unsigned length);
void AST_addNumber(struct AST *ast, const char *val, unsigned length);
void AST_createAndStep(struct AST *ast, const char *proto);
void AST_step(struct AST *ast);
void AST_stepChild(struct AST *ast);
void AST_stepNext(struct AST *ast);
void AST_stepParamlist(struct AST *ast);
void AST_stepParam(struct AST *ast);
void AST_mark(struct AST *ast, struct AST_State *mark);
void AST_revert(struct AST *ast, struct AST_State *mark);
struct Object *AST_getMessageRoot(struct AST *ast);

#endif
