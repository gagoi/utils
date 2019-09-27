#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char ** argv)
{
	int i;
	FILE * f = NULL;
	char buffer[1000];

	if (argc > 1)
	{
		for (i = 1; i < argc; ++i)
		{
			sprintf(buffer, "%s.cpp", argv[i]);
			f = fopen(buffer, "w");
			if (f)
			{
				fprintf(f, "#include <%s.hpp>\n\n", argv[i]);
				fclose(f);
			}
		}
	}
}