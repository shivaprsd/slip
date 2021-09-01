#include "store.c"
#include "util.c"

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
	putchar(dot ? '.' : 0);
	putchar(cp->atm->sym);
	return true;
}

void printexp(Cell *cp)
{
	if (!cp || printatm(cp, false))
		return;
	putchar('[');
	printexp(cp->car);
	putchar('.');
	printexp(cp->cdr);
	putchar(']');
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
