# III. Codage par Substitution

Le codage par substitution consiste à **remplacer** chaque lettre du texte en clair par un autre caractère selon une règle définie par une clé secrète.

---

## 1. Principe général

```
Texte clair  :  A B C D E F G ...
Texte codé   :  D E F G H I J ...
```

---

## 2. Code de César

Le plus ancien et le plus simple. On **décale** chaque lettre de l'alphabet d'un nombre fixe de positions (la clé).

**Exemple avec décalage de 3 :**

```
Clair  : A B C D E F G H I J K L M N O P Q R S T U V W X Y Z
Chiffré: D E F G H I J K L M N O P Q R S T U V W X Y Z A B C
```

```
P = PIE   →   C = SLH
```

**Formule mathématique :**

```
Chiffrement : C = (P + k) mod 26
Déchiffrement: P = (C - k) mod 26
```

---

## 3. Code de Marie Stuart (1586)

**Contexte historique :**
Marie Stuart, reine d'Écosse, fut jugée en Angleterre en 1586, accusée de complot pour assassiner la reine Elizabeth. Elle utilisait un code secret pour communiquer avec ses complices depuis sa prison. Sir Francis Walsingham intercepta ses lettres et fit appel à Thomas Phelippes pour les déchiffrer — ce qui scella son destin.

**Structure du code :**

- 23 symboles remplaçant les lettres de l'alphabet (sauf j, v, w)
- 36 symboles représentant des mots ou phrases entiers
- 4 nulles (symboles sans signification pour tromper)
- 1 symbole signifiant "la lettre suivante est doublée"

> 💡 Ce code est une **substitution par symboles** — chaque lettre est remplacée par un symbole unique.

---

## 4. Code de Vigenère

Plus sophistiqué que César — la clé est un **mot entier**, et chaque lettre du message est décalée d'une valeur différente.

**Le carré de Vigenère :**

```
     A B C D E F G H I J K L M N O P Q R S T U V W X Y Z
  A: A B C D E F G H I J K L M N O P Q R S T U V W X Y Z
  B: B C D E F G H I J K L M N O P Q R S T U V W X Y Z A
  C: C D E F G H I J K L M N O P Q R S T U V W X Y Z A B
  M: M N O P Q R S T U V W X Y Z A B C D E F G H I J K L
  U: U V W X Y Z A B C D E F G H I J K L M N O P Q R S T
  S: S T U V W X Y Z A B C D E F G H I J K L M N O P Q R
```

**Exemple du cours :**

```
Clé     : M U S I Q U E M U S I Q U E M U S I Q U
Message : J A D O R E E C O U T E R L A R A D I O
Chiffré : V U V W H Y I O I M B U L P M L S L Y I
```

**Formule mathématique :**

```
Chiffrement : C = (P + K) mod 26
Déchiffrement: P = (C - K) mod 26
```

**Déchiffrement :**
On utilise la même clé et l'opération inverse dans le carré — on cherche la lettre chiffrée dans la ligne de la clé, et on remonte à la colonne pour trouver la lettre en clair.

---

## 5. Substitution par alphabets (clé secrète Ens2)

```
Ens1 : A B C D E F G H I J K L M N O P Q R S T U V W X Y Z
Ens2 : C B K R Y A D M S Z L E N T I F O V G H P W U J Q X
```

**Chiffrement :**

```
P = PIE
P → F
I → S
E → Y
C = FSY
```

**Nombre de clés possibles :** 26! ≈ 4 × 10²⁶ — impossible à casser par force brute !

> 💡 Ens2 est la **clé secrète**. Sans la connaître, personne ne peut déchiffrer.

---

## 6. Codes homophones (matrice 1D)

Un code homophone permet de remplacer **une lettre par plusieurs codes différents** pour éviter l'analyse de fréquence.

**Matrice exemple :**

```
     0  1  2  3  4  5  6  7  8  9
  0: T  H  E  P  D  A  Q  T  I  L
  1: E  H  E  T  S  R  O  E  N  D
  2: S  U  C  Y  H  O  A  I  A  S
  3: E  O  M  B  N  I  L  P  N  R
```

**Exemple :** La lettre **H** a plusieurs homophones : `01, 11, 24, 50, 62`

```
Message : H  E  L  L  O
Codage  : 01 10 09 36 16
```

> 💡 Même si l'adversaire voit `01`, `11`, `24`... il ne sait pas que c'est toujours la même lettre H !

**Exercices :**
- Donner la clé de chiffrement
- Donner des codes possibles de PIE
- Décoder `73461718` et `33084956`

---

## 7. Codes homophones bidimensionnels (tableau 26×26)

Version avancée — chaque lettre a **26 homophones** dans un tableau 26×26 rempli de nombres de 1 à 676.

**Principe de codage :**
- On peut utiliser n'importe quel nombre de la ligne ou de la colonne de la lettre
- On alterne : colonne → ligne → colonne → ligne...

```
Exemple : coder T-O-U-T
T → colonne T → 072
O → ligne O   → 050
U → colonne U → 014
T → ligne T   → 084
```

**Exercices :**
- Traduire `108070020083`
- Donner la clé de chiffrement
- Coder le mot TOUT

---

## Récapitulatif

| Code | Clé | Sécurité | Principe |
|------|-----|----------|----------|
| **César** | 1 nombre (1–25) | ❌ Très faible | Décalage fixe |
| **Marie Stuart** | Tableau de symboles | ⚠️ Moyenne | Symboles + nulles |
| **Vigenère** | 1 mot | ✅ Bonne | Décalage variable |
| **Alphabets (Ens2)** | 1 alphabet (26!) | ✅✅ Forte | Substitution aléatoire |
| **Homophones 1D** | Matrice | ✅✅ Forte | Plusieurs codes / lettre |
| **Homophones 2D** | Tableau 26×26 | ✅✅✅ Très forte | 26 codes / lettre |

> 💡 **Faiblesse commune** : toutes ces méthodes restent vulnérables à l'**analyse de fréquence** — certaines lettres (E, A, S...) apparaissent plus souvent et trahissent le code.
