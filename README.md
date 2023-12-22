![La bannière de bamboo brain](bamboo_brain_banner.png)

## Installation

Installez la librairie SDL sur votre plateforme. Sur Ubuntu:
```bash
sudo apt-get install libsdl2-dev
sudo apt-get install libsdl2-ttf-dev
sudo apt-get install libsdl2-image-dev
```
#### Compilation
Compiler avec gcc:
```bash
gcc -o bamboo src/*.c src/dynamic/*.c -I/usr/include/SDL2/ -lSDL2 -lSDL2_image -lSDL2_ttf -lm
``` 
#### Lancement
```bash
./bamboo
```

## Fonctionnalités

### Modèles
- Model à un seul arbre
- Model random forest

### Calcul du split
- Mesure d'impureté de Gini
- Threshold moyen
- Tests de tout les thresholds possibles

### Bagging
- Bagging d'instances selon une proportion des instances du dataset
- Bagging de features selon une proportion des features

### Predictions
- Prédiction classique par parcours de l'arbre
- Prédiction par vote majoritaire des arbres pour la random forest
- Prédiction et probabilités avec les scores de la fonction sigmoid

### Pre Processing
- Ajout d'éléments de pre processing au runtime
- - Noir et blanc
- - Centrage

### UI
- Interface de reconaissance de nombre dessinés
- Barres de chargement et temps restant estimé

### Hyperopt
- Optimisation de paramètres 2 à 2, permet de trouver les meilleurs combinaisons

### Sauvegarde
- Sauvegarde des modèles en format ultra light (binaire)
- Chargement des modèles au runtime

## Utilisation

### Choix du mode

Dans la fonction main à la fin du main.c:
```c
// Uncomment only one of the following lines
user_interface();  // Run the user interface
// evaluation();  // Run the creating/evaluation of a model
// hyper_opt();  // Run the hyper optimizer
```
Décommentez une ligne pour choisir le mode:
- user_interface(): Interface utilisateur pour la reconnaissance de chiffres
- evaluation(): Création et évaluation d'un modèle
- hyper_opt(): Optimisation des paramètres 2 à 2

### Configuration
Les éléments de configuration sont donnés au début de chaque fonctions.
Pensez à modifier les différents chemins vers les datasets et les modèles.
La structure Config permet de modifier les paramètres de l'algorithme, suivez les commentaires pour savoir à quoi ils 
correspondent.
