/* Copyright (c) 2013-2014 Yoran Heling

  Permission is hereby granted, free of charge, to any person obtaining
  a copy of this software and associated documentation files (the
  "Software"), to deal in the Software without restriction, including
  without limitation the rights to use, copy, modify, merge, publish,
  distribute, sublicense, and/or sell copies of the Software, and to
  permit persons to whom the Software is furnished to do so, subject to
  the following conditions:

  The above copyright notice and this permission notice shall be included
  in all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "yxml.h"
#include <string.h>

/* The smaller this value, the more memory yxml_parse() will use on the system
 * stack. A value of 256 will use about 1KB of stack space. */
#define YXML_STACK_SIZE 256

/* Don't edit anything below this line. */
#define YXML_BUF_SIZE (YXML_STACK_SIZE-sizeof(void *)-sizeof(int))

typedef enum {
	YXMLS_string,
	YXMLS_attr0,
	YXMLS_attr1,
	YXMLS_attr2,
	YXMLS_attr3,
	YXMLS_cd0,
	YXMLS_cd1,
	YXMLS_cd2,
	YXMLS_comment0,
	YXMLS_comment1,
	YXMLS_comment2,
	YXMLS_dt0,
	YXMLS_dt1,
	YXMLS_elem0,
	YXMLS_elem1,
	YXMLS_elem2,
	YXMLS_elem3,
	YXMLS_enc0,
	YXMLS_enc1,
	YXMLS_enc2,
	YXMLS_etag0,
	YXMLS_etag1,
	YXMLS_etag2,
	YXMLS_init,
	YXMLS_le0,
	YXMLS_le1,
	YXMLS_le2,
	YXMLS_le3,
	YXMLS_misc,
	YXMLS_pi0,
	YXMLS_pi1,
	YXMLS_pi2,
	YXMLS_pi3,
	YXMLS_std0,
	YXMLS_std1,
	YXMLS_std2,
	YXMLS_ver0,
	YXMLS_ver1,
	YXMLS_ver2,
	YXMLS_xml0,
	YXMLS_xml1,
	YXMLS_xml2
} yxml_state_t;


static inline yxml_ret_t yxml_datacontent(yxml_t *, unsigned int);
static inline yxml_ret_t yxml_datapi(yxml_t *, unsigned int);
static inline yxml_ret_t yxml_dataattr(yxml_t *, unsigned int);


void yxml_init(yxml_t *x, void *stack, size_t stacksize) {
	memset(x, 0, sizeof(*x));
	x->line = 1;
	x->stack = stack;
	x->stacksize = stacksize;
	x->stack[0] = 0;
	x->elem = (char *)x->stack;
	x->state = YXMLS_init;
}


/* Save character `ch` to the character buffer.
 * Returns YXML_OK on success, YXML_ESTACK if the buffer is full. */
static inline yxml_ret_t yxml_push(yxml_t *x, unsigned int ch) {
	if(x->stacklen < x->stacksize) {
		x->stack[x->stacklen++] = ch;
		return YXML_OK;
	}
	return YXML_ESTACK;
}


/* Pop one character from the character buffer.
 * Returns YXML_OK on success, YXML_EINTERNAL if the buffer is empty. */
static inline void yxml_pop(yxml_t *x) {
	if(x->stacklen)
		x->stacklen--;
}


/* Returns the last character pushed to the stack, or 0 if the stack is empty. */
static inline unsigned int yxml_peek(yxml_t *x) {
	return x->stacklen ? x->stack[x->stacklen-1] : 0;
}


/* Wrappers around yxml_push() and yxml_pop() to push/pop a name from the stack. */
static inline yxml_ret_t yxml_push_name(yxml_t *x, const char *name, int namelen) {
	if(x->stacklen+namelen+1 > x->stacksize)
		return YXML_ESTACK;
	memcpy(x->stack+x->stacklen, name, namelen);
	x->stacklen += namelen;
	x->stack[x->stacklen++] = 0;
	return YXML_OK;
}


static inline void yxml_pop_name(yxml_t *x) {
	if(!x->stacklen)
		return;
	do
		x->stacklen--;
	while(x->stacklen && x->stack[x->stacklen-1]);
}


size_t yxml_symlen(yxml_t *x, const char *s) {
	unsigned char *b = (unsigned char *)s;
	/* This is a rather nasty trick that relies on the exact stack layout of
	 * yxml. The space between s and the top of the stack is the length of s. */
	if(b < x->stack || b > x->stack+x->stacklen)
		return 0;
	return (x->stack+x->stacklen) - b;
}


/* Character reference parsing.
 * Returns YXML_OK if the reference was parsed, YXML_EREF if the reference is
 * invalid, or YXML_ELEMSTART if a new element was found. */
static yxml_ret_t yxml_ref(yxml_t *x, unsigned int ch) {
	yxml_ret_t r = YXML_OK;
	if(x->reflen == 0) {
		if(ch == '#') {
			x->reflen = -1;
			return YXML_OK;
		}
		x->reflen = 1;
	}

	if(x->reflen > 0) { /* Character entity */
		if(ch == ';') {
			unsigned int c = 0;
			/* We have to do a string comparison here. The alternatives are a
			 * perfect hash function (too much code) or a big switch
			 * statement (too much code). String comparison is small, but has
			 * the disadvantage of being relatively slow. Fortunately, entity
			 * references are not used that often. */
			char *ref = (char *)x->stack+x->stacklen-x->reflen+1;
			if(x->reflen == 3 && !memcmp(ref, "amp", 3))
				c = '&';
			else if(x->reflen == 3 && !memcmp(ref, "lt;", 3))
				c = '<';
			else if(x->reflen == 3 && !memcmp(ref, "gt;", 3))
				c = '>';
			else if(x->reflen == 4 && !memcmp(ref, "apos", 4))
				c = '\'';
			else if(x->reflen == 4 && !memcmp(ref, "quot", 4))
				c = '"';
			else
				return YXML_EREF;
			x->stacklen -= x->reflen-1;
			x->reflen = 0;
			if(x->state == YXMLS_attr2)
				r = yxml_dataattr(x, c);
			else
				r = yxml_datacontent(x, c);
		} else if(x->reflen < 6 && ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))) {
			x->reflen++;
			yxml_push(x, ch);
		} else
			return YXML_EREF;

	} else { /* Numeric character reference */
		unsigned int c = 0;
		if(ch == ';') {
			char *ref = (char *)x->stack+x->stacklen+x->reflen+1;
			/* Hex */
			if(*ref == 'x') {
				ref++;
				while(*ref) {
					c <<= 4;
					if(*ref >= '0' && *ref <= '9')
						c += *ref-'0';
					else if(*ref >= 'a' && *ref <= 'f')
						c += *ref-'a'+10;
					else if(*ref >= 'A' && *ref <= 'F')
						c += *ref-'A'+10;
					else
						return YXML_EREF;
					ref++;
				}
			/* Dec */
			} else {
				while(*ref) {
					c *= 10;
					if(*ref >= '0' && *ref <= '9')
						c += *ref-'0';
					else
						return YXML_EREF;
					ref++;
				}
			}
			x->stacklen += x->reflen+1;
			x->reflen = 0;
			if(x->state == YXMLS_attr2)
				r = yxml_dataattr(x, c);
			else
				r = yxml_datacontent(x, c);
		} else if(x->reflen < -10 && ((ch >= '0' && ch <= '9') || (x->reflen == -2 && (ch == 'x' || ch == 'X')) || (x->reflen < -2 && ((ch >= 'a' && ch <= 'f') || (ch >= 'A' && ch <= 'F'))))) {
			x->reflen--;
			yxml_push(x, ch);
		} else
			return YXML_EREF;
	}
	return r;
}


static inline yxml_ret_t yxml_datacontent(yxml_t *x, unsigned int ch) {
	/* We can't just leave the data in the stack buffer, it may be overwritten
	 * by the next tag. Instead, we put it in a small buffer in the yxml_t struct.
	 * If the character is a multi-byte UTF8 character, we have to handle that
	 * as well. See http://en.wikipedia.org/wiki/UTF-8#Description */
	x->data[0] = 0;
	if(ch < 0x80) {
		x->data[0] = ch;
		x->data[1] = 0;
	} else if(x->utf8) {
		if(x->utf8 == 1) { /* Has to be 10xxxxxx */
			if((ch & 0xC0) == 0x80) {
				x->data[0] = ((x->data[0] & 0x1F) << 6) | (ch & 0x3F);
				x->utf8 = 0;
			} else
				return YXML_ESYN;
		} else if(x->utf8 == 2) { /* Has to be 110xxxxx 10xxxxxx */
			if((ch & 0xC0) == 0x80) {
				x->data[1] = ch;
				x->utf8 = 1;
			} else
				return YXML_ESYN;
		} else if(ch >= 0xC0 && ch < 0xE0) { /* 110xxxxx */
			x->data[0] = ch;
			x->utf8 = 2;
		} else if(ch >= 0xE0 && ch < 0xF0) { /* 1110xxxx */
			/* TODO */
		} else if(ch >= 0xF0 && ch < 0xF8) { /* 11110xxx */
			/* TODO */
		} else
			return YXML_ESYN;

		/* TODO: Not all values are valid characters */

		/* If we're still in a multi-byte sequence, don't return YXML_CONTENT */
		if(x->utf8)
			return YXML_OK;
	} else if(ch < 0x800) {
		x->data[0] = 0xC0 | (ch>>6);
		x->data[1] = 0x80 | (ch & 0x3F);
		x->data[2] = 0;
	} else if(ch < 0x10000) {
		x->data[0] = 0xE0 | (ch>>12);
		x->data[1] = 0x80 | ((ch>>6) & 0x3F);
		x->data[2] = 0x80 | (ch & 0x3F);
		x->data[3] = 0;
	} else if(ch < 0x200000) {
		x->data[0] = 0xF0 | (ch>>18);
		x->data[1] = 0x80 | ((ch>>12) & 0x3F);
		x->data[2] = 0x80 | ((ch>>6) & 0x3F);
		x->data[3] = 0x80 | (ch & 0x3F);
		x->data[4] = 0;
	} else
		return YXML_EREF;
	return YXML_CONTENT;
}


static inline yxml_ret_t yxml_datapi(yxml_t *x, unsigned int ch) {
	yxml_datacontent(x, ch);
	return YXML_PICONTENT;
}


static inline yxml_ret_t yxml_dataattr(yxml_t *x, unsigned int ch) {
	yxml_datacontent(x, ch);
	return YXML_ATTRVAL;
}



yxml_ret_t yxml_parse(yxml_t *x, intich) {
	/* The parser is implemented as a big state machine, using a goto for each
	 * state transition. A function pointer table is smaller, but a big switch
	 * statement is usually faster. A goto is the fastest of them all, but is
	 * also the hardest to read. */
	yxml_ret_t r = YXML_OK;
	unsigned int ch = (unsigned int)ich;
	unsigned int tmp;
	char *buf;
	int namelen;
	void *tobefreed = NULL;
#ifdef YXML_PROFILE
	x->steps++;
#endif

	if(ch == '\n') {
		x->line++;
		x->byte = 0;
	} else
		x->byte++;
	x->total++;


#define YXML_SWITCH(s) \
	x->state = s; \
	goto s
#define YXML_GOTO(s) goto s
#define YXML_CASE(s) s: \
	if(tobefreed) { \
		yxml_pop_name(x); \
		tobefreed = NULL; \
	}
#define YXML_RETURN(val) return val
#define YXML_CHAR(ch) (ch < 'a' || ch > 'z') && (ch < 'A' || ch > 'Z') && (ch < '0' || ch > '9') && ch != '_' && ch != ':' && ch != '-' && ch != '.'

	/* A note on yxml_push():
	 * Whenever we push something to the stack that may still be needed in a later
	 * state, we have to call yxml_push(), check for YXML_ESTACK, and return it
	 * if the call failed. If the pushed value is only temporary, we can assume
	 * the buffer is large enough and omit the check. In practice, this means
	 * that we only have to check when pushing characters of names. */

	switch(x->state) {
	case YXMLS_init:
		/* UTF-8 BOM. This is the only place where we might see raw UTF8 characters.
		 * After this, we're in ASCII mode until we see an encoding specified. */
		if(x->total == 1 && ch == 0xEF)
			x->utf8 = 2;
		else if(x->total == 2 && x->utf8 == 2 && ch == 0xBB)
			x->utf8 = 1;
		else if(x->total == 3 && x->utf8 == 1 && ch == 0xBF)
			x->utf8 = 0;
		else if(ch == '<')
			YXML_GOTO(YXMLS_le0);
		else if(ch > ' ')
			return YXML_ESYN;
		break;

	YXML_CASE(YXMLS_misc)
		if(ch == '<')
			YXML_GOTO(YXMLS_le0);
		if(ch > ' ')
			return YXML_ESYN;
		break;

	YXML_CASE(YXMLS_le0)
		if(ch == '/')
			YXML_GOTO(YXMLS_etag0);
		if(ch == '?')
			YXML_GOTO(YXMLS_pi0);
		if(ch == '!')
			YXML_GOTO(YXMLS_le1);
		if(YXML_CHAR(ch))
			return YXML_ESYN;
		if(yxml_push(x, ch))
			return YXML_ESTACK;
		YXML_SWITCH(YXMLS_elem0);

	YXML_CASE(YXMLS_le1)
		if(ch == '-')
			YXML_GOTO(YXMLS_comment0);
		if(ch == '[')
			YXML_GOTO(YXMLS_cd0);
		if(ch > ' ') /* DTD */
			YXML_GOTO(YXMLS_dt0);
		break;

	YXML_CASE(YXMLS_dt0)
		if(ch == '>')
			YXML_GOTO(YXMLS_misc);
		break;

	YXML_CASE(YXMLS_cd0)
		if(ch == 'C' && yxml_peek(x) == '[')
			yxml_push(x, 'C');
		else if(ch == 'D' && yxml_peek(x) == 'C')
			yxml_push(x, 'D');
		else if(ch == 'A' && yxml_peek(x) == 'D')
			yxml_push(x, 'A');
		else if(ch == 'T' && yxml_peek(x) == 'A')
			yxml_push(x, 'T');
		else if(ch == 'A' && yxml_peek(x) == 'T')
			yxml_push(x, 'A');
		else if(ch == '[' && yxml_peek(x) == 'A')
			YXML_GOTO(YXMLS_cd1);
		else
			YXML_GOTO(YXMLS_dt0);
		break;

	YXML_CASE(YXMLS_cd1)
		if(ch == ']')
			YXML_GOTO(YXMLS_cd2);
		else
			YXML_RETURN(yxml_datacontent(x, ch));

	YXML_CASE(YXMLS_cd2)
		if(ch == ']')
			YXML_GOTO(YXMLS_le2);
		else {
			r = yxml_datacontent(x, ']');
			if(r) return r;
			YXML_GOTO(YXMLS_cd1);
		}

	YXML_CASE(YXMLS_le2)
		if(ch == '>') {
			/* Wait, this isn't a le2 state at all... it's a "I've just seen
			 * ']]', now I need a '>' to make a valid CDATA section" state. */
			yxml_pop(x); yxml_pop(x); yxml_pop(x); yxml_pop(x); yxml_pop(x); yxml_pop(x);
			YXML_GOTO(YXMLS_string);
		}
		r = yxml_datacontent(x, ']');
		if(r) return r;
		r = yxml_datacontent(x, ']');
		if(r) return r;
		YXML_GOTO(YXMLS_cd1);

	YXML_CASE(YXMLS_comment0)
		if(ch == '-')
			YXML_GOTO(YXMLS_comment1);
		else
			return YXML_ESYN;

	YXML_CASE(YXMLS_comment1)
		if(ch == '-')
			YXML_GOTO(YXMLS_comment2);
		break;

	YXML_CASE(YXMLS_comment2)
		if(ch == '-')
			YXML_RETURN(YXML_ESYN);
		if(ch == '>')
			YXML_GOTO(YXMLS_misc);
		YXML_GOTO(YXMLS_comment1);

	YXML_CASE(YXMLS_pi0)
		if(YXML_CHAR(ch))
			return YXML_ESYN;
		if(yxml_push(x, ch))
			return YXML_ESTACK;
		YXML_SWITCH(YXMLS_pi1);

	YXML_CASE(YXMLS_pi1)
		if(ch <= ' ') {
			yxml_push(x, 0);
			x->pi = (char *)x->stack;
			/* Is this a XML prolog? */
			if(x->stacklen == 4 && !memcmp(x->pi, "xml", 4)) {
				yxml_pop_name(x);
				YXML_GOTO(YXMLS_xml0);
			}
			YXML_GOTO(YXMLS_pi2);
		}
		if(YXML_CHAR(ch))
			return YXML_ESYN;
		if(yxml_push(x, ch))
			return YXML_ESTACK;
		break;

	YXML_CASE(YXMLS_xml0)
		if(ch > ' ' && ch != 'v')
			return YXML_ESYN;
		if(ch == 'v')
			YXML_GOTO(YXMLS_ver0);
		break;

	YXML_CASE(YXMLS_ver0)
		if(ch > ' ' && ch != 'e')
			YXML_GOTO(YXMLS_ver1);
		if(ch == 'e')
			YXML_GOTO(YXMLS_enc0);
		break;

	YXML_CASE(YXMLS_ver1)
		if(ch == '=')
			YXML_GOTO(YXMLS_ver2);
		if(ch > ' ')
			return YXML_ESYN;
		break;

	YXML_CASE(YXMLS_ver2)
		if(ch == '"' || ch == '\'')
			x->quote = ch;
		else if(ch > ' ')
			return YXML_ESYN;
		break;
	/* TODO: Parse version */

	YXML_CASE(YXMLS_enc0)
		if(ch > ' ' && ch != 's')
			YXML_GOTO(YXMLS_ver1);
		if(ch == 's')
			YXML_GOTO(YXMLS_std0);
		break;
	/* TODO: Parse encoding */

	YXML_CASE(YXMLS_std0)
		if(ch == '>')
			YXML_GOTO(YXMLS_misc);
		break;

	YXML_CASE(YXMLS_pi2)
		r = YXML_PISTART;
		YXML_GOTO(YXMLS_pi3);

	YXML_CASE(YXMLS_pi3)
		if(r == YXML_PISTART) {
			if(ch == '?')
				yxml_push(x, '?');
			else if(ch > ' ') {
				tobefreed = x->pi;
				YXML_RETURN(r);
			}
		} else {
			if(ch == '?') {
				if(yxml_peek(x) != '?')
					yxml_push(x, '?');
			} else if(ch == '>') {
				if(yxml_peek(x) == '?') {
					yxml_pop(x);
					YXML_GOTO(YXMLS_misc);
					YXML_RETURN(YXML_PIEND);
				} else {
					tobefreed = x->pi;
					YXML_RETURN(yxml_datapi(x, ch));
				}
			} else if(yxml_peek(x) == '?') {
				r = yxml_datapi(x, '?');
				yxml_pop(x);
				if(r) {
					tobefreed = x->pi;
					return r;
				}
				tobefreed = x->pi;
				YXML_RETURN(yxml_datapi(x, ch));
			} else {
				tobefreed = x->pi;
				YXML_RETURN(yxml_datapi(x, ch));
			}
		}
		break;

	YXML_CASE(YXMLS_elem0)
		if(ch <= ' ') {
			yxml_push(x, 0);
			x->elem = (char *)x->stack;
			YXML_GOTO(YXMLS_elem1);
		} else if(ch == '/') {
			yxml_push(x, 0);
			x->elem = (char *)x->stack;
			YXML_GOTO(YXMLS_elem2);
		} else if(ch == '>') {
			yxml_push(x, 0);
			x->elem = (char *)x->stack;
			YXML_GOTO(YXMLS_string);
		} else if(YXML_CHAR(ch))
			return YXML_ESYN;
		if(yxml_push(x, ch))
			return YXML_ESTACK;
		break;

	YXML_CASE(YXMLS_elem1)
		r = YXML_ELEMSTART;
		YXML_GOTO(YXMLS_attr0);

	YXML_CASE(YXMLS_attr0)
		if(r == YXML_ELEMSTART) {
			if(ch == '/') {
				tobefreed = x->elem;
				YXML_GOTO(YXMLS_elem2);
				return r;
			} else if(ch == '>') {
				tobefreed = x->elem;
				YXML_GOTO(YXMLS_string);
				return r;
			} else if(ch > ' ' && YXML_CHAR(ch)) {
				yxml_pop_name(x);
				return YXML_ESYN;
			} else if(ch > ' ') {
				if(yxml_push(x, ch))
					return YXML_ESTACK;
				YXML_GOTO(YXMLS_attr1);
				return r;
			} else {
				tobefreed = x->elem;
				return r;
			}
		} else {
			if(ch == '/')
				YXML_GOTO(YXMLS_elem2);
			else if(ch == '>')
				YXML_GOTO(YXMLS_string);
			else if(ch > ' ') {
				if(yxml_push(x, ch))
					return YXML_ESTACK;
				YXML_GOTO(YXMLS_attr1);
			}
		}
		break;

	YXML_CASE(YXMLS_attr1)
		if(ch == '=') {
			yxml_push(x, 0);
			x->attr = (char *)x->stack+yxml_symlen(x, x->elem)+1;
			YXML_GOTO(YXMLS_attr2);
		} else if(ch <= ' ') {
			yxml_push(x, 0);
			x->attr = (char *)x->stack+yxml_symlen(x, x->elem)+1;
			YXML_GOTO(YXMLS_attr3);
		} else if(YXML_CHAR(ch)) {
			yxml_pop_name(x);
			return YXML_ESYN;
		}
		if(yxml_push(x, ch))
			return YXML_ESTACK;
		break;

	YXML_CASE(YXMLS_attr3)
		if(ch == '=')
			YXML_GOTO(YXMLS_attr2);
		else if(ch > ' ') {
			yxml_pop_name(x);
			return YXML_ESYN;
		}
		break;

	YXML_CASE(YXMLS_attr2)
		if(r == YXML_ATTRSTART) {
			if(ch == '"' || ch == '\'') {
				x->quote = ch;
				return r;
			} else if(ch > ' ') {
				yxml_pop_name(x);
				return YXML_ESYN;
			} else
				return r;
		} else if(x->reflen) {
			r = yxml_ref(x, ch);
			if(r < YXML_OK)
				return r;
			if(r > YXML_OK)
				YXML_RETURN(r);
		} else if(ch == '&') {
			x->reflen = 1;
			yxml_push(x, 0);
		} else if(ch == x->quote) {
			x->quote = 0;
			yxml_pop_name(x);
			YXML_GOTO(YXMLS_elem1);
			YXML_RETURN(YXML_ATTREND);
		} else
			YXML_RETURN(yxml_dataattr(x, ch));
		break;

	YXML_CASE(YXMLS_elem2)
		if(ch == '>') {
			r = yxml_push_name(x, x->elem, yxml_symlen(x, x->elem));
			if(r) return r;
			YXML_GOTO(YXMLS_elem3);
		}
		yxml_pop_name(x);
		return YXML_ESYN;

	YXML_CASE(YXMLS_elem3)
		if(r == YXML_ELEMSTART) {
			YXML_RETURN(r);
		} else {
			yxml_pop_name(x);
			YXML_RETURN(YXML_ELEMEND);
		}

	YXML_CASE(YXMLS_string)
		if(r == YXML_ELEMSTART)
			YXML_RETURN(r);
		if(x->reflen) {
			r = yxml_ref(x, ch);
			if(r < YXML_OK)
				return r;
			if(r > YXML_OK)
				YXML_RETURN(r);
		} else if(ch == '<')
			YXML_GOTO(YXMLS_le0);
		else if(ch == '&') {
			x->reflen = 1;
			yxml_push(x, 0);
		} else
			YXML_RETURN(yxml_datacontent(x, ch));
		break;

	YXML_CASE(YXMLS_etag0)
		if(YXML_CHAR(ch)) {
			yxml_pop_name(x);
			return YXML_ESYN;
		}
		if(yxml_push(x, ch))
			return YXML_ESTACK;
		YXML_SWITCH(YXMLS_etag1);

	YXML_CASE(YXMLS_etag1)
		if(ch == '>') {
			yxml_push(x, 0);
			buf = (char *)x->stack;
			namelen = x->stacklen-1;
			yxml_pop_name(x);
			/* Compare tag names */
			if(yxml_symlen(x, x->elem) != namelen || memcmp(x->elem, buf, namelen))
				return YXML_ECLOSE;
			yxml_pop_name(x);
			YXML_GOTO(YXMLS_misc);
			YXML_RETURN(YXML_ELEMEND);
		} else if(YXML_CHAR(ch)) {
			yxml_pop_name(x);
			return YXML_ESYN;
		}
		if(yxml_push(x, ch))
			return YXML_ESTACK;
		break;

	YXML_CASE(YXMLS_etag2)
		if(ch == '>') {
			/* Compare tag names */
			if(memcmp(x->elem, (char *)x->stack+x->stacklen, strlen(x->elem)))
				return YXML_ECLOSE;
			yxml_pop_name(x);
			yxml_pop_name(x);
			YXML_GOTO(YXMLS_misc);
			YXML_RETURN(YXML_ELEMEND);
		} else if(ch > ' ') {
			yxml_pop_name(x);
			yxml_pop_name(x);
			return YXML_ESYN;
		}
		break;
	}

	return YXML_OK;
}


yxml_ret_t yxml_eof(yxml_t *x) {
	if(x->state != YXMLS_misc)
		return YXML_EEOF;
	return YXML_OK;
}
