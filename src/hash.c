#include "hash.h"

#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <math.h>

void Hash_init(struct Hash *h, unsigned size)
{
	h->elems = (struct HashElem*)calloc(size, sizeof(struct HashElem));
	h->elemsSize = size;
	h->numElems = 0;
}

void Hash_delete(struct Hash *h)
{
	free(h->elems);
	h->elems = 0x0;
	h->elemsSize = 0;
	h->numElems = 0;
}

void Hash_grow(struct Hash *h, unsigned size)
{
	struct HashElem *elems = (struct HashElem*)calloc(size, sizeof(struct HashElem));
	memcpy(elems, h->elems, h->elemsSize * sizeof(struct HashElem));
	free(h->elems);
	h->elems = elems;
	h->elemsSize = size;
}

void Hash_copy(struct Hash *dst, struct Hash *src)
{
	Hash_delete(dst);
	Hash_init(dst, src->elemsSize);

	dst->elemsSize = src->elemsSize;
	dst->numElems = src->numElems;

	memcpy(dst->elems, src->elems, src->numElems * sizeof(struct HashElem));
}

unsigned Hash_genHashVal(const char *s){
	return Hash_genHashVal_len(s, (unsigned)strlen(s));
}

unsigned Hash_genHashVal_len(const char *s, unsigned length)
{
	unsigned n, i;
	unsigned hashval;
	unsigned ival;
	char *p;

	p = (char *) &ival;
	hashval = ival = 0;
	n = (unsigned)((((log10((double)(UINT_MAX)) / log10(2.0))) / CHAR_BIT) + 0.5);
	for(i = 0; i < length; i += n) {
		strncpy(p, s + i, n);
		hashval += ival;
	}

	return hashval;
}

void *Hash_get(struct Hash *h, const char *name)
{
	unsigned name_hash = Hash_genHashVal(name);
	return Hash_get_h(h, name_hash);
}

void *Hash_get_h(struct Hash *h, unsigned name_hash)
{
	struct HashElem *elem = Hash_find_h(h, name_hash);

	if(elem)
		return elem->data;

	return 0x0;
}

void **Hash_getHandle(struct Hash *h, const char *name)
{
	unsigned name_hash = Hash_genHashVal(name);
	return Hash_getHandle_h(h, name_hash);
}

void **Hash_getHandle_h(struct Hash *h, unsigned name_hash)
{
	struct HashElem *elem = Hash_find_h(h, name_hash);

	if(elem)
		return &(elem->data);

	return 0x0;
}

struct HashElem *Hash_find(struct Hash *h, const char *name)
{
	unsigned name_hash = Hash_genHashVal(name);
	return Hash_find_h(h, name_hash);
}

struct HashElem *Hash_find_h(struct Hash *h, unsigned name_hash)
{
	unsigned i = 0;

	while(i < h->numElems)
	{
		if(name_hash == h->elems[i].hash)
			return &(h->elems[i]);
		i++;
	}

	return 0x0;
}

void Hash_set(struct Hash *h, const char *name, void *data)
{

	unsigned name_hash = Hash_genHashVal(name);
	Hash_set_h(h, name_hash, data);
}

void Hash_set_h(struct Hash *h, unsigned name_hash, void *data)
{
	struct HashElem *elem = Hash_find_h(h, name_hash);
	if(elem)
	{
		elem->data = data;
	}
	else
	{
		if(h->numElems >= h->elemsSize)
			Hash_grow(h, h->elemsSize << 1);

		h->elems[h->numElems].hash = name_hash;
		h->elems[h->numElems].data = data;
		h->numElems++;
	}
}

void *Hash_remove(struct Hash *h, const char *name)
{
	unsigned name_hash = Hash_genHashVal(name);
	return Hash_remove_h(h, name_hash);
}

void *Hash_remove_h(struct Hash *h, unsigned name_hash)
{
	struct HashElem *elem = Hash_find_h(h, name_hash);
	void *data = 0x0;

	if(elem)
	{
		unsigned pos = (unsigned)(elem - h->elems);
		data = elem->data;
		memmove(&(h->elems[pos]), &(h->elems[pos + 1]), (h->numElems - pos - 1) * sizeof(struct HashElem));
		h->numElems--;
	}

	return 0x0;
}
