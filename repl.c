#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <editline/readline.h>
#include "parse.c"
enum err_code {
	UNEXP_TOKEN, INCOMPL_EXP, TRAIL_CHARS
};

void printerr(unsigned pad, enum err_code err)
{
	static const char arrow[] = "^--";
	static const char *err_msgs[] = {
		"Unexpected token",
		"Incomplete expression",
		"Trailing characters"
	};
	while (pad--)
		putchar(' ');
	printf("  %s %s\n", arrow, err_msgs[err]);
}

Cell *parse(const char *s)
{
	Cell *cp;
	unsigned i = 0;

	if (cp = parse_sexp(s, &i)) {
		if (s[i] != '\0') {
			printerr(i, TRAIL_CHARS);
			return NULL;
		}
	} else {
		if (s[i] == '\0')
			printerr(i, INCOMPL_EXP);
		else
			printerr(i, UNEXP_TOKEN);
		return NULL;
	}
	return cp;
}

int main()
{
	char *s;
	Cell *cp;
	initkeys(keysyms);
	while ((s = readline("> "))) {		// -Wparen
		if (*s != '\0')
			cp = parse(s);
		if (cp) {
			print(apply(car(cp), cdr(cp)));
			putchar('\n');
		}
		add_history(s);
		free(s);
	}
	return 0;
}
