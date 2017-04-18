#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "adblock.h"


void getList(char** list,char* filepath) {
  FILE* fichier = NULL;
  char chaine[3000] = "";
  fichier = fopen(filepath, "r+");
  int i = 0;
  if (fichier != NULL)
    {
      while (fgets(chaine, 3000, fichier) != NULL) // On lit le fichier tant qu'on ne reçoit pas d'erreur (NULL)
        {
            list[i]=chaine;
            i++;
        }
      fclose(fichier);
    }
    else
    {
        printf("Impossible d'ouvrir le fichier %s",filepath);
    }
}

int isInTheList(char** list,char* host) {
  int i;
  for (i=0;i<sizeof(list);i++) {
    if (strstr(host,list[i]) != NULL) { //Si le mot est dans la liste
      return 1;
    }
  }
  return 0;
}
