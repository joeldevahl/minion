/* main.c
 * Copyright (C) 2007-2008 Joel de Vahl.
 * For conditions of distribution and use, see copyright notice in LICENSE
 */

#include "state.h"

#include "bool.h"
#include "function.h"
#include "integer.h"
#include "list.h"
#include "message.h"
#include "object.h"
#include "real.h"
#include "str.h"

#include <stdio.h>

int main(int argc, char **argv)
{
	struct State state;

	State_init(&state);
	Function_register(&state);
	Object_register(&state);

//	Bool_register(&state);
	Integer_register(&state);
//	List_register(&state);
	Message_register(&state);
//	Real_register(&state);
	String_register(&state);

	State_doFile(&state, "init.io");

	if(argc > 1)
	{
		int i = 1;
		while(i < argc)
		{
			State_doFile(&state, argv[i]);
			i++;
		}
	}
	else
	{
		struct Object *res;
		char str[1024];
		while(1)
		{
			printf("%%> ");
			fflush(stdout);
			fgets(str, 1024, stdin);

			res = State_doString(&state, str);

			printf("=> ");
			if(res)
				Object_deepPrint(&state, res, 0, 1);
			else
				printf("NIL\n");
		}
	}

	State_delete(&state);

	return 0;
}
