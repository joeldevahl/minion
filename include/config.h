/* config.h
 * Copyright (C) 2007-2008 Joel de Vahl.
 * For conditions of distribution and use, see copyright notice in LICENSE
 */

#ifndef MINION_CONFIG_H
#define MINION_CONFIG_H

#define ALIGNMENT	4
#define NUM_TAGS	(ALIGNMENT-1)

#define USE_TAGGED_POINTERS	1
#define USE_TAGGED_FUNCTION	1
#define USE_TAGGED_INTEGER	1
#define USE_TAGGED_REAL		1
#define USE_TAGGED_STRING	0

#if USE_TAGGED_POINTERS && USE_TAGGED_FUNCTION+USE_TAGGED_INTEGER+USE_TAGGED_REAL+USE_TAGGED_STRING>NUM_TAGS
#error Too many objects to be tagged
#endif

#define TAG_NONE		0x00000000
#define TAG_FUNCTION	0x00000001
#define TAG_INTEGER		0x00000002
#define TAG_REAL		0x00000003

#define TAG_MASK		(~NUM_TAGS)
#define UNTAG(ptr)		(ptr & TAG_MASK)
#define GET_TAG(ptr)	(ptr & ~TAG_MASK)
#define TAG(ptr, tag)	(UNTAG(ptr) | tag)

#endif
