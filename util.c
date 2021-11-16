#include <stdlib.h>
#include <string.h>

char *dupstr(const char *s)
{
	size_t n = strlen(s) + 1;
	char *t = malloc(n);
	if (t)
		memcpy(t, s, n);
	return t;
}

char *trim(char s[])
{
	char c, *r, *t;
	char lc = ' ';
	r = t = s;
	while ((c = *s++) && c != CMNT_MAC) {
		if (isspace(c))
			c = ' ';
		if (c != ' ' || lc != ' ')
			*t++ = lc = c;
	}
	if (lc == ' ' && t > r)
		--t;
	*t = '\0';
	return r;
}

char *copystr(char *s, char *t, char c)
{
	while ((*s = *t++))		// -Wparen
		++s;
	if (c) {
		*s++ = c;
		*s = '\0';
	}
	return s;
}

char *concat(char *lines[], unsigned n)
{
	char *s, *t;
	int len, i;
	if (n == 1)
		return lines[0];
	for (len = i = 0; i < n; ++i)
		if (*lines[i])
			len += strlen(lines[i]) + 1;
	s = malloc(len + 1);
	if (!s)
		return NULL;
	for (t = s, i = 0; i < n; ++i)
		if (*lines[i])
			t = copystr(t, lines[i], ' ');
	*--t = '\0';
	return s;
}

int oc_diff(const char s[])
{
	char c;
	int i = 0;
	while ((c = *s++)) {		// -Wparen
		if (c == LIST_BEG)
			++i;
		else if (c == LIST_END)
			--i;
	}
	return i;
}
