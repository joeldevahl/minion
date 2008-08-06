Object do(
	setSlot(createIncrementor, method(a, b,
		method( setSlot(temp, a)
				setSlot(a, a add(b))
				temp)))
	setSlot(simpleIncr, createIncrementor(1, 1))
)
