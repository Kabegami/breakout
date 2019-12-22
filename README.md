# Introduction

Ce répertoire est le code pour le jeu de breakout(casse brique) codé en c++ avec la librairie SFMl.

![Snake](img/breakout_demo.png?raw=true "Démo")

Vous pouvez voir dans l'image le rendu final.

# Installation

Le code utilise la librairie SFML donc il faut l'installer. Pour ce faire, suivez les instructions fournit sur le site de la librairie : 
[SFML](https://www.sfml-dev.org/).

Puis il est nécessaire d'indiquer au Makefile la position de ses librairies soit en les rajoutant dans /usr/local/include soit en indiquant les chemins dans les variables SFML_PATH sous la forme : 

```bash
-I /chemin/vers/la/librairie/
```

Pour installer le jeu allez dans le répertoire src et faites :
```bash
make
```

Pour lancer le jeu :
```bash
./breakout
```

# Organisation du code

Les code est décomposé en 3 fichier : 
* game.cpp qui contient la boucle de jeu et gère la création / destruction / affichage des objets
* shape.cpp qui gère les formes géométriques et les calculs de collision / rebond
* gameObjects.cpp qui contient les objets du jeu qui possèdent tous une shape et gère les interactions







