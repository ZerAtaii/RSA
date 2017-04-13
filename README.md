# RSA


# Algorithme général de MyAdBlock (adapté depuis le site internet pour nous aider)

- Créer une nouvelle Socket chaque nouvelle connexion
- Recevoir le paquet
- Analyser l'header http
- En déduire l'host à qui envoyer le paquet
- Envoyer le paquet vers l'host
- Attendre réponse du serveur
- Renvoyer la réponse au client (en cas de succès ou d'echec)


# Algorithme général de MyAdBlock

- Le client envoie une requete TCP vers un host
- Server.c du proxy analyse l'entête HTTP pour trouver le server web
- Server.c analyse l'ensemble des entêtes et vérifie qu'il ne se trouve pas dans la liste des hébergeurs à éviter
- Server.c transmet la requête modifée à Client.c
- Client.c effectue la requête vers le serveur web
- Le server web répond à Client.c
- Client.c transmet la réponse au client en passant par Server.c (je crois que c'est obligatoire mais pas certain ...)

# Différents liens pouvant être utiles

https://developer.mozilla.org/en-US/docs/Web/HTTP/Methods/OPTIONS : pour l'entête http
http://forum.hardware.fr/hfr/Programmation/C/socket-programmer-proxy-sujet_88089_1.htm : forum programmation proxy
http://codes-sources.commentcamarche.net/source/10846-serveur-proxy-http-win32 : un proxy mais écrit en C++

# Avancé du projet

On a un proxy fonctionel malgré quelque segfault inexpliqué par moment ou le fait que le proxy quitte sans raison.
De plus, quand il s'éteint la connexion reste valide, alors que par moment j'ai bien le "Impossible d'accéder à la page, vérifier votre proxy" ce qui est logique s'il a planté. Truc chelou de ce coté ...
Après j'ai une wifi degueux sur un bateau, donc tu me diras ce qu'il se passe chez toi.
On a proxy banale pour l'instant, l'objectif étant de filtrer les herbergeurs reconnus pour faire de la pub, j'ai mis un ****CODE A AJOUTER ICI*** dans le proxy.c, on peut pas le rater :P

# Pour utiliser le proxy
Donc, en gros tu fais un make pour avoir l'executable (enfin l'équivalent linux).
Tu fais ./proxy numeroduport
Ensuite, tu fais des modifs sur ton navigateur pour utiliser un proxy local à l'adresse:
127.0.0.1 et au port : numeroduport
Ensuite, dans le terminal, tu dois avoir les données qui nous interessent
