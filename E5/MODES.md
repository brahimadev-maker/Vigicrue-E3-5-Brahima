# E5 · Modes d'émission

Branche : `emulateur-modes`

| Fichier | Rôle |
|---------|------|
| `emulateur/include/modes.h` | déclarations, réglages, état |
| `emulateur/src/modes.cpp` | les 3 versions de `doit_emettre` |
| `emulateur/tests/test_modes.cpp` | banc d'essai autonome sur un CSV E3 |

`mesure.h` n'est **pas** fourni ici : on utilise celui de l'ossature commune.

## Utilisation dans l'émulateur

```cpp
#include "modes.h"

EtatMode etat;
Mode mode;
if (!mode_depuis_texte(argv[1], mode)) { /* "naif", "fixe" ou "s4" */ }
init_etat(etat, mode);              // mode choisi au démarrage

// pour chaque Mesure m produite par le rejeu (E4) :
if (doit_emettre(m, etat)) {
    // envoyer la trame (m.hauteur_mm = 0xFFFF => trame "capteur muet")
}
```

## Les 3 modes

**Naïf** : émet chaque mesure valide (une trame toutes les 5 min).

**Fréquence fixe** : une trame toutes les `periode_fixe_s` (1 h par défaut).
Si l'instant prévu tombe sur une mesure invalide, la mesure valide suivante est émise.

**S4 hybride** : périodique lent + événements. Émet si :
- c'est la première mesure ;
- la hauteur a varié d'au moins `s4_delta_mm` (50 mm) depuis le dernier envoi ;
- on entre ou sort d'alerte (seuil 1500 mm, hystérésis 50 mm) ;
- la période est écoulée : 1 h hors alerte, 15 min en alerte ;
- le capteur revient après une panne.
Si le capteur est muet depuis 30 min, une seule trame invalide (0xFFFF) est émise
pour le signaler.

> **Hypothèse à valider avec l'équipe** : la définition exacte de « S4 hybride »
> n'était pas fournie. La logique ci-dessus (périodique + seuil + variation)
> est une proposition ; les réglages sont dans `ParamsModes`.

## Mesures invalides

Les modes naïf et fixe les ignorent. S4 les ignore aussi, sauf pour signaler
une fois un capteur muet.

## Test

```bash
cd emulateur
g++ -std=c++11 -Iinclude src/modes.cpp tests/test_modes.cpp -o test_modes
./test_modes s4 ../../E3/scenario_00b_SIMULE_trous.csv -v
```

Résultat sur `scenario_00b_SIMULE_trous.csv` (288 mesures, 16 invalides) :

| Mode | Émissions | Part |
|------|-----------|------|
| naïf | 272 | 94,4 % |
| fixe | 23 | 8,0 % |
| s4   | 52 | 18,1 % |
