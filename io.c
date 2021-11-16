#include <ctype.h>
#include "delims.h"
#include "store.c"

int readatom(Cell **cpx, char s[])
{
	char c;
	int i = 0, j;
	Cell *cp;
	if (isalpha(s[0]))
		while (isalnum(s[++i]))
			;
	if (i) {
		c = s[i]; s[i] = '\0';
		cp = addsym(&symtree, s, NOKEY);
		*cpx = new_cell(cp->atm);
		s[i] = c;
	}
	return i;
}

bool is_invch(char c, bool dotp)
{
	return dotp && (c == CS || c == LS);
}
bool is_invLA(char cur, char la)
{
	cur = isalnum(cur) ? 'a' : cur;
	la = isalnum(la) ? 'a' : la;
	switch (cur) {
	case LB:
		return la == CS || la == LS;
	case CS:
	case LS:
		return la == CS || la == LS || la == RB;
	case 'a':
		return la == LB;
	case RB:
		return la == LB || la == 'a';
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
	int i = 0;
	char c, *beg;
	bool dotp = false;
	Cell *tmp, *stk = NULL;
	if (*s != LB)
		return readatom(cpx, s);
	beg = s;
	while ((c = *s++)) {			// -Wparen
		if (is_invch(c, dotp))
			break;
		if (is_invLA(c, *s)) {
			++s;
			break;
		}
		if (i > 1) {
			--i;
			continue;
		}
		switch (c) {
		case CS:
			dotp = true;
		case LS:
			continue;
		case RB:
			if (!dotp)
				stk = cons(nil, stk);
			*cpx = unwind(stk);
			return s - beg;		/* success */
		case LB:
			if (s == beg + 1)
				break;
			/* else fall through */
		default:
			i = readlist(&tmp, s - 1);
			if (i <= 0)
				return beg - s + 1 + i;
			stk = cons(tmp, stk);
		}
	}
	return beg - s + 1;	/* error */
}

bool printatm(Cell *cp, bool dot)
{
	if (!is_atom(cp))
		return false;
	putchar(dot ? CS : 0);
	fputs(cp->atm->sym, stdout);
	return true;
}
void printexp(Cell *cp)
{
	if (!cp || printatm(cp, false))
		return;
	putchar(LB);
	printexp(cp->car);
	putchar(CS);
	printexp(cp->cdr);
	putchar(RB);
}
