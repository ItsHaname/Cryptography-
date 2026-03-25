# IV. Codage par Transposition

La transposition ne **remplace pas** les lettres — elle les **déplace**. On réorganise l'ordre des lettres selon une permutation définie par la clé.

---

## 1. Principe

```
P = PIE
Permutations possibles : PEI / EIP / EPI / IEP / IPE
```

> 💡 Les mêmes lettres sont là, mais dans un ordre différent. Contrairement à la substitution, un adversaire sait quelles lettres sont utilisées — mais pas dans quel ordre !

---

## 2. Transposition unidimensionnelle

On place les lettres dans un tableau 1D et on les permute selon une règle.

**Exemple avec "jean" :**

```
Position : 1  2  3  4
Lettre   : j  e  a  n
```

Permutation appliquée :

```
| 1 | 2 | 3 | 4 |
| 3 | 1 | 4 | 2 |
```

- Position 1 → va en 3
- Position 2 → va en 1
- Position 3 → va en 4
- Position 4 → va en 2

```
Original : j  e  a  n
Chiffré  : e  n  j  a
```

---

## 3. Chiffrement et Déchiffrement par permutation

**Chiffrement :**

```
Permutation :
| 1 | 2 | 3 | 4 |
| 3 | 1 | 4 | 2 |

jean → enaj
```

**Déchiffrement — permutation inverse :**

```
Permutation directe  :
| 1 | 2 | 3 | 4 |
| 3 | 1 | 4 | 2 |

Permutation inverse  :
| 1 | 2 | 3 | 4 |
| 2 | 4 | 1 | 3 |
```

> 💡 La permutation inverse remet chaque lettre à sa place d'origine.

---

## 4. Permutation de blocs

En pratique, on découpe le texte en **blocs de taille fixe** puis on permute chaque bloc.

**Exemple — "ENVAHIR NORMANDIE PAS CALAIS" :**

Découpage en blocs de 5 lettres :

```
ENVAH  IRNOR  MANDI  EPASC  ALAIS
```

Permutation appliquée à chaque bloc :

```
10  12   5  18   6
 9  21   1  13  23
19   2  25   4  16
15   3  22  11   7
20  14  24  17   8
```

Résultat après permutation et concaténation :

```
NAPDV HCSIE SNOLE IIAMA RARAN
→ NAPDVHCSIESNOLEIIAMARARАН
```

Déchiffrement avec la permutation inverse :

```
NAPDVHCSIESNOLEIIAMARARАН
→ ENVAHIRnormandiepascalais ✅
```

> 💡 On concatène les blocs sans espaces pour **cacher leur taille** et compliquer le déchiffrement.

---

## 5. La Scytale (Antiquité grecque)

La **scytale** est l'un des premiers outils cryptographiques de l'histoire, utilisé par les Spartiates.

**Comment ça marche :**

1. On enroule une bande de cuir en spirale autour d'un bâton de diamètre précis
2. On écrit le message **en lignes horizontales** le long du bâton
3. On déroule la bande — les lettres sont dans le désordre
4. Le destinataire enroule la bande sur un bâton **de même diamètre** pour lire

```
Bâton (diamètre = clé) :

  E N V A H
  I R N O R      ← on écrit sur le bâton
  M A N D I
  E P A S C
  A L A I S

Bande déroulée : EIMEA NRPAL VNNAI AODIS HRICS  ← illisible !

Même bâton chez le destinataire → message retrouvé ✅
```

> 💡 La clé est le **diamètre du bâton**. Sans bâton de même taille, impossible de lire.

---

## 6. Transposition par mot-clé

On utilise un mot mémorisable pour générer l'alphabet de codage.

**Exemple avec CALIGULA :**

Étape 1 — Écrire les lettres du mot-clé sans répétition :

```
C  A  L  I  G  U
```

Étape 2 — Compléter avec les lettres manquantes ligne par ligne :

```
| C | A | L | I | G | U |
| B | D | E | F | H | J |
| K | M | N | O | P | Q |
| R | S | T | V | W | X |
| Y | Z |   |   |   |   |
```

Étape 3 — Lire les colonnes pour obtenir l'alphabet codé.

> 💡 Le mot-clé **CALIGULA** suffit à reconstruire tout l'alphabet — facile à mémoriser, impossible à deviner !

---

## Récapitulatif

| Méthode | Clé | Principe |
|---------|-----|----------|
| **Unidimensionnelle** | Permutation numérique | Déplace les lettres d'un mot |
| **Permutation de blocs** | Matrice de permutation | Découpe en blocs + permute |
| **Scytale** | Diamètre du bâton | Enroulement sur bâton |
| **Mot-clé** | Un mot (ex. CALIGULA) | Génère un alphabet par colonnes |

> 💡 **Différence fondamentale avec la substitution :**
> - Substitution → change **quelles** lettres
> - Transposition → change **où** sont les lettres
