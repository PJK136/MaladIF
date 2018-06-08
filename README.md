# Malad'IF

## Objectif

Malad'IF est une application aux services des médecins qui permet à partir d’une base de données chargée 
par l’utilisateur de donner les caractéristiques d’une maladie et d'effectuer des diagnostics à partir 
d’un fichier d’empreintes.

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

À compiler en C++ 17

## Mode d’emploi

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

## Auteurs

* **Paul DU** - [PJK136](https://github.com/PJK136)
* **Ismail JATTIOUI** - [yugismop](https://github.com/yugismop)
* **Valentin RIGOLLE** - [Crejak](https://github.com/Crejak)
* **Alexandra VERRIER** - [averrier](https://github.com/averrier)
