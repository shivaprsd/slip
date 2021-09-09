#include "core.c"

Cell *symtree = NULL;
Cell *nil, *tru, *quot;

/* add|lookup atomic symbols in a BST */
Cell *addsym(Cell **cpx, char sym, char key)
{
	char c;
	Cell *cp = *cpx;
	if (!cp)
		return *cpx = new_cell(new_atom(sym, key));
	if ((c = cp->atm->sym) > sym)
		cp = addsym(&cp->car, sym, key);
	else if (c < sym)
		cp = addsym(&cp->cdr, sym, key);
	return cp;
}
void initkeys(const char syms[])
{
	int k;
	nil = addsym(&symtree, syms[NIL], NIL);
	tru = addsym(&symtree, syms[T], T);
	quot = addsym(&symtree, syms[QUOTE], QUOTE);
	for (k = T + 1; syms[k] != '\0'; ++k)
		addsym(&symtree, syms[k], k);
}
