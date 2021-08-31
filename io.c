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

bool is_invLA(char cur, char la)
{
	cur = isalpha(cur) ? 'a' : cur;
	la = isalpha(la) ? 'a' : la;
	switch (cur) {
	case '[':
		return la == '.' || la == ',';
	case '.':
	case ',':
		return la == '.' || la == ',' || la == ']';
	case 'a':
	case ']':
		return la == '[' || la == 'a';
	}
}

Cell *unwind(Cell *st)
{
	Cell *cp = NULL;
	if (!st)
		return NULL;
	cp = st->car;
	st = st->cdr;
	for (; st != NULL; st = st->cdr)
		cp = cons(st->car, cp);
	return cp;
}

int readlist(Cell **cp, char s[])
{
	int i;
	char c, *beg;
	bool dotp = false;
	Cell *tmp, *st = NULL;
	if (*s == '\0')
		return 0;
	if (*s != '[') {
		if (!isalpha(*s))
			return 0;
		*cp = new_cell(NULL);
		(*cp)->atm = new_atom('\0');
		return readatom((*cp)->atm, s);
	}
	beg = s;
	while ((c = *s++)) {		// -Wparen
		if (is_invLA(c, *s))
			return beg - s;
		switch (c) {
		case '.':
			if (!dotp) {
				dotp = true;
				break;
			}
			/* else fall through */
		case ',':
			if (dotp)
				return beg - s + 1;
			break;
		case ']':
			if (!dotp)
				st = cons(&NIL, st);
			*cp = unwind(st);
			return s - beg;
		case '[':
			if (s == beg + 1)
				break;
			/* else fall through */
		default:
			i = readlist(&tmp, --s);
			if (i <= 0)
				return beg - s + i;
			s += i;
			st = cons(tmp, st);
		}
	}
	return beg - s + 1;
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
