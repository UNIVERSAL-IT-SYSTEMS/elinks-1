#ifndef EL_DOM_STRING_H
#define EL_DOM_STRING_H

#include "util/memory.h"

struct dom_string {
	size_t length;
	unsigned char *string;
};

#define INIT_DOM_STRING(strvalue, strlength) \
	{ (strlength) == -1 ? sizeof(strvalue) - 1 : (strlength), (strvalue) }

static inline void
set_dom_string(struct dom_string *string, unsigned char *value, size_t length)
{
	string->string = value;
	string->length = length == -1 ? strlen(value) : length;
}

static inline int
dom_string_casecmp(const struct dom_string *string1, const struct dom_string *string2)
{
	size_t length = int_min(string1->length, string2->length);
	size_t string_diff = strncasecmp(string1->string, string2->string, length);

	/* If the lengths or strings don't match strncasecmp() does the
	 * job else return which ever is bigger. */
	return string_diff ? string_diff : string1->length - string2->length;
}

static inline int
dom_string_ncasecmp(struct dom_string *string1, struct dom_string *string2, size_t length)
{
	return strncasecmp(string1->string, string2->string, length);
}

#define copy_dom_string(string1, string2) \
	set_dom_string(string1, (string2)->string, (string2)->length)

static inline struct dom_string *
add_to_dom_string(struct dom_string *string, unsigned char *str, size_t len)
{
	unsigned char *newstring;

	newstring = mem_realloc(string->string, string->length + len + 1);
	if (!newstring)
		return NULL;

	string->string = newstring;
	memcpy(string->string + string->length, str, len);
	string->length += len;
	string->string[string->length] = 0;

	return string;
}

#define init_dom_string(string, str, len) add_to_dom_string(string, str, len)

#define is_dom_string_set(str) ((str)->string && (str)->length)

#define done_dom_string(str) \
	do { mem_free_set(&(str)->string, NULL); (str)->length = 0; } while (0)

#define isquote(c)	((c) == '"' || (c) == '\'')

#endif