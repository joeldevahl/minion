#include "state.h"

#include "bool.h"
#include "function.h"
#include "integer.h"
#include "list.h"
#include "message.h"
#include "object.h"
#include "real.h"
#include "string.h"

#include "hash.h"

int main()
{
	struct State state;

	State_init(&state);
	Function_register(&state);
	Object_register(&state);

	Bool_register(&state);
	Integer_register(&state);
	List_register(&state);
	Message_register(&state);
	Real_register(&state);
	String_register(&state);

	State_doFile(&state, "test.io");

	State_delete(&state);

	return 0;
}
