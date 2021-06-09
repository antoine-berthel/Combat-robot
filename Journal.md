------------------
# Journal de bord
------------------
# A faire :
	- Les fonction de base de :	- arene.c
								- missile.c
								- robot.c
	- Le parser
	- L'Interface Visuelle
	
	Arene.c :
		- Robot coin d'arene
		- Collision() (Robot -> Robot); (Robot -> Mur); (Robot -> Missile)
		-
	Robot.c :
		- Init()
		- NextInstruction()
		- Degat()
		- posToInt()
		- MiseAJour()
		- Destruction()

	Robot.h : (Ajouter un etat de moteur ?)
		struct engine: Vitesse et direction
		struct shoot: Direction et distance
		union action : Engine et shoot


	missile.c : 
		- Explose()
		- MiseAJour()

	Cycle :
		- Robot.NextInstruction()
		- robot.MiseAJour()
		- missile.MiseAjour()
		- missile.Explose()
		- robot.Degat()

	A voir : 
		- 
____________________
## Date : 05/02/2020
### Travail effectué:
Aujourd'hui, nous avons commencé par discuter d'une possible implementation du projet, et de ce que nous allions devoir faire.
Nous avons défini comment implementer les différents objets.

Class robot:			
Position			        | #define missile_max( 2)
Vitesse (pourcentage)		| #define size (10x10)
Dégâts (pourcentage)		| #define vitesse_max (10)
Nb missiles en cour 		| #define collision (2%)
Script				        |
Mort (boolean)			    |

Class missile :
Position			| #define vitesse (500)
Position_depart	    | #define distance_max (7000)
				    | #define d_400 (3%)
				    | #define d_200 (5%)
				    | #define d_50 (10%)

Class arene:
Liste robot (max_robot)	| #define x (10 000)
Liste missile			| #define y (10 000)
					    | #define robot (4)


### Travail à faire:

Reflechir à l'implementation du cycle et implementer les fichiers .c


## Date : 12/02/2020
### Travail effectué:
Mise en place des differentes etapes à faire. (Section 'A faire')
On commencé à regarder le fonctionnement de l'affichage dans le terminal et debuté les fonctions. (Collision)
### Travail à faire:

## Date : 11/03/2020
### Travail effectué:
Nous avons revu la structure du projet afin d'en faciliter la lecture.
Nous avons réglé des problèmes liés a CMake. Certaines fonction ont été modifier ou ajouter car nous nous somme rendu compte qu'il manquait des choses.
L'affichage avance petit à petit. La compréhention de Ncurses commence a rentrer.
### Travail à faire:
Avancer l'affichage et faire le parser (voir s'il ne manque pes d'autres fonctions et si rien n'est a corriger).