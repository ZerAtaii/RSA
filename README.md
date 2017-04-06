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
