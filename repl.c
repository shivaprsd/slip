#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <editline/readline.h>
#include "io.c"
enum err_code {
	UNEXP_TOKEN, INCOMPL_EXP,
	TRAIL_CHARS, EMPTY_STR
};

void printerr(char s[], unsigned pad, enum err_code err)
{
	static const char arrow[] = "^--";
	static const char *err_msgs[] = {
		"Unexpected token",
		"Incomplete expression",
		"Trailing characters"
	};
	if (err != EMPTY_STR) {
		puts(s);
		while (pad--)
			putchar(' ');
		printf("%s %s\n", arrow, err_msgs[err]);
	}
}

Cell *read(char s[])
{
	Cell *cp = NULL;
	int slen, rlen;
	enum err_code err;
	slen = strlen(s);
	if ((rlen = readlist(&cp, s)) < 0) {
		rlen = -rlen;
		err = (rlen == slen) ? INCOMPL_EXP : UNEXP_TOKEN;
	} else if (rlen == 0) {
		err = slen ? UNEXP_TOKEN : EMPTY_STR;
	} else if (rlen < slen) {
		err = TRAIL_CHARS;
	} else {
		return cp;
	}
	printerr(s, rlen, err);
	return NULL;
}

void print(Cell *cp)
{
	Cell *root;
	if (!cp || printatm(cp, false))
		return;
	putchar('[');
	for (root = cp; !is_null(cp); cp = cdr(cp)) {
		if (printatm(cp, true))
			break;
		if (cp != root)
			putchar(',');
		print(car(cp));
	}
	putchar(']');
}

int unmatch_lb(char s[])
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

int main()
{
	int i, j, n;
	char *s, *t[100];
	const char *pmt = "> ";
	Cell *cp;
	initkeys(keysyms);
	i = j = 0;
	while ((s = readline(pmt))) {		// -Wparen
		t[j++] = s;
		if ((i += unmatch_lb(s)) > 0) {
			pmt = ".. ";
			continue;
		}
		if (j > 1) {
			for (n = i = 0; i < j; ++i)
				n += strlen(trim(t[i]));
			s = malloc(n + 1);
			for (*s = '\0', i = 0; i < j; ++i) {
				strcat(s, t[i]);
				free(t[i]);
			}
		} else {
			trim(s);
		}
		cp = read(s);
		if (cp) {
			print(apply(car(cp), cdr(cp)));
			putchar('\n');
		}
		add_history(s);
		free(s);
		cp = NULL;
		i = j = 0;
		pmt = "> ";
	}
	return 0;
}
