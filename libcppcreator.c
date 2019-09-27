#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define F_CONSTRUCTOR (1 << 0)
#define F_DESTRUCTOR (1 << 1)

void checkParams(short * flag, int argc, char ** argv);
void createFiles(short flag, int argc, char ** argv);
void createConstructor(FILE * hpp, FILE * cpp, char * nom);
void createDestructor(FILE * hpp, FILE * cpp, char * nom);

int main(int argc, char ** argv)
{
	short flag = 0;


	if (argc > 1)
	{
		checkParams(&flag, argc, argv);
		createFiles(flag, argc, argv);
	}
	else
	{
		printf("Pour afficher l'aide utiliser \"%s -h\"\n", argv[0]);
	}
	
}

void checkParams(short * flag, int argc, char ** argv)
{
	int i;
	for (i = 1; i < argc; ++i) // Parcourt des paramêtres.
	{
		if (argv[i][0] == '-') // Si le premier argument est un paramêtre. 
		{
			switch(argv[i][1])
			{
				case 'c': // Ajouter le constructeur.
					*flag += F_CONSTRUCTOR;
					printf("Ajout de constructeur(s)\n");
					break; 
				case 'd': // Ajouter le destructeur.
					*flag += F_DESTRUCTOR;
					printf("Ajout de destructeur(s) : %d\n", F_DESTRUCTOR);
					break; 
				case 'h': // Afficher l'aide et quitter (seulement si c'est le premier paramètre)
					if (i == 1)
					{
						printf("Ce programme créer un fichier .cpp et un fichier .hpp correspondant au(x) nom(s) passé(s) en argument.\n\nOptions :\
						\n- \"-c\" : Retire les constructeurs par défaut aux librairies.\
						\n- \"-d\" : Retire les destructeurs par défaut aux librairies.\
						\n");
						return;
					}
					break;
				default:
					printf("Pour afficher l'aide utiliser \"%s -h\"\n", argv[0]);
					break;
			}
		}
	}
}


void createFiles(short flag, int argc, char ** argv)
{
	int i, j;
	char nom[100];
	char gardien[105];
	char buffer[104];

	FILE * hpp = NULL;
	FILE * cpp = NULL;

	for (i = 1; i < argc; ++i)
	{
		if (argv[i][0] != '-')
		{
			// Ajout d'une majuscule au nom de la librairie.
			strcpy(nom, argv[i]);
			nom[0] = toupper(nom[0]);

			// Création du nom du gardien.
			j = 0;
			while (gardien[j++] = toupper(nom[j]));
			strcat(gardien, "_HPP_");

			printf("Création de la librairie : %s\n", nom);
			
			// Ouverture des fichiers.
			sprintf(buffer, "%s.cpp", nom);
			cpp = fopen(buffer, "w");

			sprintf(buffer, "%s.hpp", nom);
			hpp = fopen(buffer, "w");


			if (cpp && hpp)
			{
				printf("%d\n", flag);

				fprintf(cpp, "#include <%s.hpp>\n", nom);
				fprintf(hpp, "#ifndef %s\n#define %s\n\nclass %s\n{\nprivate:\npublic:\n", gardien, gardien, nom);

				if (!(flag & F_CONSTRUCTOR)) createConstructor(hpp, cpp, nom);
				if (!(flag & F_DESTRUCTOR)) createDestructor(hpp, cpp, nom);

				fprintf(hpp, "}\n\n#endif\n");

				fclose(cpp);
				fclose(hpp);
			}
		}
	}
}


void createConstructor(FILE * hpp, FILE * cpp, char * nom)
{
	fprintf(cpp, "\n%s::%s()\n{\n}\n", nom, nom);
	fprintf(hpp, "\t%s();\n", nom);
}

void createDestructor(FILE * hpp, FILE * cpp, char * nom)
{
	fprintf(cpp, "\n%s::~%s()\n{\n}\n", nom, nom);
	fprintf(hpp, "\t~%s();\n", nom);
}