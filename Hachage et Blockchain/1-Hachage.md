# Fonctions de Hachage Cryptographique

> Une introduction complète aux fonctions de hachage — théorie, algorithmes et applications réelles.

---

## Table des matières

- [Qu'est-ce qu'une fonction de hachage ?](#quest-ce-quune-fonction-de-hachage-)
- [Propriétés fondamentales](#propriétés-fondamentales)
- [La famille MD](#la-famille-md)
- [La famille SHA](#la-famille-sha)
- [Applications concrètes](#applications-concrètes)

---

## Qu'est-ce qu'une fonction de hachage ?

Une fonction de hachage est une fonction mathématique qui prend n'importe quelle entrée (texte, fichier, image...) et produit une sortie de taille fixe, appelée selon le contexte :

- Condensé
- Empreinte
- Hash
- Message Digest
- MAC

```
Entrée (taille variable)   ->   [Fonction de Hachage]   ->   Sortie (taille fixe)

"Bonjour"                  ->          MD5              ->   8b1a9953c461...  (32 car.)
"Bonjour tout le monde"    ->          MD5              ->   3d2f1a9b8c7e...  (32 car.)
Un fichier de 10 Go        ->          MD5              ->   9f8e7d6c5b4a...  (32 car.)
```

> Peu importe la taille de l'entrée, la sortie a toujours la même taille. C'est comme un résumé universel.

---

## Propriétés fondamentales

### 1. Sens unique (One-Way)

```
M     -->  F(M)    [Facile et rapide]
F(M)  -->  M       [Mathématiquement impossible]
```

**Analogie :** Le hachoir à viande — tu peux hacher facilement, mais impossible de reconstituer le morceau original depuis le haché.

**Application réelle :** Les mots de passe ne sont jamais stockés en clair sur les serveurs. Seul leur hash est conservé. Même l'administrateur du site ne connaît pas ton mot de passe.

```
Mot de passe saisi  :  "azerty123"
Ce qui est stocké   :  "a8f5f167f44f4964e6c998dee827110c"
```

---

### 2. Résistance aux collisions

```
M != M'   -->   F(M) != F(M')
```

Deux messages différents ne peuvent jamais produire le même hash.

**Analogie :** Comme les empreintes digitales — chaque personne possède les siennes, uniques au monde.

---

### 3. Effet avalanche

Un changement minime dans l'entrée produit un hash radicalement différent :

```
"Bonjour"  -->  7c6f4d3e2b1a...
"bonjour"  -->  9a8b7c6d5e4f...   <- totalement différent !
```

Un simple changement de casse (`B` -> `b`) suffit à modifier l'empreinte de façon complète et imprévisible.

---

### 4. Déterminisme

La même entrée produit toujours le même hash, indépendamment du moment ou du système utilisé :

```
"Bonjour"  ->  MD5  ->  8b1a9953...  (aujourd'hui)
"Bonjour"  ->  MD5  ->  8b1a9953...  (dans 10 ans)
```

---

## La famille MD

### MD4

- Créé par Ronald Rivest au MIT
- Rapidement abandonné en raison de vulnérabilités critiques
- Des chercheurs ont trouvé des collisions sans difficulté

### MD5 (1994)

- Créé par Rivest pour remplacer MD4
- Produit une empreinte de **128 bits** soit **32 caractères hexadécimaux**
- Largement utilisé pour vérifier l'intégrité des fichiers
- **Déconseillé en cryptographie** — des collisions ont été démontrées dès 2004

**Fonctionnement interne simplifié :**

```
Message
  |
  v
Découpage en blocs de 512 bits
  |
  v
4 rondes de transformations (64 opérations)
  |
  v
Hash de 128 bits
```

---

## La famille SHA

### SHA-0

- Première version, retirée immédiatement après la découverte d'une faille majeure
- Pratiquement jamais déployée en production

### SHA-1

- Produit **160 bits** soit **40 caractères**
- Utilisé pendant de nombreuses années, mais officiellement déconseillé depuis 2017
- Google a démontré une collision en 2017 via le projet **SHAttered**

### SHA-2 — La famille moderne

Plusieurs variantes selon le niveau de sécurité requis :

| Algorithme | Taille     | Caractères | Usage                      |
|------------|------------|------------|----------------------------|
| SHA-224    | 224 bits   | 56 car.    | Rare                       |
| SHA-256    | 256 bits   | 64 car.    | **Standard actuel**        |
| SHA-384    | 384 bits   | 96 car.    | Sécurité renforcée         |
| SHA-512    | 512 bits   | 128 car.   | Très haute sécurité        |

**SHA-256** est utilisé par Bitcoin, les certificats SSL/TLS et les signatures numériques. C'est le standard mondial de référence.

---

## Applications concrètes

| Application           | Algorithme utilisé    | Description                                       |
|-----------------------|-----------------------|---------------------------------------------------|
| Mots de passe         | SHA-256, bcrypt       | Stocker le hash, jamais le mot de passe en clair  |
| Bitcoin               | SHA-256               | Sécuriser les blocs (Proof of Work)               |
| Intégrité de fichiers | MD5, SHA-256          | Comparer les empreintes avant/après transfert     |
| Signatures numériques | SHA-256               | Signer le hash du document                        |
| Certificats SSL/HTTPS | SHA-256               | Authentifier les sites web                        |

---

# Partie pratique :
 ## Installation et vérification:
 
 <img width="543" height="103" alt="image" src="https://github.com/user-attachments/assets/00a0a4d9-83f4-494d-be11-6918933e9d9f" />

## Découvrir les algorithmes:

<img width="843" height="174" alt="image" src="https://github.com/user-attachments/assets/dc522002-366f-46a8-82b4-aadbd9dbc038" />

   md5    → 32  caractères
   
   sha1   → 40  caractères
   
  sha256 → 64  caractères
  
  sha512 → 128 caractères

## Intégrité d'un fichier avec MD5
<img width="652" height="261" alt="image" src="https://github.com/user-attachments/assets/de0bd2ac-a825-4f8d-a899-9fe3212093bd" />


💡 La modification est immédiatement détectée 

## Intégrité de plusieurs fichiers:
<img width="549" height="396" alt="image" src="https://github.com/user-attachments/assets/2f353b59-8304-4eb3-83ab-5562481d9db6" />



































## Ressources complémentaires

- [NIST — Secure Hash Standard](https://csrc.nist.gov/publications/detail/fips/180/4/final)
- [RFC 1321 — The MD5 Message-Digest Algorithm](https://www.rfc-editor.org/rfc/rfc1321)
- [SHAttered — First SHA-1 Collision (2017)](https://shattered.io/)

---

*Document rédigé à titre éducatif dans le cadre d'une introduction à la cryptographie.*
