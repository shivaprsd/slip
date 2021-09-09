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

Atom *new_atom(char c)
{
	Atom *ap;
	ap = (Atom *) malloc((size_t) sizeof(Atom));
	if (!ap)
		fprintf(stderr, "error: memory allocation failed\n");
	else
		ap->sym = c;
	return ap;
}
Cell *new_cell(Atom *ap)
{
	Cell *cp;
	cp = (Cell *) malloc((size_t) sizeof(Cell));
	if (!cp) {
		fprintf(stderr, "error: memory allocation failed\n");
	} else {
		cp->atm = ap;
		cp->car = cp->cdr = NULL;
	}
	return cp;
}

bool is_atom(Cell *x)
{
	if (!x) {
		fputs("fatal: is_atom() received NULL pointer arg\n", stderr);
		exit(1);
	}
	return x->atm != NULL;
}
bool is_eq(Cell *x, Cell *y)
{
	if (!is_atom(x) || !is_atom(y)) {
		puts("warning: is_eq() on non-atoms is undefined");
		return false;
	}
	return x->atm == y->atm;
}
Cell *car(Cell *x)
{
	if (is_atom(x)) {
		puts("warning: car() on atom is undefined");
		return NULL;
	}
	return x->car;
}
Cell *cdr(Cell *x)
{
	if (is_atom(x)) {
		puts("warning: cdr() on atom is undefined");
		return NULL;
	}
	return x->cdr;
}
Cell *cons(Cell *x, Cell *y)
{
	Cell *e = new_cell(NULL);
	if (e) {
		e->car = x;
		e->cdr = y;
	}
	return e;
}

bool is_null(Cell *x)
{
	return is_atom(x) && is_eq(x, &NIL);
}
