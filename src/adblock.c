#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "adblock.h"


void getList(char** list,char* filepath) {
  FILE* fichier = NULL;
  char *chaine=malloc(3000*sizeof(char));
  fichier = fopen(filepath, "r+");
  int i = 0;
  if (fichier != NULL)
    {
      while (fgets(chaine, 3000, fichier) != NULL) // On lit le fichier tant qu'on ne reçoit pas d'erreur (NULL)
        {
          int j = strlen(chaine);
          while (j > 0 && chaine[j-1] == '\n')
              j--;
          chaine[j] = '\0';
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
  char * res=NULL;
  for (i=0;i<sizeof(list);i++) {
    if ( (res =strstr(host,list[i])) != NULL) { //Si le mot est dans la liste
      return 1;
    }
  }
  return 0;
}
