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
	putchar(LB);
	for (root = cp; !is_null(cp); cp = cdr(cp)) {
		if (printatm(cp, true))
			break;
		if (cp != root)
			putchar(LS);
		print(car(cp));
	}
	putchar(RB);
}

int unmatch_lb(char s[])
{
	char c;
	int i = 0;
	while ((c = *s++)) {		// -Wparen
		if (c == LB)
			++i;
		else if (c == RB)
			--i;
	}
	return i;
}

char *trim(char s[])
{
	char c, *r, *t;
	char lc = '^';			// for #if below
	r = t = s;
	while ((c = *s++))		// -Wparen
		if (!isspace(c))
			*t++ = lc = c;
#if LS == ' '
		else {
			while (isspace(*s))
				++s;
			if (lc != '^' && lc != LB && *s && *s != RB)
				*t++ = ' ';
		}
#endif
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
				n += strlen(t[i]) + 1;
			s = malloc(n + 1);
			for (*s = '\0', i = 0; i < j; ++i) {
				strcat(s, t[i]);
				strcat(s, " ");
				free(t[i]);
			}
		}
		cp = read(trim(s));
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
