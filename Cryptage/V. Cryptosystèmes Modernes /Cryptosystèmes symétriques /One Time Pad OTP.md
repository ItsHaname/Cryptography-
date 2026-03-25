# V. Cryptosystèmes Modernes — Symétriques

---

## 1. Principe des cryptosystèmes symétriques

Dans un cryptosystème symétrique, **la même clé** est utilisée pour chiffrer et déchiffrer.

```
Alice                          Bob
  |                              |
  P → [Ek] → C ──réseau──► C → [Dk] → P
        ↑                        ↑
      clé K                    clé K
      (même clé des deux côtés)
```

> 💡 Aussi appelés **cryptosystèmes à clé secrète** ou **conventionnels**.

---

## 2. One Time Pad (OTP / Vernam, 1917)

Inventé par **Gilbert Vernam** en 1917, l'OTP est le seul système **inconditionnellement sûr** — mathématiquement impossible à casser si bien utilisé.

### Principe — XOR bit à bit

```
Chiffrement : C = M ⊕ K
Déchiffrement: M = C ⊕ K
```

### Exemple

```
M = 1 1 0 0 1 0 1 0 1 1 0
K = 1 1 1 1 0 1 0 0 1 0 0
    ─────────────────────
C = 0 0 1 1 1 1 1 0 0 1 0

Déchiffrement :
C = 0 0 1 1 1 1 1 0 0 1 0
K = 1 1 1 1 0 1 0 0 1 0 0
    ─────────────────────
M = 1 1 0 0 1 0 1 0 1 1 0  ✅
```

### Table XOR

```
0 ⊕ 0 = 0
0 ⊕ 1 = 1
1 ⊕ 0 = 1
1 ⊕ 1 = 0
```

### Règles d'or de l'OTP

- La clé doit être **aussi longue** que le message
- La clé doit être **parfaitement aléatoire**
- La clé ne doit être utilisée qu'**une seule fois**

> ⚠️ Si la clé est réutilisée, le système devient cassable !

---

## 3. DES — Data Encryption Standard (IBM, 1977)

### Vue d'ensemble

```
Texte clair (64 bits)
        ↓
  Permutation initiale PI
        ↓
  ┌─────────────────┐
  │  16 itérations  │  ← chaque tour utilise une sous-clé Ki
  └─────────────────┘
        ↓
  Permutation inverse PI⁻¹
        ↓
Texte chiffré (64 bits)
```

**Caractéristiques :**
- Bloc de **64 bits**
- Clé de **56 bits** (64 bits dont 8 bits de parité ignorés)
- **16 tours** de chiffrement

---

### Étape 1 — Permutation initiale PI

```
58  50  42  34  26  18  10   2
60  52  44  36  28  20  12   4
62  54  46  38  30  22  14   6
64  56  48  40  32  24  16   8
57  49  41  33  25  17   9   1
59  51  43  35  27  19  11   3
61  53  45  37  29  21  13   5
63  55  47  39  31  23  15   7
```

---

### Étape 2 — Division en G et D

```
G0 = 32 bits gauche
D0 = 32 bits droite
```

---

### Étape 3 — Table d'expansion E (32 → 48 bits)

```
32   1   2   3   4   5
 4   5   6   7   8   9
 8   9  10  11  12  13
12  13  14  15  16  17
16  17  18  19  20  21
20  21  22  23  24  25
24  25  26  27  28  29
28  29  30  31  32   1
```

---

### Étape 4 — XOR avec la sous-clé Ki

```
D0' = E[D0] ⊕ K1   (48 bits ⊕ 48 bits)
```

---

### Étape 5 — Fonctions de substitution S1 à S8

```
D0' (48 bits) → 8 blocs de 6 bits
                      ↓
              S1  S2  S3  S4  S5  S6  S7  S8
                      ↓
              8 blocs de 4 bits = 32 bits
```

**Comment lire une S-box :**
```
Bloc = 001011
  → bits 1 et 6 = 01 → ligne 1
  → bits 2 à 5  = 0101 = 5 → colonne 5
  → valeur = S[1][5]
```

---

### Étape 6 — Permutation P

```
16   7  20  21
29  12  28  17
 1  15  23  26
 5  18  31  10
 2   8  24  14
32  27   3   9
19  13  30   6
22  11   4  25
```

---

### Étape 7 — XOR avec G0 et échange

```
D1 = G0 ⊕ F[K1, D0]
G1 = D0
→ répéter 16 fois
```

---

### Étape 8 — Permutation inverse PI⁻¹

```
40   8  48  16  56  24  64  32
39   7  47  15  55  23  63  31
38   6  46  14  54  22  62  30
37   5  45  13  53  21  61  29
36   4  44  12  52  20  60  28
35   3  43  11  51  19  59  27
34   2  42  10  50  18  58  26
33   1  41   9  49  17  57  25
```

---

### Génération des sous-clés (CP1 et CP2)

**CP1** — réduit la clé de 64 à 56 bits :

```
57  49  41  33  25  17   9
 1  58  50  42  34  26  18
10   2  59  51  43  35  27
19  11   3  60  52   4  36
63  55  47  39  31  23  15
 7  62  54  46  38  30  22
14   6  61  53  45  37  29
21  13   5  28  20  12   4
```

**CP2** — extrait 48 bits pour former Ki :

```
14  17  11  24   1   5
 3  28  15   6  21  10
23  19  12   4  26   8
16   7  27  20  13   2
41  52  31  37  47  55
30  40  51  45  33  48
44  49  39  56  34  53
46  42  50  36  29  32
```

---

### Déchiffrement DES

> Le déchiffrement est **identique** au chiffrement, sauf que les sous-clés sont utilisées dans l'ordre **inverse** : K16 → K15 → ... → K1

---

### Cryptanalyse du DES

| Attaque | Découverte par | Principe | Complexité |
|---------|---------------|----------|------------|
| **Force brute** | — | Tester les 2⁵⁶ clés | 228 ans à 10M clés/sec |
| **Différentielle** | Biham & Shamir (1991) | Analyser différences entre textes | 2⁴⁷ textes en clair |
| **Linéaire** | Matsui (1993) | Approximation linéaire de DES | 2⁴³ couples |
| **Tickling attack** | IBM (années 70) | Variante différentielle | Renforcé les S-boxes |

---

## 4. Triple DES (3DES)

**Mode EDE (Encrypt – Decrypt – Encrypt) :**

```
Message M
    ↓
  E(K1)   ← chiffrement avec clé K1
    ↓
  D(K2)   ← déchiffrement avec clé K2
    ↓
  E(K1)   ← chiffrement avec clé K1
    ↓
Chiffré C

Formule : C = E_K1( D_K2( E_K1(M) ) )
```

**Taille de clé effective :**
- 2 clés → 112 bits
- 3 clés → 168 bits

> 💡 Si K1 = K2 → comportement identique au DES simple (rétrocompatible)

---

## 5. Distribution de clés

### Le problème

```
N utilisateurs → N(N-1)/2 clés nécessaires
100 users      → 4 950 clés !
```

### Protocole Needham-Schroeder

Introduit un **KDC (Key Distribution Center)** :

```
P, V, np
    ↓ (P demande une clé pour parler à V)
KDC
    ↓ { np, V, Kss, {Kss, P}Kv }Kp
P reçoit la clé de session Kss
    ↓ { Kss, P }Kv  (ticket pour V)
V reçoit le ticket
    ↓ nv (challenge)
P répond { f(nv) }Kss
```

- **Kss** = clé de session temporaire
- **Kp**  = clé secrète de P avec le KDC
- **Kv**  = clé secrète de V avec le KDC

### Schéma Kerberos

Implémentation du protocole Needham-Schroeder, développée au MIT, utilisée dans **Windows Active Directory**.

```
Client
  │── 1. Demande de ticket ──►  KDC (AS)
  │◄─ 2. Ticket TGT ───────────────────
  │── 3. Demande service ──►  KDC (TGS)
  │◄─ 4. Ticket de service ────────────
  │── 5. Accès avec ticket ──►  Serveur
  │◄─ 6. Confirmation ─────────────────
```

---

## Récapitulatif

| Algorithme | Clé | Bloc | Sécurité |
|-----------|-----|------|----------|
| **OTP** | = longueur message | — | ✅✅✅ Parfaite (si bien utilisé) |
| **DES** | 56 bits | 64 bits | ❌ Cassé (obsolète) |
| **3DES** | 112 ou 168 bits | 64 bits | ✅ Acceptable mais lent |

> 💡 DES est aujourd'hui **obsolète** — remplacé par **AES** depuis 2001, avec des clés de 128, 192 ou 256 bits.
