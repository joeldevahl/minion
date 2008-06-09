/* parser.c
 * Copyright (C) 2007-2008 Joel de Vahl.
 * For conditions of distribution and use, see copyright notice in LICENSE
 */

#include "parser.h"
#include "lexer.h"
#include "ast.h"

void Parser_parseParams(struct Parser *p)
{
	struct AST_State mark;
	unsigned first = 1;

	if(Lexer_currSym(p->lexer) != StartParams)
		return;

	AST_stepParamlist(p->ast);
	AST_mark(p->ast, &mark);
	AST_stepParam(p->ast);

	Lexer_getSym(p->lexer);
	while(1)
	{
		if(Lexer_currSym(p->lexer) == EndParams)
		{
			AST_revert(p->ast, &mark);
			Lexer_getSym(p->lexer);
			break;
		}

		if(Lexer_currSym(p->lexer) == SeparateParams)
		{
			AST_revert(p->ast, &mark);
			AST_stepNext(p->ast);
			AST_mark(p->ast, &mark);
			AST_stepParam(p->ast);

			Lexer_getSym(p->lexer);
		}
		else if(!first)
		{
			AST_stepNext(p->ast);
		}

		Parser_parseMessageTree(p);
		first = 0;
	}
}

void Parser_parseMessage(struct Parser *p)
{
	struct AST_State mark;

	switch(Lexer_currSym(p->lexer))
	{
	case Name:
		AST_mark(p->ast, &mark);
		AST_addMsg(p->ast, Lexer_currSymData(p->lexer), Lexer_currSymLength(p->lexer));
		Lexer_getSym(p->lexer);

		if(Lexer_currSym(p->lexer) == StartParams)
		{
			Parser_parseParams(p);
		}
		AST_revert(p->ast, &mark);
		break;
	case String:
		AST_addString(p->ast, Lexer_currSymData(p->lexer), Lexer_currSymLength(p->lexer));
		Lexer_getSym(p->lexer);
		break;
	case Number:
		AST_addNumber(p->ast, Lexer_currSymData(p->lexer), Lexer_currSymLength(p->lexer));
		Lexer_getSym(p->lexer);
		break;
	default:
		*((int*)(0x0)) = 0;
	}
}

void Parser_parseMessageTree(struct Parser *p)
{
	struct AST_State mark;
	Lexer_dropEnd(p->lexer);

	AST_mark(p->ast, &mark);
	while(	Lexer_currSym(p->lexer) != End &&
			Lexer_currSym(p->lexer) != SeparateParams &&
			Lexer_currSym(p->lexer) != EndParams &&
			Lexer_currSym(p->lexer) != None)
	{
		Parser_parseMessage(p);
		AST_stepChild(p->ast);
	}
	AST_revert(p->ast, &mark);
}

void Parser_init(struct Parser *p)
{
	p->lexer = 0x0;
	p->ast = 0x0;

}

void Parser_setLexer(struct Parser *p, struct Lexer *l)
{
	p->lexer = l;
}

void Parser_setAST(struct Parser *p, struct AST *a)
{
	p->ast = a;
}

void Parser_parse(struct Parser *p)
{
	while(Lexer_getSym(p->lexer))
	{
		Parser_parseMessageTree(p);
		AST_stepNext(p->ast);
	}
}

struct AST *Parser_getAST(struct Parser *p)
{
	return p->ast;
}
