#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <editline/readline.h>
#include "io.c"
enum err_code {
	UNEXP_TOKEN, INCOMPL_EXP,
	TRAIL_CHARS, EMPTY_STR
};

void printerr(unsigned pad, enum err_code err)
{
	static const char arrow[] = "^--";
	static const char *err_msgs[] = {
		"Unexpected token",
		"Incomplete expression",
		"Trailing characters"
	};
	if (err != EMPTY_STR) {
		while (pad--)
			putchar(' ');
		printf("  %s %s\n", arrow, err_msgs[err]);
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
	printerr(rlen, err);
	return NULL;
}

void print(Cell *cp)
{
	Cell *root;
	if (!cp)
		return;
	if (is_atom(cp)) {
		printexp(cp);
		return;
	}
	putchar('[');
	for (root = cp; cp != NULL; cp = cdr(cp)) {
		if (is_atom(cp) && !is_eq(cp, &NIL)) {
			putchar('.');
			printexp(cp);
		}
		if (root != cp && cdr(cp))
			putchar(',');
		print(car(cp));
	}
	putchar(']');
}

int main()
{
	char *s;
	Cell *cp;
	while ((s = readline("> "))) {		// -Wparen
		print(cp = read(s));
		if (cp)
			putchar('\n');
		add_history(s);
		free(s);
		cp = NULL;
	}
	return 0;
}
