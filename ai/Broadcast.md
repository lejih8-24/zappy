# 🚀 Architecture de l'Intelligence Artificielle - Projet Zappy

Ce document décrit la structure, les protocoles et les comportements du client autonome (IA) du projet Zappy. L'IA est conçue pour fonctionner de manière totalement asynchrone, réactive et sécurisée.

---

## 1. Le Paradigme Fondamental : L'Asynchronisme

La règle d'or de Trantor est que le serveur est le seul maître du temps. L'IA ne doit jamais mettre son propre processus en pause pour attendre une réponse.

**Le concept de la File d'Attente (Buffer) :**
Le serveur accepte que chaque joueur ait jusqu'à 10 commandes en attente. L'IA exploite cette mécanique pour planifier ses actions à l'avance.
* **Action :** L'IA empile ses décisions (avancer, tourner, regarder) dans une file d'attente interne et les envoie au serveur.
* **Réaction :** La boucle principale de l'IA écoute le réseau en continu. Dès qu'une réponse arrive, elle l'associe à la commande la plus ancienne de sa file d'attente et met à jour ses connaissances.

---

## 2. Le Centre de Tri (Routage des Messages)

Le réseau mélange deux types de messages qu'il est vital de séparer dès leur réception pour éviter que l'IA ne confonde une discussion avec un inventaire.

**Les Événements Asynchrones (Urgences) :**
Ce sont des messages que le serveur envoie de lui-même. Ils sont traités en priorité absolue et modifient l'état de l'IA sans toucher à sa file d'attente.
* *Exemple :* L'IA reçoit un message indiquant qu'elle a été éjectée. Elle annule immédiatement toutes ses intentions de déplacement en cours car sa position a été faussée par un autre joueur.

**Les Réponses Synchrones :**
Si le message n'est pas une urgence, c'est la réponse à une action demandée. L'IA retire alors la première commande de sa liste d'attente et analyse le résultat (succès, échec, ou données reçues).

---

## 3. La Gestion du Temps et de la Survie

Puisque l'IA ne bloque pas son programme, elle doit "prédire" l'avenir pour ne pas mourir de faim au milieu d'une longue action.

**Le Budget Survie :**
Chaque action possède un coût théorique en unités de temps. L'IA connaît son niveau de nourriture exact uniquement lorsqu'elle reçoit la réponse d'un inventaire.
* *Le calcul :* Avant d'envoyer une nouvelle commande longue (comme une incantation qui coûte 300 unités de temps), l'IA soustrait le coût de toutes les actions déjà dans sa file d'attente à ses réserves de nourriture actuelles.
* *Exemple :* L'IA a 10 unités de nourriture (soit environ 1260 unités de temps de vie). Elle a déjà demandé d'avancer 5 fois (coût 35). Elle estime sa vie restante à 1225. Elle sait donc qu'elle peut sereinement lancer une incantation sans mourir de faim pendant l'incantation.

---

## 4. Le Cerveau Spatial : Vision et Déplacement

L'IA perçoit son environnement à travers une grille linéaire qui s'élargit en forme de cône selon son niveau. La grande force de cette architecture est qu'elle ne cherche jamais à deviner ses coordonnées absolues (X, Y) sur la carte globale, mais raisonne toujours par rapport à elle-même.

**Le Pathfinding Relatif :**
Lorsqu'elle cherche un objet dans son cône de vision, l'IA calcule le chemin mathématique direct en utilisant l'index de la case visée.
* *Le principe :* L'IA détermine sur quelle "ligne devant elle" se trouve l'objet (Axe Y), puis détermine le "décalage" sur la gauche ou la droite (Axe X).
* *Exemple :* L'IA analyse sa vision et trouve de la nourriture sur l'index 10. Les mathématiques lui indiquent que c'est à 3 cases tout droit, et 2 cases sur la gauche. Elle traduit cela en : "Avancer 3 fois, Tourner à Gauche, Avancer 2 fois". Le coût total de cette manœuvre est calculé pour être comparé avec d'autres nourritures potentiellement plus proches.

---

## 5. La Logique Décisionnelle (Machine à États)

L'IA n'est pas un long script linéaire, mais une machine qui change de comportement (État) en fonction de ses priorités. 

1.  **Survie :** Priorité absolue. Si la nourriture estimée tombe sous un seuil critique, l'IA abandonne tout. Elle observe, calcule le chemin vers la nourriture la plus proche, et va s'en nourrir.
2.  **Collecte (Farming) :** Si la survie est assurée, l'IA regarde de quelles pierres elle a besoin pour son prochain niveau. Elle explore et ramasse les ressources manquantes.
3.  **Regroupement :** L'inventaire est plein. L'IA se met en route vers un chef d'équipe, ou devient elle-même chef et attend les autres.
4.  **Élévation :** Les joueurs et les pierres sont réunis. L'IA lance le processus d'incantation.

---

## 6. Le Protocole de Communication (Broadcast)

Pour s'élever jusqu'au niveau 8, les membres de l'équipe doivent se coordonner. Le serveur diffusant les messages à toutes les équipes, une structure de communication sécurisée et imperméable au brouillage adverse est mise en place.

**La Structure du Message :**
Chaque message diffusé est séparé par des **|** et contient les informations suivantes :
1.  **Clé de sécurité (Token) :** Un mot de passe d'équipe. Tout message ne contenant pas ce mot de passe est ignoré.
2.  **Identifiant de l'émetteur :** Un code unique généré par l'IA à sa naissance pour être reconnue formellement.
3.  **Identifiant de la cible :** Permet de s'adresser à un joueur précis ou à toute l'équipe.
4.  **Niveau du joueur :** Pour déterminer si le joueur est éligible à une incantation.
5.  **Rôle Hiérarchique :** Indique si le joueur est un Explorateur, un Chef (qui organise une incantation) ou un Esclave (qui est en route vers un chef).
6.  **Action requise :** L'intention du message (Appel à l'aide, demande de regroupement, confirmation d'arrivée).

* *Exemple de scénario :* Un Chef de niveau 2 a besoin d'un joueur. Il diffuse son appel. Un Explorateur de niveau 2 l'entend, valide le mot de passe, vérifie qu'il a le bon niveau, s'assigne le rôle d'Esclave, enregistre l'identifiant du Chef pour ne plus écouter les autres, et se dirige vers le son de sa voix.