#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <math.h>
#include <netdb.h>
#include "adblock.h"

char **list;
/*
* Code qui réalise le proxy le plus basique possible, en modifiant la fonction proxy appelée
* par les threads lors de connexion, on peut choisir de garder ou non les demandes HTTP et
* donc d'éliminer les hébergeurs de pub connus sur le site donné
*/






/* cut_host_and_port
* separe le host de son port
*/
void cut_host_and_port(char * host, char * port){
 unsigned short i = 0;
 unsigned short y = 0;
 unsigned short size = strlen(host);
 while(host[i] != ':') {
  i++; }
 host[i] = '\0';
 i++;
 for(y = 0; y < 5, i < size; i++, y++) {
 	port[y] = host[i]; }
}

/* getURL()
* recupère l'URL pour la comparer à la liste
*/
char* getURL(char * httpRequest) {
  unsigned short start=0,end=0,i=0;
  char * url = (char *)malloc(512 * sizeof(char));
  while (httpRequest[end] != '\n'){
    end++;
  }
  while (httpRequest[start] != 'h'){
    start++;
  }
  for (i=start; i<end-11 ; i++) {
    url[i-start]=httpRequest[i];
  }
  return url;
}


/* get_service()
* Isole le service cherché HTTP ou HTTPS
*/

char* get_service(char * httpRequest) {
  unsigned short start=0,end=0,i=0;
  char * service = (char *)malloc(10 * sizeof(char));
  while (httpRequest[end] != ':'){
    end++;
  }
  //if (end>5) {
  //  end = 5;
  //}
  while (httpRequest[start] != 'h'){
    start++;
  }
  for (i=start; i<end ; i++) {
    service[i-start]=httpRequest[i];
  }
  service[end-start]='\0';
  return service;
}

/*
* get_host()
* fonction qui isole le header http (Host:)
*/
char * get_host(char * httpRequest){
	unsigned short i = 0, j = 0;
	char * buffer = strstr(httpRequest, "Host: " );
	char * host = (char *)malloc(256 * sizeof(char));

	while( buffer[i] != '\n' ) {
 		i++;
	}
	for(j = 6; j < i-1; j++) {
 		host[j - 6] = buffer[j];
		host[j-6+1] = '\0';
	}
	return host;
}

/*
* proxy()
*/
int proxy (int* socket_arg){
  int * ps = socket_arg;
  int socket_client = *ps;
  close(&ps);
  struct addrinfo * structHost,*rp;
  struct addrinfo hints;
  hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
  hints.ai_socktype = SOCK_STREAM;

  int sfd;
  char buffer[1024*8];  // Buffer de 8 ko
  char buffer2[1024*8]; // 2ème Buffer de 8 ko
  memset(buffer,'\0', sizeof(buffer)); // On initialise un buffer vide

  // On recoit la requete du navigateur
  int size = recv(socket_client, buffer, sizeof(buffer), 0);
  if (strlen(buffer)==0) {
    printf("Connection expired\n");
    return 1;
  }
  printf("----Request received ----\r\n%s-----------------\r\n", buffer);
  // On récupère le header (Host:) de la requete
  // pour pouvoir se connecter au serveur web
  char * host = get_host(buffer);
  // On récupère le service pour utiliser getaddrinfo
  char * service = get_service(buffer);

  //On récupère l'URL pour bloquer la pub
  char * url = getURL(buffer);

  // Parfois host a la forme (Host: adresse:port
  // donc on creer une variable port pour récupérer ce dernier
  char * port = (char *)malloc(5 * sizeof(char));
  if( !strncmp(host, "error", 5) ){ //Compare les strings 1 et 2 sur n bits
   perror("get_host return error.\r\n");
   close(socket_client);
   return 1;
   }else if(strchr(host,':')) { //Cherche l'occurence de ':' dans le string pointé en argument
    cut_host_and_port(host, port); //Cas spécial ou on doit séparer l'host et le port (le port est dans l'host apres les :)
   }
   else{
    port = "80"; //Sinon on prend le port 80 par défaut (port HTTP normal / 443 pour le HTTPS)
   }
   int isIn=4;
   if ((isIn =isInTheList(list,url)) > 0) {
     printf("Host : [%s],  BLOCKED \r\n", host);
     return 0;
   }
   printf("Host : [%s], port : [%s], service : [%s] \r\n", host, port,service);

   int status;
   // Gestion des erreurs
   if( (status = getaddrinfo(host,service,&hints,&structHost)) != 0){
    	printf("Host error. Can't find %s Error: %s \r\n", host,gai_strerror(status));
    	return 1;
   }

   // On envoie la requete au serveur web
   // grace a une nouvelle socket



   //On se connecte au premier serveur disponible dans la liste chainée

   for (rp = structHost; rp != NULL; rp = rp->ai_next) {
           sfd = socket(rp->ai_family, rp->ai_socktype,rp->ai_protocol);
           if (sfd == -1) {
               continue; }

           if (connect(sfd, rp->ai_addr, rp->ai_addrlen) != -1) {
               break; }                  /* Success */

           close(sfd);
       }
  if (rp == NULL) {               /* No address succeeded */
           fprintf(stderr, "Could not connect to %s\r\n",host);
           return(1);
  }

   send(sfd, buffer, strlen(buffer), 0);
   // Reception des données à partir du serveur
   // et on les renvoie au navigateur
   while( (size = recv(sfd, buffer2, sizeof(buffer2),0)) > 0){
    buffer2[size]='\0';
    send(socket_client, buffer2, size, 0);
    }
   //Nettoyage
    close(socket_client);
    close(sfd);
    freeaddrinfo(structHost);
    return 0;
   }



int main(int argc, char * argv[]){
  if (argc<2) {
    printf("Please use the proxy like ./proxy numberofport\n");
    return 1;
  }
  printf("start\n");
  list = malloc (61050*sizeof(char*));
  for (int j=0;j<61050;j++) {
    list[j]=malloc(1000*sizeof(char));
  }
  getList(list,"pub.txt");
  //Initialisation des variables
  struct sockaddr_in serv_addr, cli_addr;
  int socket_local;
  int socket_client;
  pthread_t * threads;
  threads = malloc(5*sizeof(pthread_t));
  int sin_size = sizeof(serv_addr);

  // Définition du proxy port en argument
  serv_addr.sin_family = 	AF_UNSPEC; // IPv4 ou IPv6
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); // on peut recevoir les messages de toute ip
	serv_addr.sin_port = htons(atoi(argv[1]));

  // Initialisation de la socket locale
  socket_local = socket(AF_INET,SOCK_STREAM,0);
  if( socket_local  < 0 )
  {
      perror("Socket error !\n");
      exit(1);
  }

  // On lie la socket à son sockaddr_in
  if (bind(socket_local,(struct sockaddr *)&serv_addr,sizeof(struct sockaddr)) < 0) {
    perror("Bind error\n");
    exit(1);
  }

  // On définie la taille de la file de connexion en attente pour la socket
  listen(socket_local, 5);

  while(1) {
    if( (socket_client = accept(socket_local,(struct sockaddr * )&cli_addr,(socklen_t *)&sin_size)) < 0 )
    {
      perror("Accept error !" );
      close(socket_client);
      exit(1);
    }
    else
    {
    	//printf("Connexion de : %s \n",inet_ntoa(cli_addr.sin_addr));
      // On cree un thread par nouvelle connexion en
      // appelant la fonction proxy avec comme paramètre le socket client
      // Si erreur à la creation d'un thread
      pthread_t thread;
      if(		pthread_create(&thread,NULL,(void*)proxy,&socket_client) < 0 )
      {
       perror("CreateThread error !" );
       exit(1);
      }
    }
  }
  return(0);
}
