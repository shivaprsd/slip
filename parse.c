/*
 * An LL(1) recursive descent parser to turn S-Expressions, defined by the
 * following minimal grammar, in to Lisp lists.
 *
 *   <S-Expr>	::=  <atom> | <list> | "'" <S-Expr>
 *   <list>	::=  "(" <space> <items> <space> ")"
 *   <space>	::=  <empty> | " "
 *   <items>	::=  <empty> | <sequence>
 *   <sequence>	::=  <S-Expr> | <S-Expr> " " <space> <sequence>
 *   <atom>	::=  <char> <symbol>
 *   <symbol>	::=  <empty> | <char> <symbol>
 *   <char>	::=  <non-delimiter-character>
 */
#include <ctype.h>
#include <stdbool.h>
#include "io.c"
#include "parse.h"

Cell *parse_sexp(const char s[], unsigned *i)
{
	Cell *cp;

	if ((cp = parse_atom(s, i)) || (cp = parse_list(s, i)))
		return cp;
	if (parse_token(QUOT_MAC, s, i))
		if (cp = parse_sexp(s, i))
			return list(quot, cp);
	return NULL;
}

Cell *parse_atom(const char s[], unsigned *i)
{
	Cell *cp;
	bool isnum;
	char c, t[MAX_SYM_LEN + 1];
	int j = 0, val = NOVAL;

	while (!is_delim(c = s[*i])) {
		if (j < MAX_SYM_LEN)
			t[j++] = c;
		++*i;
	}
	if (!j)
		return NULL;
	t[j] = '\0';
	isnum = parse_num(t, &val);
	cp = addsym(&symtree, t, isnum ? NUM : SYM, val);
	return new_cell(cp->atm);
}

Cell *parse_list(const char s[], unsigned *i)
{
	Cell *cp;

	if (!parse_token(LIST_BEG, s, i))
		return NULL;
	parse_space(s, i);
	if (!(cp = parse_items(s, i)))
		return NULL;
	parse_space(s, i);
	if (!parse_token(LIST_END, s, i))
		return NULL;
	return cp;
}

Cell *parse_items(const char s[], unsigned *i)
{
	if (s[*i] == LIST_END)
		return nil;
	return parse_seq(s, i);
}

Cell *parse_seq(const char s[], unsigned *i)
{
	unsigned j;
	Cell *cp, *seq;

	if (!(cp = parse_sexp(s, i)))
		return NULL;
	j = *i;
	if (parse_token(LIST_SEP, s, i)) {
		parse_space(s, i);
		if (seq = parse_seq(s, i))
			return cons(cp, seq);
		if (*i > j + 2)
			return NULL;
	}
	*i = j;
	return cons(cp, nil);
}

bool parse_token(const char t, const char s[], unsigned *i)
{
	if (s[*i] == t) {
		++*i;
		return true;
	}
	return false;
}

bool parse_num(const char *s, int *n)
{
	long l;
	char *t;

	l = strtol(s, &t, 0);
	if (*t != '\0')
		return false;
	*n = l;
	return true;
}
