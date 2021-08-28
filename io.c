#include <ctype.h>
#include "core.c"
int buildcell(Cell **cpx, char s[], char t);

int readatom(Atom *ap, char s[])
{
	char c;
	int i = 0;
	if (!ap)
		return -1;
	while (isalpha(c = s[i++]))
		ap->sym = c;
	return i - 1;
}

int readexp(Cell *cp, char s[])
{
	int carl, cdrl;
	if (!cp)
		return -1;
	if (s[0] != '[') {
		cp->atm = new_atom('\0');
		return readatom(cp->atm, s);
	}
	carl = buildcell(&cp->car, ++s, '.');
	if (carl <= 0)
		return carl - 1;
	cdrl = buildcell(&cp->cdr, ++s + carl, ']');
	return (cdrl <= 0)
		? cdrl - carl - 2
		: cdrl + carl + 3;
}

int buildcell(Cell **cpx, char s[], char t)
{
	int l;
	*cpx = new_cell(NULL);
	l = readexp(*cpx, s);
	if (l > 0 && s[l] != t)
		return -l;
	return l;
}

void printexp(Cell *cp)
{
	if (!cp)
		return;
	if (is_atom(cp)) {
		putchar(cp->atm->sym);
		return;
	}
	putchar('[');
	printexp(cp->car);
	putchar('.');
	printexp(cp->cdr);
	putchar(']');
}
