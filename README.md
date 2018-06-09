# Malad'IF

## Objectif

Malad'IF est une application aux services des médecins qui permet à partir d’une base de données chargée par l’utilisateur de donner les caractéristiques d’une maladie et d'effectuer des diagnostics à partir d’un fichier d’empreintes.

## Liste des fichiers

CLI.cpp
Database.cpp
FileReader.cpp
CLI.h
CLI_utils.h
Database.h
FileReader.h
FingerPrint.h
Metadata.h
main.cpp

## Compilation du projet

Pour compiler ce projet, il faut CMake, git et un compilateur supportant le C++17 (par exemple GCC >7.0).

À la racine du répertoire des sources (contenant ce README.md), il suffit de taper :

```
mkdir build #Pour ne pas mélanger sources et objets générés
cd build
cmake -DCMAKE_BUILD_TYPE=Release .. #Pour générer la version Release (et non Debug)
make
make test #Pour lancer les tests
```

L'exécutable généré est situé à `build/src/MaladIF`.

Ces commandes fonctionnent bien sous Linux où CMake, GCC et Make déjà sont dans le PATH.

Pour Windows, on peut télécharger [CMake](https://cmake.org/download/) et [Mingw64](https://sourceforge.net/projects/mingw-w64/files/mingw-w64/).

Il faut alors penser à mettre leurs dossiers `bin` dans le PATH, et au lieu de `make`, cela sera `mingw32-make`.

Il est également possible d'ouvrir le projet avec un IDE supportant CMake comme CLion (testé) ou Visual Studio 2017 (non-testé) pour le compiler.

Ou de générer différents Makefile ou projets d'IDE. [Doc](https://cmake.org/cmake/help/v3.0/manual/cmake-generators.7.html) [Tutoriel](http://preshing.com/20170511/how-to-build-a-cmake-based-project/)

## Mode d’emploi

### Application MaladIF

Pour lancer l'application, il suffit de lancer l'exécution `MaladIF` sans paramètres en ligne de commande.

Pour accéder aux fonctionnalités de l’application, il faut d’abord charger la base de données : 

	-> Appuyez sur 1 ou sur C.
	-> Entrez le nom de fichier de métadonnées (sans oublier l’extension) qui se trouve au même niveau que l'exécutable.
	-> Entrez le nom de fichier de données (sans oublier l’extension) correspondant au fichier de métadonnées qui se trouve au même niveau que l'exécutable.

Pour avoir la liste des maladies répertoriées :
	
	-> Appuyez sur 2 ou sur L.

Pour avoir les caractéristiques d’une maladie en particulier :

	-> Appuyez sur 3 ou sur A.
	-> Entrez le nom de la maladie.

Pour demander un diagnostic d’empreintes : 
	
	-> Appuyez sur 4 ou sur D.
	-> Entrez le nom du fichier contenant les empreintes (sans oublier l’extension).

Pour quitter l’application : 
	
	-> Appuyez sur 5 ou Q.

Il est également possible de lancer l'application et d'effectuer directement un diagnostic sans passer par le menu :

```
./MaladIF metadata.txt data.txt empreintes.txt
```

### Générateur de données aléatoires

Pour générer un fichier de métadonnées `metadata.txt` avec `[nbAttributs]` le nombre d'attributs en dehors de l'ID:

```
./generateRandomMetadata.py [nbAttributs] > metadata.txt
```

Pour générer un fichier de données `data.txt` avec `[nbEmpreintes]` le nombre d'empreintes et `[nbMaladies]` le nombre de maladies différentes :

```
./generateRandomData.py metadata.txt [nbEmpreintes] [nbMaladies]
```

Attention, le fichier `diseases.txt` doit être dans le répertoire d'exécution de `generateRandomData.py`.

## Auteurs

* **Paul DU** - [PJK136](https://github.com/PJK136)
* **Ismail JATTIOUI** - [yugismop](https://github.com/yugismop)
* **Valentin RIGOLLE** - [Crejak](https://github.com/Crejak)
* **Alexandra VERRIER** - [averrier](https://github.com/averrier)

