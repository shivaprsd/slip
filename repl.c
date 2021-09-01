#include <stdio.h>
#include <stdlib.h>
#include <editline/readline.h>
#include "parse.c"
enum err_code {
	UNEXP_TOKEN, INCOMPL_EXP, TRAIL_CHARS
};

void printerr(const char *s, unsigned pad, enum err_code err)
{
	static const char arrow[] = "^--";
	static const char *err_msgs[] = {
		"Unexpected token",
		"Incomplete expression",
		"Trailing characters"
	};
	puts(s);
	while (pad--)
		putchar(' ');
	printf("%s %s\n", arrow, err_msgs[err]);
}

Cell *parse(const char *s)
{
	Cell *cp;
	unsigned i = 0;

	if (cp = parse_sexp(s, &i)) {
		if (s[i] != '\0') {
			printerr(s, i, TRAIL_CHARS);
			return NULL;
		}
	} else {
		if (s[i] == '\0')
			printerr(s, i, INCOMPL_EXP);
		else
			printerr(s, i, UNEXP_TOKEN);
		return NULL;
	}
	return cp;
}
int main()
{
	char *s;
	Cell *cp;

	initkeys(keysyms);
	while ((s = read_input())) {		// -Wparen
		if (*s && (cp = parse(s))) {
			print(apply(car(cp), cdr(cp)));
			putchar('\n');
		}
		add_history(s);
		free(s);
	}
	return 0;
}
