/* lexer.c
 * Copyright (C) 2007-2008 Joel de Vahl.
 * For conditions of distribution and use, see copyright notice in LICENSE
 */

#include "lexer.h"

#include <string.h>
#include <stdio.h>

void Lexer_init(struct Lexer *l)
{
	l->sym = None;
	l->symData = 0x0;
	l->symLength = 0;

	l->lineNumber = 0;
	l->fileName = 0x0;

	l->stream = 0x0;
}

void Lexer_setStream(struct Lexer *l, const char *s)
{
	l->stream = s;
}

enum Symbols Lexer_getSym(struct Lexer *l)
{
	unsigned len = 1;
	unsigned rerun = 0;
	l->stream += strspn(l->stream, " \t");

	switch(*(l->stream))
	{
	case '/':
		rerun = 1;
		if(*(l->stream + len) == '/')
		{
			len += (unsigned)strcspn(l->stream + len, "\r\n");
		}
		else if(*(l->stream + len) == '*')
		{
			len++;
			do {
				len += (unsigned)strcspn(l->stream + len, "*") + 1;
			} while(*(l->stream + len) != '/');
		}
		break;
	case '(':
		l->sym = StartParams;
		break;
	case ')':
		l->sym = EndParams;
		break;
	case ',':
		l->sym = SeparateParams;
		break;
	case '\'':
		l->sym = Char;
		break;
	case '"':
		l->sym = String;
		do {
			len += (unsigned)strcspn(l->stream + len, "\"");
		} while(*(l->stream + len) == '\\');
		len++;
		break;
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case '.':
		l->sym = Number;
		len = (unsigned)strspn(l->stream, "0123456789.");
		break;
	case ';':
	case '\n':
	case '\r':
		l->sym = End;
		len = (unsigned)strspn(l->stream, " \t\r\n;");
		break;
	case '\0':
		len = 0;
	default:
		len = (unsigned)strcspn(l->stream, "(), \t\r\n;");
		l->sym = Name;
	}

	l->symData = l->stream;
	l->symLength = len;
	l->stream += len;

	if(rerun == 1)
	{
		Lexer_getSym(l);
	}
	else if(len == 0)
	{
		l->sym = None;
	}

	return l->sym;
}

enum Symbols Lexer_currSym(struct Lexer *l)
{
	return l->sym;
}

const char *Lexer_currSymData(struct Lexer *l)
{
	return l->symData;
}

unsigned Lexer_currSymLength(struct Lexer *l)
{
	return l->symLength;
}

void Lexer_dropEnd(struct Lexer *l)
{
	while(l->sym == End)
		Lexer_getSym(l);
}
