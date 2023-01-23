#ifndef DELIM_H
#define DELIM_H

#define CELL_BEG '['
#define CELL_END ']'
#define CELL_SEP '.'

#define LIST_BEG '('
#define LIST_END ')'
#define LIST_SEP ' '

#define QUOT_MAC '\''
#define CMNT_MAC ';'

const char DELIMS[] = {
	CELL_BEG, CELL_END, CELL_SEP,
	LIST_BEG, LIST_END, LIST_SEP,
	QUOT_MAC, CMNT_MAC, '\0'
};

#endif
