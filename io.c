#include "store.c"

char *read_input()
{
	int n, i;
	char *s, *lines[100];
	const char *pmt = "> ";

	n = i = 0;
	while ((s = readline(pmt))) {		// -Wparen
		lines[i++] = trim(s);
		pmt = ".. ";
		if ((n += oc_diff(s)) <= 0) {
			s = concat(lines, i);
			break;
		}
	}
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
