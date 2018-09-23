#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stddef.h>
#include <stdint.h>

#define MAX(x,y) ((x) >= (y) ? (x) : (y))

typedef struct BufHdr {
	size_t len;
	size_t cap;
	char buf[0];
} BufHdr;

#define buf__hdr(b) ((BufHdr*)((char *)(b) - (offsetof(BufHdr, buf))))
#define buf__fits(b,n) (buf_len(b) + (n) <= buf_cap(b))
#define buf__fit(b,n) (buf__fits(b,n) ? 0: (b = buf__grow((b),buf_len(b) + (n), sizeof(*(b))))) 

#define buf_len(b) ((b) ? buf__hdr(b)->len : 0)
#define buf_cap(b) ((b) ? buf__hdr(b)->cap : 0)
#define buf_push(b,x) (buf__fit(b, 1), (b)[buf_len(b)] = (x), buf__hdr(b)->len++)
#define buf_free(b) ((b) ? free(buf__hdr(b)): 0)

void *buf__grow(const void *buf, int new_len, size_t elem_size) {
	size_t new_cap = MAX(1 + 2 * buf_cap(buf), new_len);
	assert(new_cap >= new_len);

	size_t new_size = offsetof(BufHdr, buf) + new_cap * elem_size;

	BufHdr *new_hdr;

	if (buf) {
		new_hdr = realloc(buf__hdr(buf), new_size);
	}
	else {
		new_hdr = malloc(new_size);
		new_hdr->len = 0;
	}
	new_hdr->cap = new_cap;

	return new_hdr->buf;
}


int main(int argc, char **argv) {
	for (int i = 0; i < argc; i++) {
		printf("Running program at path %s\n", argv[i]);
	}

	int *lala = NULL;
	buf_push(lala, 42);
	buf_push(lala, 420);
	for (int i = 0; i < buf_len(lala); i++) {
		printf("%d\n", lala[i]);
	}
	buf_free(lala);
	return 0;
}