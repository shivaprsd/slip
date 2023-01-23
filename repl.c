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

int initenv(Cell **env, const char *fname)
{
	FILE *fp;
	char *s;
	int err = 0;
	Cell *cp = NULL;

	*env = nil;
	if ((fp = fopen(fname, "r")) == NULL)
		err = 1;
	while (!err && (s = read_input(fp))) {
		if (*s && !(cp = parse(s)))
			err = 2;
		free(s);
		if (cp) {
			eval(cp, *env);
			cp = NULL;
		}
	}
	return err;
}

const char *greeting = "SLIP: A Small LIst Processor\n";
const char *initfile = "init.lisp";

int main()
{
	int err;
	char *s;
	Cell *cp;

	initkeys(keysyms);
	initops(numops);
	if (err = initenv(&envlist, initfile)) {
		fprintf(stderr, "Unable to load %s\n", initfile);
		return err;
	}
	puts(greeting);
	while ((s = read_input(stdin))) {	// -Wparen
		if (*s && (cp = parse(s))) {
			print(eval(cp, envlist));
			putchar('\n');
		}
		add_history(s);
		free(s);
	}
	return 0;
}
