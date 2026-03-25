# DES — Data Encryption Standard (IBM, 1977)

---

## Vue d'ensemble

DES prend un **bloc de 64 bits** et le fait passer par **16 tours** identiques, chacun utilisant une sous-clé différente de 48 bits extraite de la clé principale de 56 bits.

```
Texte clair (64 bits)
         ↓
   Permutation initiale PI
         ↓
   Division → G0 (32 bits) | D0 (32 bits)
         ↓
   Tour 1  : D1 = G0 ⊕ F(D0, K1)  ;  G1 = D0
   Tour 2  : D2 = G1 ⊕ F(D1, K2)  ;  G2 = D1
   ...
   Tour 16 : D16 = G15 ⊕ F(D15, K16)
         ↓
   Permutation inverse PI⁻¹
         ↓
Texte chiffré (64 bits)
```

**Caractéristiques :**
- Bloc de **64 bits**
- Clé de **56 bits** (64 bits dont 8 bits de parité ignorés)
- **16 tours** de chiffrement (schéma de Feistel)

---

## Étape 1 — Permutation Initiale PI

Les 64 bits sont réordonnés selon la table fixe PI.

```
Table PI :
58 50 42 34 26 18 10  2
60 52 44 36 28 20 12  4
62 54 46 38 30 22 14  6
64 56 48 40 32 24 16  8
57 49 41 33 25 17  9  1
59 51 43 35 27 19 11  3
61 53 45 37 29 21 13  5
63 55 47 39 31 23 15  7
```

> 💡 Cette permutation n'apporte pas de sécurité — elle optimise l'implémentation matérielle.

---

## Étape 2 — Division G0 et D0

```
64 bits → G0 (32 bits gauche) | D0 (32 bits droite)
```

---

## Étape 3 — Un tour DES (schéma de Feistel)

```
      Gi-1              Di-1
       │                  │
       │           ┌──────┴──────┐
       │           │   E(Di-1)   │  32 → 48 bits
       │           └──────┬──────┘
       │                  │ ⊕ Ki (48 bits)
       │           ┌──────┴──────┐
       │           │  S1...S8    │  48 → 32 bits
       │           └──────┬──────┘
       │           ┌──────┴──────┐
       │           │      P      │  permutation 32 bits
       │           └──────┬──────┘
  ┌────┴────┐             │
  │   XOR   │◄────────────┘
  └────┬────┘
       │
  Di = Gi-1 ⊕ F(Di-1, Ki)
  Gi = Di-1
```

---

## Étape 4 — Table d'Expansion E (32 → 48 bits)

Certains bits sont dupliqués pour passer de 32 à 48 bits.

```
Table E :
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

## Étape 5 — XOR avec la sous-clé Ki

```
Résultat = E[D] ⊕ Ki     (48 bits ⊕ 48 bits = 48 bits)
```

---

## Étape 6 — Les 8 boîtes S (S-boxes)

C'est le **cœur de la sécurité de DES** — la seule partie non linéaire.

```
48 bits → [6][6][6][6][6][6][6][6]
              ↓  ↓  ↓  ↓  ↓  ↓  ↓  ↓
             S1 S2 S3 S4 S5 S6 S7 S8
              ↓  ↓  ↓  ↓  ↓  ↓  ↓  ↓
           [4][4][4][4][4][4][4][4] = 32 bits
```

**Comment utiliser une S-box — exemple avec S1 et bloc `001011` :**

```
001011
│    └─ bit 6 = 1 ┐
└────── bit 1 = 0 ┘ → ligne = 01 = 1

 0101
└───┘ → bits 2 à 5 = 0101 = 5 → colonne 5

S1[1][5] = 2 → 0010
```

> 💡 Les S-boxes sont **non linéaires** — c'est ce qui résiste à l'algèbre. Elles ont été soigneusement conçues pour résister aux attaques connues.

---

## Étape 7 — Permutation P (32 bits)

```
Table P :
16   7  20  21
29  12  28  17
 1  15  23  26
 5  18  31  10
 2   8  24  14
32  27   3   9
19  13  30   6
22  11   4  25
```

> 💡 La permutation P assure l'**effet avalanche** : les bits d'une S-box influencent plusieurs S-boxes au tour suivant.

---

## Étape 8 — Permutation Inverse PI⁻¹

```
Table PI⁻¹ :
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

## Génération des Sous-clés (CP1 & CP2)

```
Clé (64 bits) → CP1 → 56 bits → L0 (28) | R0 (28)
                                     ↓
                          Décalages gauche par tour
                                     ↓
                    Li, Ri → CP2 → Ki (48 bits)
```

**Décalages par tour :**

```
Tour :  1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16
Décal:  1   1   2   2   2   2   2   2   1   2   2   2   2   2   2   1
```

---

## Déchiffrement DES

> **Même algorithme** que le chiffrement — les sous-clés sont utilisées en ordre **inverse** : K16 → K15 → ... → K1

C'est la propriété du **schéma de Feistel** — il est auto-inverse !

---

## Cryptanalyse du DES

| Attaque | Par | Principe | Complexité |
|---------|-----|----------|------------|
| **Force brute** | — | Tester les 2⁵⁶ clés | 228 ans à 10M/sec (cassé en 56h en 1998) |
| **Différentielle** | Biham & Shamir (1991) | Analyser différences entre textes | 2⁴⁷ textes choisis |
| **Tickling** | IBM (années 70) | Variante différentielle, secrète 20 ans | Renforça les S-boxes |
| **Linéaire** | Matsui (1993) | Approximation linéaire | 2⁴³ couples |

---

## Triple DES (3DES)

```
Mode EDE : C = EK1( DK2( EK1(M) ) )

M → E(K1) → D(K2) → E(K1) → C
```

| Config | Clés | Sécurité effective |
|--------|------|--------------------|
| 2 clés | K1 ≠ K2 | 112 bits |
| 3 clés | K1 ≠ K2 ≠ K3 | 168 bits |
| K1 = K2 | — | DES simple (rétrocompat) |

---

## Distribution de Clés

### Protocole Needham-Schroeder

```
1. P → KDC : { P, V, np }
2. KDC → P : { np, V, Kss, {Kss, P}Kv }Kp
3. P → V   : { Kss, P }Kv
4. V → P   : { nv }Kss
5. P → V   : { f(nv) }Kss  ✅ session établie
```

### Kerberos

```
Client ──AS-REQ──────────► AS
Client ◄─TGT + Kss─────────
Client ──TGS-REQ + TGT───► TGS
Client ◄─Ticket service────
Client ──AP-REQ + ticket──► Serveur
Client ◄─AP-REP────────────  ✅
```

---

## Résumé DES

| Propriété | Valeur |
|-----------|--------|
| Taille bloc | 64 bits |
| Taille clé | 56 bits (+ 8 parité) |
| Nombre de tours | 16 |
| Structure | Feistel |
| Statut | ❌ Obsolète — remplacé par AES |
