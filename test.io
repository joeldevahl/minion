2 add(5) sub(1) mul(2) print
"\n" print
"foobar" length print
"\n" print

Object do(
	createSlot(newline, method("\n" print))
	createSlot(test, method(a, a mul(a)))
)

test(2) print
