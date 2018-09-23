#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <ctype.h>

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

int buf_test() {
	
	int *lala = NULL;
	buf_push(lala, 42);
	buf_push(lala, 420);
	for (int i = 0; i < buf_len(lala); i++) {
		printf("%d\n", lala[i]);
	}
	buf_free(lala);
	return 1;
}

typedef enum TokenKind {
	TOKEN_INT = 128,
	TOKEN_NAME,
	//...
}TokenKind;

typedef struct Token {
	TokenKind kind;
	union {
		uint64_t val;
	};
}Token;

const char *stream;
Token token;

void next_token() {
	switch (*stream) {
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
			{
				uint64_t  val = 0;
				while (isdigit(*stream)) {
					val *= 10;
					val += *stream++ - '0';
				}
				token.kind = TOKEN_INT;
				token.val = val;
				break;
			}
		case 'a':
		case 'b':
		case 'c':
		case 'd':
		case 'e':
		case 'f':
		case 'g':
		case 'h':
		case 'i':
		case 'j':
		case 'k':
		case 'l':
		case 'm':
		case 'n':
		case 'o':
		case 'p':
		case 'q':
		case 'r':
		case 's':
		case 't':
		case 'u':
		case 'v':
		case 'w':
		case 'x':
		case 'y':
		case 'z':
		case 'A':
		case 'B':
		case 'C':
		case 'D':
		case 'E':
		case 'F':
		case 'G':
		case 'H':
		case 'I':
		case 'J':
		case 'K':
		case 'L':
		case 'M':
		case 'N':
		case 'O':
		case 'P':
		case 'Q':
		case 'R':
		case 'S':
		case 'T':
		case 'U':
		case 'V':
		case 'W':
		case 'X':
		case 'Y':
		case 'Z':
		case '_': {
			//TODO: lexing of identifiers: kind = TOKEN_NAME + store a start and end position
		}
		default:
			token.kind = *stream++;
			break;
	}
}


int lex_test() {
	char *source = "+-[]123456789s";
	stream = source;
	next_token();
	while (token.kind) {
		printf("TOKEN: %d\n", token.kind);
		next_token();
	}
}


int main(int argc, char **argv) {
	for (int i = 0; i < argc; i++) {
		printf("Running program at path %s\n", argv[i]);
	}
	if (buf_test()) {
		printf("passed buf_test\n");
	}
	if (lex_test()) {
		printf("passed lex_test\n");
	}
	return 0;
}