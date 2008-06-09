/* parser.h
 * Copyright (C) 2007-2008 Joel de Vahl.
 * For conditions of distribution and use, see copyright notice in LICENSE
 */

#ifndef MINION_PARSER_H
#define MINION_PARSER_H

struct Parser
{
	struct Lexer *lexer;
	struct AST *ast;
};

void Parser_parseParams(struct Parser *p);
void Parser_parseMessage(struct Parser *p);
void Parser_parseMessageTree(struct Parser *p);
void Parser_init(struct Parser *p);
void Parser_setLexer(struct Parser *p, struct Lexer *l);
void Parser_setAST(struct Parser *p, struct AST *a);
void Parser_parse(struct Parser *p);
struct AST *Parser_getAST(struct Parser *p);

#endif
