/*
 * An LL(1) recursive descent parser to turn S-Expressions, defined by the
 * following minimal grammar, in to Lisp lists.
 *
 *   <S-Expr>	::=  <atom> | <list>
 *   <list>	::=  "[" <space> <items> <space> "]"
 *   <space>	::=  <empty> | " "
 *   <items>	::=  <empty> | <sequence>
 *   <sequence>	::=  <S-Expr> | <S-Expr> "," <space> <sequence>
 *   <atom>	::=  <letter> <symbol>
 *   <symbol>	::=  <empty> | <letter> <symbol>
 */
#include <ctype.h>
#include <stdbool.h>
#include "io.c"
#include "parse.h"

Cell *parse_sexp(const char s[], unsigned *i)
{
	Cell *cp;

	if (cp = parse_atom(s, i))
		return cp;
	return parse_list(s, i);
}

Cell *parse_atom(const char s[], unsigned *i)
{
	Cell *cp;
	char c, t[MAX_SYM_LEN + 1];
	unsigned j = 0;

	while (isalpha(c = s[*i])) {
		if (j < MAX_SYM_LEN)
			t[j++] = c;
		++*i;
	}
	if (!j)
		return NULL;
	t[j] = '\0';
	cp = addsym(&symtree, t, NOKEY);
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
