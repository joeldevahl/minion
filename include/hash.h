#ifndef MINION_HASH_H
#define MINION_HASH_H

struct HashElem
{
	unsigned hash;
	void *data;
};

struct Hash{
	struct HashElem *elems;
	unsigned elemsSize;
	unsigned numElems;
};

void Hash_init(struct Hash *h, unsigned size);
void Hash_delete(struct Hash *h);

void Hash_grow(struct Hash *h, unsigned size);
void Hash_copy(struct Hash *dst, struct Hash *src);

unsigned Hash_genHashVal(const char *s);
unsigned Hash_genHashVal_len(const char *s, unsigned length);

void *Hash_get(struct Hash *h, const char *name);
void *Hash_get_h(struct Hash *h, unsigned name_hash);
void **Hash_getHandle(struct Hash *h, const char *name);
void **Hash_getHandle_h(struct Hash *h, unsigned name_hash);

struct HashElem *Hash_find(struct Hash *h, const char *name);
struct HashElem *Hash_find_h(struct Hash *h, unsigned name_hash);

void Hash_set(struct Hash *h, const char *name, void *data);
void Hash_set_h(struct Hash *h, unsigned name_hash, void *data);

void *Hash_remove(struct Hash *h, const char *name);
void *Hash_remove_h(struct Hash *h, unsigned name_hash);

#endif
