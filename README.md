# 2526_PROJET2A_TDI
Total Drive Immersion - Motion System


The objective of this project is to create a 3 axis motion plateform for car racing simulations.
It consist of four linear actuators (with their respective [**drivers**](https://github.com/INBOARDBOT/2526_PROJET2A_TDI/tree/main/HARDWARE/PCB'S/FOC_CONTROLLER_V2)), a Nucleo Discovery which handle
the communication between the drivers and the computer as well as a touchscreen interface.
The data is sent from SimHub to the the Nucleo via UART. Then the position values are distributed to each actuator via an [**RS485**](https://github.com/INBOARDBOT/2526_PROJET2A_TDI/tree/main/HARDWARE/PCB'S/COM_BOARD) bus.



# 2526_PROJET2A_TDI
Total Drive Immersion - Motion System


Master - Slave synchonization cycle  
![Diagram Name](masterslaveSync.drawio.svg)

Backend - Frontend synchonization cycle  
![Diagram Name](backendfrontendSync.drawio.svg)


# Achitecture logicielle

L'architecture logicielle est conçue de manière modulaire, segmentant les fonctionnalités du simulateur de conduite en plusieurs sous-systèmes spécialisés. Cette approche permet de séparer les responsabilités et de faciliter la maintenance du système. L'architecture s'articule autour de trois piliers principaux :

Couche Communication : Gère les protocoles d'échange de données entre l'unité centrale et les drivers de vérins (via le bus RS-485), garantissant l'intégrité et la rapidité des transmissions.

Module de Gestion des Actionneurs : Assure le traitement des commandes, le calcul des trajectoires des vérins et le monitoring de l'état des drivers.

Interface Graphique et IHM : Pilote le rendu visuel sur l'écran LCD-TFT et traite les interactions tactiles de l'utilisateur, tout en gérant les ressources stockées en mémoire Flash.

## Présentation des Composants

### Unité Centrale

La centrale controle l'enssemble du systeme de simulation, elle doit etre reactive et embarquer une interface graphique servant d'IHM. Voici donc les principales fonctionnalites que la carte STM32F746G nous propose :
* Puissance de calcul : Architecture Arm Cortex STM32F746NGH6
* Interactions avec l'utilisateur (IHM) : Ecran tactile 4.3” RGB 480×272 color LCD-TFT
* Stockage des ressources graphiques lourdes commes les imges : 128-Mbit Quad-SPI Flash memory

Fonctionnalitees addiotionnelles petinentes :
* Gestion Audio intégrée : Audio line in and line out jack & Stereo speaker outputs


### Driver de vérin

Les drivers des verrins permettent de faire l'interface entre les ordres transmis par la centrale et l'action entrepris par les verrins. Il est compose des elements suivants :


### Interface de Communication (Bus RS-485)

L'interface de communication est une carte électronique intermédiaire positionnée entre l'unité centrale et les différents actionneurs (vérins).

Pour surmonter la contrainte de la distance physique et garantir une transmission rapide et immunisée contre les parasites, la liaison s'appuie sur la norme RS-485. Ce standard utilise une signalisation différentielle particulièrement fiable en milieu industriel.

L'interface assure la conversion des signaux : elle transforme le protocole UART (TTL) de la centrale en signaux différentiels pour le bus, puis les drivers des vérins effectuent la conversion inverse pour traiter l'information. L'ensemble des drivers est raccordé en parallèle selon une topologie en bus, permettant une gestion simplifiée du câblage.

## Environnement de Développement
Le développement du projet repose sur une chaîne d'outils intégrée, permettant de gérer aussi bien la configuration matérielle que l'interface utilisateur et le versionnage du code.

`STM32CubeIDE` <br>
Il s'agit de l'environnement de développement (IDE) "tout-en-un" fourni par STMicroelectronics.

* Configuration : Intègre STM32CubeMX pour configurer graphiquement les périphériques (UART, RS-485, Horloges) et générer le code d'initialisation.

* Développement & Debug : Permet l'écriture du code en C/C++ et offre des outils de débogage avancés (lecture de registres en temps réel, analyseur de variables).

* Compilation : Utilise le compilateur GCC pour optimiser les performances du processeur Cortex-M7.

`TouchGFX Designer` <br>
Framework graphique avancé spécifiquement optimisé pour les microcontrôleurs STM32.

* Design UI : Permet de concevoir l'interface de manière visuelle pour ensuite developper en C++ dessus.

* Performance : Tire parti de l'accélérateur matériel Chrom-ART de la carte pour obtenir des animations fluides (60 FPS) sans surcharger le processeur.

* Synchronisation RTOS : S'intègre avec FreeRTOS pour séparer le thread de rendu graphique des threads critiques de communication et de calcul (Backend/Frontend).

`GitHub` <br>
Plateforme d'hébergement et de gestion de versions basée sur l'outil Git.

* Collaboration : Centralise le code source pour permettre à plusieurs développeurs de travailler simultanément sur différents modules (centrale, drivers) via un système de branches.

* Documentation : Héberge le présent fichier README et la documentation technique associée pour une meilleure visibilité du projet.


# Interface Graphique
L'interface graphique (IHM) permet de visualiser en temps réel l'état du système de simulation. Elle offre à l'utilisateur une plateforme interactive pour piloter le simulateur, lancer les procédures de calibration et ajuster les paramètres de conduite.

## TouchGFX Designer
TouchGFX Designer est l'outil utilisé pour concevoir l'environnement visuel. Il génère un code C++ optimisé qui tire parti des capacités d'accélération matérielle du STM32.

### Creation du projet
La première étape consiste à sélectionner le BSP (Board Support Package) correspondant à notre matériel : le `STM32F746G Discovery Kit`.
{photo}

[!IMPORTANT] Cette étape est cruciale car TouchGFX génère non seulement l'interface, mais aussi toute la structure du projet (fichiers de configuration, drivers d'écran, gestion de la mémoire) que nous importerons ensuite dans STM32CubeIDE.

### Architecture de l'interface
L'interface est découpée en plusieurs Écrans (Screens). Chaque écran possède son propre fichier de vue (View) et son présentateur (Presenter), permettant de séparer la logique graphique de la logique métier.

#### Ecran
Les vues du simulateur
Pour garantir une navigation fluide, nous avons segmenté l'IHM en quatre écrans principaux :

* SimHubScreen : Le tableau de bord principal. Il affiche les données télémétriques en temps réel, telles que l'extension des vérins et la consommation électrique des drivers.

* LoadingScreen : Une interface de transition utilisée lors des processus lourds, typiquement durant la phase de calibration automatique des vérins.

* MenuScreen : Le point d'entrée central permettant à l'utilisateur de naviguer entre les différentes sections.

* SettingScreen : Un espace dédié à la configuration logicielle et matérielle du système.

{photo}

Avoir une multitude d'ecran permet de faciliter le developpement des differents UI. 

#### Composants et Outils
L'IDE propose deux types de blocs de construction pour concevoir les écrans :

1. Les Widgets : Ce sont les éléments de base (boutons, zones de texte, jauges, formes géométriques). Ils sont entièrement personnalisables pour correspondre à la charte graphique du simulateur.

2. Les Conteneurs (Containers) : Ils permettent de regrouper plusieurs widgets au sein d'une même entité. Cela facilite la création de fenêtres modales, de listes déroulantes ou de menus complexes.

Vous pouvez retrouver ci-dessous la documentation sur les outils et conteneurs pour en apprendre d'avantages [Contenuer/Outil](https://support.touchgfx.com/docs/development/ui-development/working-with-touchgfx/widgets-and-containers)


### Generer le projet
Une fois le design finalisé, l'outil génère le code source. Ce processus inclut :

* La conversion des images en tableaux de données stockés en Flash QSPI.

* L'implémentation de l'API graphique compatible avec FreeRTOS.

* La configuration des périphériques de la carte (écran, tactile, DMA2D).

## Backend

## Frontend

# Protocole de Communication

## Systeme de commmunication BUS 

## Protocole de communication



# Gestion des Tâches (RTOS)


# Resources 
Documentation de la carte STM32F746G [link1](https://www.st.com/en/evaluation-tools/32f746gdiscovery.html#st_description_sec-nav-tab)
Documentation de l'API graphique TouchGFX [link2](https://support.touchgfx.com/docs/introduction/welcome)
Documentation RTOS sur la synchronization de threads [link3](https://arm-software.github.io/CMSIS_5/RTOS/html/cmsis__os_8h.html)
