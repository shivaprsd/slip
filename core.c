#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
typedef struct {
	char sym;
} Atom;
typedef struct cell {
	Atom *atm;
	struct cell *car;
	struct cell *cdr;
} Cell;

Atom _NIL = { '%' };
Cell NIL = { &_NIL, NULL, NULL };
Cell *new_cell(Atom *ap)
{
	Cell *cp;
	cp = (Cell *) malloc((size_t) sizeof(Cell));
	if (!cp)
		fprintf(stderr, "error: memory allocation failed\n");
	else
		cp->atm = ap;
	return cp;
}

bool is_atom(Cell *cp)
{
	return cp && cp->atm;
}
bool is_eq(Cell *c1, Cell *c2)
{
	return is_atom(c1) && is_atom(c2) && (c1->atm == c2->atm);
}
Cell *car(Cell *cp)
{
	return is_atom(cp) ? NULL : cp->car;
}
Cell *cdr(Cell *cp)
{
	return is_atom(cp) ? NULL : cp->cdr;
}
Cell *cons(Cell *c1, Cell *c2)
{
	Cell *cp;
	cp = new_cell(NULL);
	if (cp) {
		cp->car = c1;
		cp->cdr = c2;
	}
	return cp;
}

void prnexp(Cell *cp)
{
	if (!cp)
		return;
	if (is_atom(cp)) {
		putchar(cp->atm->sym);
		return;
	}
	putchar('[');
	prnexp(cp->car);
	putchar('.');
	prnexp(cp->cdr);
	putchar(']');
}
