#include "core.c"
#include "util.c"

Cell *symtree = NULL;
Cell *envlist = NULL;
Cell *nil, *tru, *quot;

/* add|lookup atomic symbols in a BST */
Cell *addsym(Cell **cpx, const char *sym, Keywrd key, int val)
{
	int c;
	Cell *cp = *cpx;
	if (!cp) {
		if (key > NKEYS)
			sym = dupstr(sym);
		return *cpx = new_cell(new_atom(sym, key, val));
	}
	if ((c = strcmp(sym, cp->atm->sym)) < 0)
		cp = addsym(&cp->car, sym, key, val);
	else if (c > 0)
		cp = addsym(&cp->cdr, sym, key, val);
	return cp;
}
void initkeys(const char *syms[])
{
	int k;
	nil = addsym(&symtree, syms[NIL], NIL, NOVAL);
	tru = addsym(&symtree, syms[T], T, NOVAL);
	for (k = T + 1; k < NKEYS; ++k)
		addsym(&symtree, syms[k], k, NOVAL);
	quot = addsym(&symtree, syms[QUOTE], QUOTE, NOVAL);
}
void initops(const char *syms[])
{
	int k = 0;
	while (*syms[k])
		addsym(&symtree, syms[k++], NUMOP, NOVAL);
}

Cell *bindenv(Cell *sym, Cell *val)
{
	envlist = cons(list(sym, val), envlist);
	return val;
}
