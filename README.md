# 🌿 The Garden - Projet Synthèse d'images

![C++](https://img.shields.io/badge/C++-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![OpenGL](https://img.shields.io/badge/OpenGL-FFFFFF?style=for-the-badge&logo=opengl&logoColor=5586A4)
![CMake](https://img.shields.io/badge/CMake-064F8C?style=for-the-badge&logo=cmake&logoColor=white)

**The Garden** est un environnement virtuel interactif en 3D développé en C++ avec OpenGL. Ce projet simule un écosystème dynamique comprenant un terrain généré procéduralement, une forêt, un moulin animé et une faune adaptative.

## ✨ Fonctionnalités

- **Génération de Terrain :** Relief dynamique créé à partir d'une *HeightMap* (`.pgm`) avec lissage automatique et calcul des normales pour un éclairage réaliste.
- **Forêt Procédurale :** Placement automatique d'arbres sur les zones planes identifiées sur la carte.
- **Faune Animée & Adaptative :**
    - **Oiseau :** Vol sinusoïdal fluide avec animation articulée des ailes.
    - **Moutons :** IA de déplacement aléatoire capable de lire le relief en temps réel pour rester parfaitement au contact du sol.
- **Système d'Éclairage :** - Support des modes **Flat Shading** et **Blinn-Phong**.
    - Lumière directionnelle globale (Soleil) et lumière ponctuelle mobile attachée à l'animal volant.
- **Optimisation VRAM :** Architecture Orientée Objet avec utilisation de pointeurs statiques pour le partage des maillages géométriques, permettant d'afficher une forêt dense sans perte de performance.

## 🛠️ Installation et Compilation

### Prérequis
- Un compilateur C++ (supportant C++11 ou supérieur)
- CMake
- Librairies OpenGL (GLFW, GLEW)

### Compilation
Exécutez les commandes suivantes à la racine du projet :

```bash
mkdir build
cd build
cmake ..
make
```

### 4.2. Exécution du programme

Une fois la compilation terminée, l'exécutable est généré dans le dossier `bin`. Pour que les chemins relatifs vers les textures et les shaders soient valides, vous devez vous placer dans le répertoire `bin` avant de lancer l'application :

```bash
cd bin
./GardenProject_main ../assets/textures/terrain.pgm
```

### 🎮 Commandes Utilisateur

| Touche | Action |
| :--- | :--- |
| **Z / S** | Avancer / Reculer (Caméra FPS) |
| **Q / D** | Déplacement latéral gauche / droit |
| **Haut / Bas** | Monter / Descendre en altitude (Axe Z) |
| **Gauche / Droite** | Pivoter la vue (Rotation 360°) |
| **F** | Alterner entre Flat Shading et Blinn-Phong |
| **L** | Activer le mode Fil de fer (Wireframe) |
| **P** | Revenir au mode Plein (Fill) |
| **ESC** | Quitter l'application |

### 📂 Structure du Projet

* **`/src`** : Classes C++ (Entités, Moteur, Gestion du terrain).
* **`/assets`** : Textures (herbe, pierre, bois) et Shaders GLSL.
* **`/bin`** : Répertoire de sortie des exécutables.

### 👥 Auteurs

* Projet réalisé par **Ryad Meghachi** et **Lucas Benesby**.
* **GitHub** : [https://github.com/RedCodeSnoo/TheGarden](https://github.com/RedCodeSnoo/TheGarden)