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

#ifndef YXML_H
#define YXML_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	YXML_OK = 0,
	YXML_ELEMSTART,
	YXML_ELEMEND,
	YXML_ATTRSTART,
	YXML_ATTRVAL,
	YXML_ATTREND,
	YXML_CONTENT,
	YXML_PISTART,
	YXML_PICONTENT,
	YXML_PIEND,

	YXML_EREF = -1,
	YXML_ECLOSE = -2,
	YXML_ESTACK = -3,
	YXML_ESYN = -4,
	YXML_EEOF = -5
} yxml_ret_t;


typedef struct {
	/* PUBLIC. You may read these fields, but don't change them. */
	char *elem;           /* Name of the currently open element */
	char *attr;           /* Name of the currently open attribute */
	char *pi;             /* Target of the currently open processing instruction */
	char data[8];         /* Content/attribute/pi value */
	uint64_t line;        /* Line number in the XML document */
	uint64_t byte;        /* Byte offset in the current line */
	uint64_t total;       /* Total bytes consumed from the document */

	/* PRIVATE. Don't touch. */
	int state;
	unsigned char *stack;
	size_t stacksize;
	size_t stacklen;
	unsigned int quote;
	int reflen;
	int utf8;
} yxml_t;


/* Initialize a yxml_t structure.
 *
 * The `stack` is a writable buffer where yxml will keep track of open
 * elements and attributes. The size of this buffer determines the maximum
 * nesting depth and the maximum length of element/attribute names. A buffer of
 * 1024 bytes should be more than enough for most applications.
 *
 * A single yxml_t struct can be reused to parse multiple documents, but only
 * after re-initializing it with yxml_init().
 */
void yxml_init(yxml_t *x, void *stack, size_t stacksize);


/* Parse a single character.
 *
 * Returns a yxml_ret_t value. On success, this will be either YXML_OK or one
 * of the token values (>0). On error, a negative value is returned. See the
 * yxml_ret_t definition for details.
 *
 * Once yxml_parse() has returned an error, it must not be called again on the
 * same yxml_t struct.
 */
yxml_ret_t yxml_parse(yxml_t *x, int ch);


/* Inform yxml that the document has ended.
 *
 * This function should be called after the application has provided all bytes
 * to yxml_parse(). It will check for some final well-formedness errors.
 *
 * Returns YXML_OK on success, or a negative value on error. Once this
 * function returns an error, it must not be called again on the same yxml_t
 * struct.
 */
yxml_ret_t yxml_eof(yxml_t *x);


/* A utility function to get the length of a symbol.
 *
 * Returns the same as strlen(s), except that the length is already known by
 * yxml. This function should ONLY be called directly after yxml_parse() has
- * returned YXML_ELEMSTART, YXML_ATTRSTART or YXML_PISTART, and may ONLY be
+ * returned YXML_ELEMSTART, YXML_ATTRSTART or YXML_PISTART, and may ONLY be
 * used on x->elem, x->attr or x->pi, respectively. The string passed to this
 * function MUST be one of these three pointers.
 */
size_t yxml_symlen(yxml_t *x, const char *s);


#ifdef __cplusplus
}
#endif

#endif
