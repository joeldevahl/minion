/* lexer.h
 * Copyright (C) 2007-2008 Joel de Vahl.
 * For conditions of distribution and use, see copyright notice in LICENSE
 */

#ifndef MINION_LEXER_H
#define MINION_LEXER_H

struct Lexer
{
	enum Symbols {None, Name, End, StartParams, EndParams, SeparateParams, String, Char, Number} sym;
	const char *symData;
	unsigned symLength;

	unsigned lineNumber;
	const char *fileName;

	const char *stream;
};

void Lexer_init(struct Lexer *l);
void Lexer_setStream(struct Lexer *l, const char *s);
enum Symbols Lexer_getSym(struct Lexer *l);
enum Symbols Lexer_currSym(struct Lexer *l);
const char *Lexer_currSymData(struct Lexer *l);
unsigned Lexer_currSymLength(struct Lexer *l);
void Lexer_dropEnd(struct Lexer *l);

#endif
