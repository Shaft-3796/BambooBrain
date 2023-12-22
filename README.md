![La bannière de bamboo brain](bamboo_brain_banner.png)

## Installation

### Dependences
Installez la librairie SDL sur votre plateforme. Sur Ubuntu:
```bash
sudo apt-get install libsdl2-dev
sudo apt-get install libsdl2-ttf-dev
sudo apt-get install libsdl2-image-dev
```
#### Makefile build
Soon...


## Fonctionnalités principales
### 🌳 Arbre de décision fonctionnel
  - Random forest
  - Différents modes de calcul de threshold
    - Moyenne `(min+max)/2`
    - Tester toutes les valeurs de features avec un pas
  - Calcul du score sigmoid
  - Différents modes de baggings
- Sauvegarde / chargement d'un arbre (fichiers dans [`datasets/*.bb`](datasets))
### ⚙️ Preprocessing des instances
  - Suppression des nuances de gris
  - Recentrage des images
### 📊 Interface utilisateur
  - Prediction en temps réel des dessins de chiffres
  - Pourcentage de probabilité pour chaque chiffre

### 📈 Optimisation des hyperparamètres
- Variation des différents hyperparamètres
- Exportation pour visualiser les scores en fonction des hyperparamètres


## À savoir
Tout est facilement configurable à travers la structure `Config`, vous pouvez retrouver le détail de chaque paramètre dans le fichier [`src/Config`](src/Config.h).