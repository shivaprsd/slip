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
	r = t = s;
	while ((c = *s++))		// -Wparen
		if (!isspace(c))
			*t++ = c;
	*t = '\0';
	return r;
}

char *copystr(char *s, char *t)
{
	while ((*s = *t++))		// -Wparen
		++s;
	return s;
}

char *concat(char *lines[], unsigned n)
{
	char *s, *t;
	int len, i;
	if (n == 1)
		return lines[0];
	for (len = i = 0; i < n; ++i)
		len += strlen(lines[i]);
	s = malloc(len + 1);
	if (s)
		for (t = s, i = 0; i < n; ++i)
			t = copystr(t, lines[i]);
	return s;
}

int oc_diff(const char s[])
{
	char c;
	int i = 0;
	while ((c = *s++)) {		// -Wparen
		if (c == '[')
			++i;
		else if (c == ']')
			--i;
	}
	return i;
}
