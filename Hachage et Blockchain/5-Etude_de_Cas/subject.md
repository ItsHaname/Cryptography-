# 🔗 Implémentation d'une Blockchain en C pour la Traçabilité Agroalimentaire
### Maroc 🇲🇦 → Europe 🇪🇺

---

## 📋 Table des matières

1. [Problématique](#-problématique)
2. [Contexte](#-contexte)
3. [Objectif](#-objectif)
4. [Acteurs du système](#-acteurs-du-système)
5. [Solution proposée](#-solution-proposée)
6. [Structure technique](#-structure-technique)
7. [Fonctions à implémenter](#-fonctions-à-implémenter)
8. [Parcours du produit](#-parcours-du-produit)
9. [Résultats attendus](#-résultats-attendus)

---

## ❓ Problématique

> **Comment garantir l'authenticité et la traçabilité des produits agricoles marocains exportés vers l'Europe en utilisant la technologie Blockchain ?**

---

## 🌍 Contexte

Un groupe d'exportateurs de produits agricoles au **Maroc** souhaite améliorer la traçabilité des fruits exportés vers l'**Europe**.

### Ce que les clients européens exigent :

```
✅ Transparence sur l'origine du produit
✅ Preuve de qualité et certification
✅ Respect des normes sanitaires
✅ Rapidité en cas de rappel produit
```

### Problèmes du système actuel :

```
❌ Système basé sur papier + base de données centralisée
❌ Risque de falsification des documents
❌ Manque de transparence
❌ Difficulté d'audit
❌ Délais importants en cas de contrôle
```

---

## 🎯 Objectif

> Garantir la **transparence** et la **confiance** dans la chaîne d'approvisionnement alimentaire du **producteur → au consommateur** en implémentant une solution Blockchain en langage **C**.

---

## 👥 Acteurs du système

| Acteur | Rôle |
|---|---|
| 🌾 Agriculteur | Enregistre la récolte |
| 🏢 Coopérative | Coordonne les acteurs |
| 🔬 Laboratoire | Certifie la qualité |
| 🚛 Transporteur | Assure le transport |
| 🛃 Douanes | Valide l'export |
| 🏭 Importateur | Réceptionne en Europe |
| 🏪 Supermarché | Vend au consommateur |

---

## 💡 Solution proposée

La coopérative décide d'implémenter une solution basée sur une **Blockchain** :

```
Chaque étape du parcours du produit = un bloc
Chaque bloc est lié au précédent via son hash
Toute modification est détectée immédiatement
```

---

## 🏗️ Structure technique

```c
typedef struct Block {
    int          indice;          // numéro du bloc
    char         id_produit[20];  // ex: P001
    char         date[20];        // ex: 02/03/2026
    char         acteur[50];      // ex: Agriculteur
    char         action[256];     // ex: Récolte 100kg
    int          quantite;        // ex: 100
    char         prev_hash[65];   // hash du bloc précédent
    char         hash[65];        // hash SHA256 de ce bloc
    struct Block *next;           // lien liste chaînée
} Block;
```

---

## ⚙️ Fonctions à implémenter

```
CreeBlock()    → créer et allouer un bloc en mémoire
ComputeHash()  → calculer le hash SHA256 du bloc
addBlock()     → ajouter un bloc à la fin de la chaîne
print_chain()  → afficher tout l'historique du produit
verify_chain() → vérifier l'intégrité de la blockchain
```

---

## 📦 Parcours du produit P001

```
Bloc 0 ─── Genesis
            Initialisation de la blockchain
                    │
                    ▼
Bloc 1 ─── Agriculteur
            Date     : 02/03/2026
            Action   : Récolte oranges parcelle A3 - Agadir
            Quantité : 100 kg
                    │
                    ▼
Bloc 2 ─── Laboratoire
            Date     : 03/03/2026
            Action   : Test phytosanitaire OK - Certifié UE
            Quantité : 100 kg
                    │
                    ▼
Bloc 3 ─── Transporteur
            Date     : 04/03/2026
            Action   : Transport Agadir→Casablanca Temp:4°C
            Quantité : 100 kg
                    │
                    ▼
Bloc 4 ─── Douanes
            Date     : 05/03/2026
            Action   : Documents validés - Export autorisé
            Quantité : 100 kg
                    │
                    ▼
Bloc 5 ─── Supermarché
            Date     : 06/03/2026
            Action   : Réception Paris - QR code généré
            Quantité : 100 kg
```

---

## ✅ Résultats attendus

### 1️⃣ Affichage de la chaîne complète
```
+-----------------------------------------+
| BLOC 1                                  |
+-----------------------------------------+
  ID Produit : P001
  Date       : 02/03/2026
  Acteur     : Agriculteur
  Action     : Récolte oranges parcelle A3
  Quantité   : 100 kg
  Hash       : e3b0c44298fc1c149afb...
+-----------------------------------------+
```

### 2️⃣ Vérification de l'intégrité
```
✅ Bloc 1 (Agriculteur)  → OK
✅ Bloc 2 (Laboratoire)  → OK
✅ Bloc 3 (Transporteur) → OK
✅ Bloc 4 (Douanes)      → OK
✅ Bloc 5 (Supermarché)  → OK

✅ Blockchain valide ! Historique infalsifiable.
```

### 3️⃣ Simulation d'une attaque
```
💀 Hacker modifie le Bloc 2 (certificat labo)...

❌ Bloc 3 corrompu ! La chaîne est invalide.
→ Falsification détectée immédiatement !
```

---

## 🛠️ Compilation

```bash
gcc -o tracabilite tracabilite.c -lssl -lcrypto && ./tracabilite
```

---

## 📚 Technologies utilisées

| Technologie | Usage |
|---|---|
| Langage C | Implémentation de la blockchain |
| OpenSSL SHA256 | Calcul des hash des blocs |
| Liste chaînée | Structure de la blockchain |

---

> *"La blockchain garantit qu'aucun acteur ne peut modifier rétroactivement une information sans que tout le réseau le détecte immédiatement."*
