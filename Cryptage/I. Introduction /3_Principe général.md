# Principe général : Substitutions et Transpositions

Toute la cryptographie classique repose sur deux opérations fondamentales.

---

## 1. La Substitution

On **remplace** chaque lettre (ou caractère) du message par un autre caractère selon une règle définie par la clé.

**Exemple simple — décalage de 3 (César) :**

```
Alphabet clair  :  A B C D E F G H I J K L M N O P Q R S T U V W X Y Z
Alphabet codé   :  D E F G H I J K L M N O P Q R S T U V W X Y Z A B C
```

```
P = PIE
C = SLH
```

> La **structure** du message change — les lettres sont remplacées, mais leur **position** reste la même.

---
<img width="667" height="189" alt="image" src="https://github.com/user-attachments/assets/4a9786eb-72e0-4305-91ea-6b6058aa2dbf" />
<img width="667" height="189" alt="image" src="https://github.com/user-attachments/assets/feee36b8-c9fb-4595-b032-00af34f516f4" />

<img width="680" height="281" alt="image" src="https://github.com/user-attachments/assets/1323bebe-90db-4fc2-880a-9b465f9d7a5c" />

## 2. La Transposition

On **déplace** les lettres du message sans les remplacer — on réorganise leur ordre selon une permutation.

**Exemple :**

```
P = PIE
```

Permutations possibles :

```
PEI  /  EIP  /  EPI  /  IEP  /  IPE
```

> Les **mêmes lettres** sont présentes, mais dans un **ordre différent**.

---

## 3. Différence clé entre les deux

| | Substitution | Transposition |
|---|---|---|
| **Principe** | Remplace les lettres | Déplace les lettres |
| **Lettres originales** | ❌ Changées | ✅ Conservées |
| **Position originale** | ✅ Conservée | ❌ Changée |
| **Exemple** | César, Vigenère | Scytale, permutation de blocs |

---

## 4. En pratique

Les algorithmes modernes comme **DES** combinent les deux opérations plusieurs fois de suite :

```
Texte clair
    ↓
Substitution  →  brouille les lettres
    ↓
Transposition →  brouille les positions
    ↓
Substitution  →  encore...
    ↓
    ...  (16 tours pour DES)
    ↓
Texte chiffré  ✅
```

> 💡 **Plus on combine et répète ces deux opérations, plus le chiffrement est difficile à casser.**
