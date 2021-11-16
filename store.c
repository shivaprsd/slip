#include "core.c"
#include "util.c"

Cell *symtree = NULL;
Cell *nil, *tru, *quot;

/* add|lookup atomic symbols in a BST */
Cell *addsym(Cell **cpx, const char *sym, Keywrd key)
{
	int c;
	Cell *cp = *cpx;
	if (!cp) {
		if (key == NOKEY)
			sym = dupstr(sym);
		return *cpx = new_cell(new_atom(sym, key));
	}
	if ((c = strcmp(sym, cp->atm->sym)) < 0)
		cp = addsym(&cp->car, sym, key);
	else if (c > 0)
		cp = addsym(&cp->cdr, sym, key);
	return cp;
}
void initkeys(const char *syms[])
{
	int k;
	nil = addsym(&symtree, syms[NIL], NIL);
	tru = addsym(&symtree, syms[T], T);
	quot = addsym(&symtree, syms[QUOTE], QUOTE);
	for (k = T + 1; k < NKEYS; ++k)
		addsym(&symtree, syms[k], k);
}
