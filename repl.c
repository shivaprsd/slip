#include <stdio.h>
#include <stdlib.h>
#include <editline/readline.h>

int main()
{
	char *s;
	while (s = readline("> ")) {
		puts(s);
		add_history(s);
		free(s);
	}
	return 0;
}
