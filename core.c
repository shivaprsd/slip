#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
typedef enum {
	NOKEY = -1, NIL, T,
	ATOM, EQ, CAR, CDR, CONS,
	QUOTE, COND, LABEL, LAMBDA
} Keywrd;
typedef struct {
	char sym;
	Keywrd key;
} Atom;
typedef struct cell {
	const Atom *atm;
	struct cell *car;
	struct cell *cdr;
} Cell;

const char keysyms[] = {
	/*none,*/ '%', 'T',
	'A', 'E', 'F', 'R', 'C',
	'Q', 'I', 'L', 'Y', '\0'
};
extern Cell *nil, *tru, *quot;

Atom *new_atom(char c, Keywrd k)
{
	Atom *ap;
	ap = (Atom *) malloc((size_t) sizeof(Atom));
	if (!ap) {
		fprintf(stderr, "error: memory allocation failed\n");
	} else {
		ap->sym = c;
		ap->key = k;
	}
	return ap;
}
Cell *new_cell(const Atom *ap)
{
	Cell *cp = (Cell *) malloc((size_t) sizeof(Cell));
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
	return is_atom(x) && is_eq(x, nil);
}
#define is_true(x) !is_null(x)
#define prop(b) (b ? tru : nil)

#define caar(x) car(car(x))
#define cadr(x) car(cdr(x))
#define cadar(x) cadr(car(x))
#define caddr(x) cadr(cdr(x))
#define caddar(x) caddr(car(x))
#define list(x, y) cons(x, cons(y, nil))

Cell *append(Cell *x, Cell *y)
{
	return is_null(x) ? y : cons(car(x), append(cdr(x), y));
}
Cell *pair(Cell *x, Cell *y)
{
	if (is_null(x) && is_null(y))
		return nil;
	else if (!is_atom(x) && !is_atom(y))
		return cons(list(car(x), car(y)), pair(cdr(x), cdr(y)));
	return NULL;
}
Cell *assoc(Cell *x, Cell *y)
{
	return is_eq(caar(y), x) ? cadar(y) : assoc(x, cdr(y));
}

Cell *eval(Cell *, Cell *);
Cell *evcon(Cell *c, Cell *a)
{
	return is_true(eval(caar(c), a)) ? eval(cadar(c), a) : evcon(cdr(c), a);
}
Cell *evlis(Cell *m, Cell *a)
{
	return is_null(m) ? nil : cons(eval(car(m), a), evlis(cdr(m), a));
}
Cell *eval(Cell *e, Cell *a)
{
	Cell *cp;
	if (is_atom(e))
		return assoc(e, a);
	if (is_atom(cp = car(e))) {
		switch (cp->atm->key) {
		case QUOTE:
			return cadr(e);
		case ATOM:
			return prop(is_atom(eval(cadr(e), a)));
		case EQ:
			return prop(is_eq(eval(cadr(e), a), eval(caddr(e), a)));
		case COND:
			return evcon(cdr(e), a);
		case CAR:
			return car(eval(cadr(e), a));
		case CDR:
			return cdr(eval(cadr(e), a));
		case CONS:
			return cons(eval(cadr(e), a), eval(caddr(e), a));
		default:
			return eval(cons(assoc(car(e), a), cdr(e)), a);
		}
	}
	if (is_atom(cp = caar(e))) {
		switch (cp->atm->key) {
		case LABEL:
			return eval(cons(caddar(e), cdr(e)),
				cons(list(cadar(e), car(e)), a));
		case LAMBDA:
			return eval(caddar(e),
				append(pair(cadar(e), evlis(cdr(e), a)), a));
		default:
			break;
		}
	}
	return NULL;
}
Cell *appq(Cell *m)
{
	return is_null(m) ? nil : cons(list(quot, car(m)), appq(cdr(m)));
}
Cell *apply(Cell *f, Cell *args)
{
	return eval(cons(f, appq(args)), nil);
}
