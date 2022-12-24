#ifndef PARSE_H
#define PARSE_H

Cell *parse_sexp (const char s[], unsigned *i);
Cell *parse_atom (const char s[], unsigned *i);
Cell *parse_list (const char s[], unsigned *i);
Cell *parse_items(const char s[], unsigned *i);
Cell *parse_seq  (const char s[], unsigned *i);

bool parse_token (const char t, const char s[], unsigned *i);

#endif