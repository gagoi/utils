#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define F_CONSTRUCTOR (1 << 0)
#define F_DESTRUCTOR (1 << 1)
#define F_LIBRARIES (1 << 2)

void checkParams(short * flag, int argc, char ** argv);
void createFiles(short flag, int argc, char ** argv);
void createConstructor(FILE * hpp, FILE * cpp, char * nom);
void createDestructor(FILE * hpp, FILE * cpp, char * nom);
void createIncludes(FILE * hpp);
void printParams(short flag);

int main(int argc, char ** argv)
{
	short flag = 0;

	if (argc > 1)
	{
		checkParams(&flag, argc, argv);
		printParams(flag);
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
				case 'c': // Retirer le constructeur.
					*flag += F_CONSTRUCTOR;
					break; 
				case 'd': // Retirer le destructeur.
					*flag += F_DESTRUCTOR;
					break; 
				case 'i': // Retirer les librairies.
					*flag += F_LIBRARIES;
					break;
				case 'h': // Afficher l'aide et quitter (seulement si c'est le premier paramètre)
					if (i == 1)
					{
						printf("Ce programme créer un fichier .cpp et un fichier .hpp correspondant au(x) nom(s) passé(s) en argument.\n\nOptions :\
						\n- \"-c\" : Retire les constructeurs par défaut aux librairies.\
						\n- \"-d\" : Retire les destructeurs par défaut aux librairies.\
						\n- \"-i\" : Retire l'inclusion par défaut des librairies.\
						\n");
						return;
					}
					break;
				default:
					printf("Argument %s non reconnu, pour afficher l'aide utiliser \"%s -h\"\n", argv[i], argv[0]);
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

	for (i = 1; i < argc; ++i) // Pour chaque argument de lancement
	{
		if (argv[i][0] != '-') // Si c'est un paramêtre
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


			if (cpp && hpp) // Si les fichiers sont bien ouverts
			{
				fprintf(cpp, "#include <%s.hpp>\n", nom);
				fprintf(hpp, "#ifndef %s\n#define %s\n\n", gardien, gardien);
				if (!(flag & F_LIBRARIES)) createIncludes(hpp);
				fprintf(hpp, "\nclass %s\n{\nprivate:\npublic:\n", nom);
				if (!(flag & F_CONSTRUCTOR)) createConstructor(hpp, cpp, nom);
				if (!(flag & F_DESTRUCTOR)) createDestructor(hpp, cpp, nom);

				fprintf(hpp, "}\n\n#endif\n");

				// Libération des pointeurs
				fclose(cpp);
				fclose(hpp);
			}
		}
	}
}

void createIncludes(FILE * hpp)
{
	char buffer[100];

	// On utilise les variables d'environnements pour accéder à l'emplacement du fichier de configuration
	#ifdef __WIN32__ // Si on est sous windows on utilise userprofile
		sprintf(buffer, "%s/.libcreator", getenv("USERPROFILE"));
	#else // Sinon on suppose l'utilisation de linux et on utilise home
		sprintf(buffer, "%s/.libcreator", getenv("HOME"));
	#endif

	FILE * prop = fopen(buffer, "r"); // Ouverture du fichier
	if (prop) // Si il est ouvert
		while(fscanf(prop, "%s", buffer) != EOF) // On le parcourt en entier
			fprintf(hpp, "#include <%s.hpp>\n", buffer); // Pour ajouter le bon include au header
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

void printParams(short flag)
{
	printf("Constructeur : %s\n", (flag & F_CONSTRUCTOR) ? "OFF" : "ON");
	printf("Destructeur : %s\n", (flag & F_DESTRUCTOR) ? "OFF" : "ON");
	printf("Librairies : %s\n", (flag & F_LIBRARIES) ? "OFF" : "ON");
}