#include "store.c"

#define MAX_LINES 100

char *get_input(FILE *fp, const char *pmt)
{
	char *t;
	size_t n;

	if (fp == stdin)
		return readline(pmt);
	t = NULL;
	if (getline(&t, &n, fp) == -1)
		return NULL;
	return t;
}

char *read_input(FILE *fp)
{
	int n, i;
	char *s, *lines[MAX_LINES];
	const char *pmt = "> ";

	n = i = 0;
	while (i < MAX_LINES && (s = get_input(fp, pmt))) {
		lines[i++] = trim(s);
		pmt = ".. ";
		if ((n += oc_diff(s)) <= 0)
			break;
	}
	if ((s || fp != stdin) && i > 0)
		s = concat(lines, i);
	if (!s || i > 1) {
		while (--i >= 0)
			free(lines[i]);
	}
	return s;
}

bool printatm(Cell *cp, bool dot)
{
	if (!is_atom(cp))
		return false;
	putchar(dot ? CELL_SEP : 0);
	fputs(cp->atm->sym, stdout);
	return true;
}

void printexp(Cell *cp)
{
	if (!cp || printatm(cp, false))
		return;
	putchar(CELL_BEG);
	printexp(cp->car);
	putchar(CELL_SEP);
	printexp(cp->cdr);
	putchar(CELL_END);
}

void print(Cell *cp)
{
	Cell *root;
	if (!cp || printatm(cp, false))
		return;
	putchar(LIST_BEG);
	for (root = cp; !is_null(cp); cp = cdr(cp)) {
		if (printatm(cp, true))
			break;
		if (cp != root)
			putchar(LIST_SEP);
		print(car(cp));
	}
	putchar(LIST_END);
}
