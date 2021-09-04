#include <ctype.h>
#include "core.c"
int buildcell(Cell **cpx, char s[], char t);

int readatom(Atom *ap, char s[])
{
	char c;
	int i = 0;
	if (ap) {
		while (isalpha(c = s[i++]))
			ap->sym = c;
	}
	return i - 1;
}

int readcell(Cell **cpx, char s[])
{
	if (cpx) {
		*cpx = new_cell(NULL);
		(*cpx)->atm = new_atom('\0');
		return readatom((*cpx)->atm, s);
	}
	return -1;
}

bool is_invch(char c, bool dotp)
{
	return dotp && (c == '.' || c == ',');
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
	/* unknown cur => cannot determine validity of la */
	return false;
}

Cell *unwind(Cell *stk)
{
	Cell *cp = NULL;
	if (stk) {
		cp = stk->car;
		while ((stk = stk->cdr))		// -Wparen
			cp = cons(stk->car, cp);
	}
	return cp;
}

int readlist(Cell **cpx, char s[])
{
	int i;
	char c, *beg;
	bool dotp = false;
	Cell *tmp, *stk = NULL;
	if (*s != '[') {
		if (!isalpha(*s))
			return 0;
		return readcell(cpx, s);
	}
	beg = s;
	while ((c = *s++)) {			// -Wparen
		if (is_invch(c, dotp))
			break;
		if (is_invLA(c, *s)) {
			++s;
			break;
		}
		switch (c) {
		case '.':
			dotp = true;
		case ',':
			continue;
		case ']':
			if (!dotp)
				stk = cons(&NIL, stk);
			*cpx = unwind(stk);
			return s - beg;		/* success */
		case '[':
			if (s == beg + 1)
				break;
			/* else fall through */
		default:
			i = readlist(&tmp, --s);
			if (i <= 0)
				return beg - s + i;
			stk = cons(tmp, stk);
			s += i;
		}
	}
	return beg - s + 1;	/* error */
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
