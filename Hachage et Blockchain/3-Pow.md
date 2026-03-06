# ⛏️ Proof of Work — Concept Théorique

---

## Table des matières

1. [Le problème à résoudre](#1-le-problème-à-résoudre)
2. [L'analogie simple](#2-lanalogie-simple)
3. [Le Nonce](#3-le-nonce)
4. [La difficulté](#4-la-difficulté)
5. [Pourquoi c'est utile ?](#5-pourquoi-cest-utile-)
6. [Résumé final](#6-résumé-final)

---

## 1. Le problème à résoudre

Dans une blockchain, n'importe qui peut essayer d'ajouter un faux bloc :

```
[Bloc 0] → [Bloc 1] → [FAUX BLOC] ← hacker !
```

Il faut un moyen de dire :

> **"Pour ajouter un bloc, tu dois prouver que tu as travaillé dur"**

C'est ça le **Proof of Work** — une preuve de travail.

---

## 2. L'analogie simple

Imagine que je te dis :

```
"Trouve un nombre qui, ajouté à 'Bonjour',
donne un résultat qui commence par 000"
```

Tu es obligé d'essayer un par un :

```
"Bonjour1"   → résultat = "a3f8..." ❌
"Bonjour2"   → résultat = "7bc1..." ❌
"Bonjour3"   → résultat = "f2d4..." ❌
...
"Bonjour847" → résultat = "000abc" ✅ TROUVÉ !
```

> Tu as dû **travailler** pour trouver 847. C'est ta preuve !

---

## 3. Le Nonce

Ce nombre qu'on cherche s'appelle le **Nonce** :

```
Nonce = Number used ONCE
      = un nombre qu'on essaie une seule fois
```

La règle est simple :

```
hash(data + nonce) = "000000..." ← doit commencer par des zéros
```

Le mineur essaie nonce = 0, 1, 2, 3... jusqu'à trouver un hash valide.

---

## 4. La difficulté

Le réseau décide combien de zéros sont nécessaires :

```
difficulté 1  → hash doit commencer par "0"
difficulté 2  → hash doit commencer par "00"
difficulté 3  → hash doit commencer par "000"
difficulté 4  → hash doit commencer par "0000"
```

Plus il y a de zéros → plus c'est difficile → plus ça prend du temps :

```
difficulté 1  → trouve en quelques essais
difficulté 4  → trouve en milliers d'essais
difficulté 20 → trouve en milliards d'essais  ← Bitcoin réel !
```

---

## 5. Pourquoi c'est utile ?

### 🔒 Sécurité

Pour modifier un bloc, le hacker doit **recalculer le Nonce** de tous les blocs suivants :

```
[Bloc 0] → [Bloc 1] → [Bloc 2] → [Bloc 3]
               ↑
          hacker modifie ici
               ↓
    doit recalculer Nonce de Bloc 1
    doit recalculer Nonce de Bloc 2
    doit recalculer Nonce de Bloc 3
    = impossible en pratique !
```

---

### 🤝 Consensus

Tout le réseau accepte le bloc **seulement si** le Nonce est valide :

```
Mineur trouve Nonce → envoie au réseau
Réseau vérifie      → hash(data + nonce) commence par "000..." ?

OUI → bloc accepté ✅
NON → bloc rejeté  ❌
```

---

### 🏆 Récompense

Le mineur qui trouve le Nonce en premier reçoit des **Bitcoin** comme récompense :

```
Mineur A cherche...
Mineur B cherche...   → Mineur B trouve en premier !
Mineur C cherche...      → Mineur B reçoit 3.125 BTC 🎉
```

---

## 6. Résumé final

```
MINER UN BLOC :

données du bloc
      +
   nonce = 0      → hash = "a3f8..." ❌
   nonce = 1      → hash = "7bc1..." ❌
   nonce = 2      → hash = "f2d4..." ❌
      ...
   nonce = 48291  → hash = "0000a3" ✅

      ↓
bloc accepté par le réseau
      ↓
mineur reçoit récompense
```

| Concept      | Définition                                              |
|--------------|---------------------------------------------------------|
| Proof of Work| Preuve que le mineur a travaillé dur                    |
| Nonce        | Nombre qu'on cherche pour obtenir un hash valide        |
| Difficulté   | Nombre de zéros requis au début du hash                 |
| Mining       | Processus de chercher le bon Nonce                      |
| Récompense   | Bitcoin reçu par le mineur qui trouve le Nonce en premier|

---

> *"Le Proof of Work est le mécanisme qui rend la blockchain immuable et sécurisée — modifier un bloc signifie refaire le travail de tous les blocs suivants."*
