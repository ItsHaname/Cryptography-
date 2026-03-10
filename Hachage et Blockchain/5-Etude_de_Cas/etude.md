# 🔗 Blockchain Traçabilité Agroalimentaire
### Projet en langage C — Maroc → Europe — Produit P001 (Oranges)

---

## 📋 Table des matières

1. [Le Problème — Pourquoi une Blockchain ?](#1-le-problème)
2. [C'est quoi une Blockchain ?](#2-cest-quoi-une-blockchain)
3. [C'est quoi un Hash ?](#3-cest-quoi-un-hash)
4. [Comment les blocs sont liés ?](#4-comment-les-blocs-sont-liés)
5. [La structure en C — struct Block](#5-la-structure-en-c)
6. [Les fonctions — explication simple](#6-les-fonctions)
7. [Le parcours complet du produit P001](#7-le-parcours-complet)
8. [Simulation d'attaque — comment on détecte un hacker](#8-simulation-dattaque)
9. [Le QR Code — comment ça marche](#9-le-qr-code)
10. [Comment compiler et lancer](#10-comment-compiler-et-lancer)


---

## 1. Le Problème

Imagine que tu achètes des oranges au supermarché à Paris 🍊

Tu te poses ces questions :

- **D'où viennent ces oranges ?** → Quelle ferme, quelle région ?
- **Sont-elles certifiées ?** → Est-ce qu'un laboratoire les a contrôlées ?
- **La chaîne du froid a-t-elle été respectée ?** → Transport à 4°C ?
- **Les documents sont-ils authentiques ?** → Personne n'a falsifié ?

### ❌ Avant la blockchain (système classique)

```
Agriculteur  →  papier  →  Laboratoire  →  papier  →  Supermarché
                  ↑                           ↑
            Facile à falsifier          Peut être modifié
```

- Tout est sur **papier** → n'importe qui peut modifier
- Base de données **centralisée** → un seul hacker suffit
- **Aucune transparence** → le consommateur ne sait rien

### ✅ Avec la blockchain (notre système)

```
Agriculteur  →  Bloc 1  →  Laboratoire  →  Bloc 2  →  Supermarché  →  Bloc 5
                  ↑                           ↑                           ↑
            Hash verrouillé           Hash verrouillé             QR Code généré
```

- Chaque étape est **enregistrée et verrouillée** par un hash
- Modifier un bloc **casse toute la chaîne** → détection immédiate
- Le client **scanne le QR code** et voit tout l'historique

---

## 2. C'est quoi une Blockchain ?

C'est simplement une **liste de blocs** où chaque bloc est lié au précédent.

Imagine une **chaîne de cadenas** :

```
[Bloc 0] ──► [Bloc 1] ──► [Bloc 2] ──► [Bloc 3] ──► [Bloc 4] ──► [Bloc 5] ──► NULL
 Genesis      Farmer        Lab         Transport     Customs      Supermarket
```

> **La règle d'or :** Si tu modifies un bloc au milieu,
> toute la chaîne après lui est cassée.
> C'est exactement comme enlever un maillon d'une chaîne.

### Pourquoi c'est puissant ?

Parce que chaque bloc contient **deux empreintes** :
- Son propre hash (son empreinte à lui)
- Le hash du bloc **précédent** (l'empreinte de son voisin)

→ Si quelqu'un modifie le Bloc 2, son hash change.
→ Le Bloc 3 dit : *"Attends, le hash que j'ai stocké ne correspond plus !"*
→ **Falsification détectée !**

---

## 3. C'est quoi un Hash ?

Un hash c'est une **empreinte digitale numérique** d'un texte.

Comme les empreintes de doigts :
- Chaque texte a **son empreinte unique**
- Si tu changes **une seule lettre** → empreinte complètement différente
- **Impossible** de retrouver le texte original depuis l'empreinte

### Exemple concret

```
Texte original :
"Harvest Agadir 100kg"  →  hash  →  "3373254562361698194"

Texte modifié (juste le k → K) :
"Harvest Agadir 100Kg"  →  hash  →  "9182736450123456789"
                                          ↑
                              Complètement différent !
```

### L'algorithme utilisé : djb2

C'est l'algorithme qu'on utilise dans le code. Il est simple mais efficace :

```c
static void computeHash(const char *input, char out[32])
{
    unsigned long h = 5381;           // valeur de départ
    for (int i = 0; input[i]; i++)
        h = ((h << 5) + h) + input[i]; // formule magique : h * 33 + lettre
    sprintf(out, "%lu", h);           // convertir en texte
}
```

**Traduction en français :**
1. On part du nombre `5381`
2. Pour chaque lettre du texte, on fait un calcul mathématique
3. Le résultat final est un grand nombre unique → c'est le hash
---

## 4. Comment les blocs sont liés ?

C'est **le cœur de la blockchain**. Voici comment ça fonctionne en détail :

```
BLOC 1                         BLOC 2                         BLOC 3
┌──────────────────────┐       ┌──────────────────────┐       ┌──────────────────────┐
│ index    : 1         │       │ index    : 2         │       │ index    : 3         │
│ actor    : Farmer    │       │ actor    : Lab        │       │ actor    : Transport │
│ action   : Harvest   │       │ action   : Test OK   │       │ action   : 4°C       │
│ quantity : 100 kg    │       │ quantity : 100 kg    │       │ quantity : 100 kg    │
│ prevHash : hash0  ───┼──┐    │ prevHash : hash1  ───┼──┐    │ prevHash : hash2  ───┼──►...
│ hash     : hash1     │  │    │ hash     : hash2     │  │    │ hash     : hash3     │
│ next  ───────────────┼──┼───►│ next  ───────────────┼──┼───►│ next  ───────────────┼──► NULL
└──────────────────────┘  │    └──────────────────────┘  │    └──────────────────────┘
                           │                               │
                     prevHash du Bloc 2             prevHash du Bloc 3
                     = hash du Bloc 1               = hash du Bloc 2
```

Il y a **deux liens** entre chaque bloc :

| Lien | Champ | Rôle |
|------|-------|------|
| **Lien de liste** | `struct Block *next` | Pointer vers le bloc suivant en mémoire |
| **Lien cryptographique** | `prevHash` | Stocker l'empreinte du bloc précédent |

> Le lien `next` c'est la **liste chaînée**.
> Le lien `prevHash` c'est la **sécurité blockchain**.
> Notre projet combine les deux !

---

## 5. La structure en C

### Le bloc — struct Block

```c
typedef struct Block {
    int          index;          // numéro du bloc : 0, 1, 2, 3...
    char         productID[20];  // identifiant produit : "P001"
    char         actor[50];      // qui agit : "Farmer", "Laboratory"...
    char         action[150];    // ce qu'il fait : "Harvest - Agadir"
    int          quantity;       // quantité : 100 kg
    char         prevHash[32];   // empreinte du bloc AVANT moi
    char         hash[32];       // ma propre empreinte
    struct Block *next;          // flèche vers le bloc suivant ← liste chaînée
} Block;
```

**Chaque champ expliqué :**

- `index` → le numéro d'ordre. Bloc 0 = Genesis, Bloc 1 = Farmer, etc.
- `productID` → toujours "P001" dans notre cas (les oranges)
- `actor` → qui ajoute ce bloc ? L'agriculteur, le labo, le transporteur...
- `action` → qu'est-ce qu'il fait ? Récolte, test, transport...
- `quantity` → combien de kg de produit
- `prevHash` → l'empreinte du bloc d'avant. C'est ça qui crée la **chaîne**
- `hash` → mon empreinte calculée à partir de TOUS mes champs
- `*next` → le pointeur de liste chaînée. Pointe vers le bloc suivant

### La blockchain — conteneur de la liste

```c
typedef struct {
    Block *head;  // pointeur vers le PREMIER bloc (Genesis)
    Block *tail;  // pointeur vers le DERNIER bloc (Supermarket)
} Blockchain;
```

On garde `tail` pour ne pas parcourir toute la liste à chaque ajout.

---

## 6. Les fonctions

### `createBlock()` — Fabriquer un bloc

```c
Block* createBlock(int index, const char *productID,
                   const char *actor, const char *action,
                   int quantity, const char *prevHash)
{
    Block *b = malloc(sizeof(Block));  // ← réserver de la mémoire
    // remplir tous les champs...
    computeHash(buf, b->hash);         // ← calculer l'empreinte
    return b;                          // ← retourner le bloc créé
}
```

**Ce que fait `malloc` :**
- `malloc` = *memory allocation* = demander à l'ordinateur de réserver de la place
- Sans `malloc`, le bloc disparaîtrait à la fin de la fonction
- On doit faire `free()` à la fin pour libérer cette mémoire

**Ce que fait `computeHash` ici :**
- Elle prend TOUS les champs : index + productID + actor + action + quantity + prevHash
- Elle les colle ensemble en une seule chaîne
- Elle calcule l'empreinte → stockée dans `b->hash`

---

### `newBlockchain()` — Créer la blockchain avec le Genesis

```c
Blockchain* newBlockchain()
{
    Blockchain *bc = malloc(sizeof(Blockchain));
    bc->head = createBlock(0, "P001", "System", "Genesis Block", 0, "00000000");
    bc->tail = bc->head;  // au début, head et tail pointent vers le même bloc
    return bc;
}
```

Le **Bloc Genesis** (bloc 0) c'est le point de départ.
- Son `prevHash` = `"00000000"` → il n'y a rien avant lui
- C'est comme la première page d'un livre, vide, qui dit "ici commence l'histoire"

---

### `addBlock()` — Ajouter un bloc à la fin

```c
void addBlock(Blockchain *bc, ...) {
    // Créer le nouveau bloc avec prevHash = hash du dernier bloc
    Block *nouveau = createBlock(
        bc->tail->index + 1,    // index = dernier + 1
        product, actor, action, qty,
        bc->tail->hash          // ← prevHash = hash du dernier bloc actuel
    );

    bc->tail->next = nouveau;   // ← le dernier bloc pointe vers le nouveau
    bc->tail = nouveau;         // ← le nouveau devient le dernier
}
```

**Schéma de ce qui se passe :**

```
AVANT addBlock :
[Genesis] → [Farmer] → [Lab] → NULL
                          ↑
                        tail

APRÈS addBlock("Transporter", ...) :
[Genesis] → [Farmer] → [Lab] → [Transporter] → NULL
                                      ↑
                                    tail
```

---

### `verifyChain()` — Détecter une falsification

```c
int verifyChain(Blockchain *bc)
{
    Block *prev = NULL, *cur = bc->head;

    while (cur) {                           // parcourir tous les blocs
        // Étape 1 : recalculer le hash de ce bloc
        computeHash(buf, expected);

        // Étape 2 : comparer avec le hash stocké
        if (strcmp(expected, cur->hash) != 0)
            return 0;                       // ← FALSIFICATION DÉTECTÉE !

        // Étape 3 : vérifier le lien avec le bloc précédent
        if (prev && strcmp(cur->prevHash, prev->hash) != 0)
            return 0;                       // ← CHAÎNE BRISÉE !

        prev = cur;
        cur = cur->next;
    }
    return 1;                               // tout est bon → VALIDE
}
```

**En français :** Pour chaque bloc, on refait le calcul du hash depuis zéro.
Si le résultat est différent de ce qui est stocké → quelqu'un a triché.

---

### `generateHTML()` — Créer la page web automatiquement

```c
void generateHTML(Blockchain *bc, const char *filename)
{
    FILE *f = fopen(filename, "w");   // créer le fichier
    // ...
    // Parcourir la liste chaînée et écrire chaque bloc en HTML
    Block *cur = bc->head;
    while (cur) {
        writeBlock(f, cur, ...);      // écrire ce bloc en HTML
        if (cur->next) writeArrow(f); // écrire la flèche →
        cur = cur->next;              // passer au suivant
    }
    fclose(f);
}
```

Le programme C **écrit lui-même** le fichier HTML ligne par ligne avec `fprintf()`.
C'est comme si le programme C "tapait" du HTML à ta place.

---

## 7. Le parcours complet du produit P001

Voici ce qui se passe quand on lance le programme :

```
main()
  │
  ├─► newBlockchain()
  │     └─► Bloc 0 : System → "Genesis Block"
  │           hash = 8958039863731247492
  │           prevHash = 00000000
  │
  ├─► addBlock("Farmer", "Harvest - Parcelle A3 - Agadir", 100)
  │     └─► Bloc 1 : Farmer
  │           hash = 3373254562361698194
  │           prevHash = 8958039863731247492  ← hash du Bloc 0
  │
  ├─► addBlock("Laboratory", "Phytosanitary Test OK - Certifié UE", 100)
  │     └─► Bloc 2 : Laboratory
  │           hash = 3056869204396632378
  │           prevHash = 3373254562361698194  ← hash du Bloc 1
  │
  ├─► addBlock("Transporter", "Transport Agadir→Casablanca - 4°C", 100)
  │     └─► Bloc 3 : Transporter
  │           hash = 6529657210709740653
  │           prevHash = 3056869204396632378  ← hash du Bloc 2
  │
  ├─► addBlock("Customs", "Documents validés - Export autorisé", 100)
  │     └─► Bloc 4 : Customs
  │           hash = 17537833833942589170
  │           prevHash = 6529657210709740653  ← hash du Bloc 3
  │
  ├─► addBlock("Supermarket", "Réception Paris - QR code généré", 100)
  │     └─► Bloc 5 : Supermarket
  │           hash = 17135948130579089384
  │           prevHash = 17537833833942589170 ← hash du Bloc 4
  │
  ├─► printChain()     → affiche dans le terminal
  ├─► verifyChain()    → vérifie l'intégrité → VALIDE ✓
  └─► generateHTML()   → crée blockchain.html avec QR code
```

---

## 8. Simulation d'attaque

Que se passe-t-il si un hacker essaie de modifier un bloc ?

### Scénario : le hacker modifie le Bloc 2 (Lab)

```
AVANT (légitime) :
Bloc 2 : action = "Phytosanitary Test OK"
         hash   = 3056869204396632378

APRÈS (hacker change le résultat) :
Bloc 2 : action = "Phytosanitary Test FAILED"  ← modifié !
         hash   = 3056869204396632378           ← MAIS le hash n'a pas changé !
```

**Quand on appelle `verifyChain()` :**

```
Bloc 2 :
  → On recalcule le hash de "Phytosanitary Test FAILED"
  → Résultat : 9999888877776666555  ← complètement différent !
  → On compare avec le hash stocké : 3056869204396632378
  → ILS SONT DIFFÉRENTS → FALSIFICATION DÉTECTÉE ! ❌
```

**Et même si le hacker met à jour le hash du Bloc 2 :**

```
Bloc 3 :
  → Son prevHash dit : 3056869204396632378  (l'ancien hash du Bloc 2)
  → Mais maintenant le hash du Bloc 2 est : 9999888877776666555
  → ILS NE CORRESPONDENT PAS → CHAÎNE BRISÉE ! ❌
```

> **Conclusion :** Pour falsifier un bloc, il faudrait recalculer
> TOUS les blocs suivants. Et dans une vraie blockchain publique,
> c'est impossible car des milliers d'ordinateurs vérifient en permanence.

---

## 9. Le QR Code

### Comment il est généré ?

Le programme C construit automatiquement une URL comme ça :

```c
void buildQRUrl(Blockchain *bc, char *url, int urlSize)
{
    // Construire le contenu texte à encoder dans le QR
    // "TRACABILITE PRODUIT P001
    //  1. Farmer : Harvest - Agadir
    //  2. Laboratory : Test OK
    //  3. Transporter : 4°C
    //  ...
    //  Hash final : 17135948130579089384
    //  BLOCKCHAIN VALIDE"

    // Construire l'URL de l'API
    snprintf(url, urlSize,
        "https://api.qrserver.com/v1/create-qr-code/?data=...");
}
```

### Comment l'image apparaît dans le HTML ?

```html
<img src="https://api.qrserver.com/v1/create-qr-code/?data=TRACABILITE+PRODUIT+P001...">
```

Le navigateur télécharge l'image du QR depuis Internet et l'affiche.
**Aucune bibliothèque C nécessaire !** L'API fait tout le travail.

### Ce que le client voit quand il scanne

```
📱 Scanner avec le téléphone →

TRACABILITE PRODUIT P001
Maroc - Europe
---
1. Farmer : Harvest - Parcelle A3 - Agadir
2. Laboratory : Phytosanitary Test OK - Certifié UE
3. Transporter : Transport Agadir-Casablanca - 4°C
4. Customs : Documents validés - Export autorisé
5. Supermarket : Réception Paris - QR code généré
---
Hash final : 17135948130579089384
BLOCKCHAIN VALIDE ✓
```

---

## 10. Comment compiler et lancer

### Prérequis

```bash
# Vérifier que gcc est installé
gcc --version
```

### Compilation

```bash
# Un seul fichier, aucune bibliothèque externe nécessaire !
gcc -o blockchain main.c
```

### Lancer le programme

```bash
./blockchain
```

### Ce que tu vois dans le terminal

```
=== Construction de la blockchain ===

---------------------
Index    : 0
Actor    : System
Action   : Genesis Block
Hash     : 8958039863731247492
PrevHash : 00000000
---------------------
Index    : 1
Actor    : Farmer
Action   : Harvest - Parcelle A3 - Agadir
Quantity : 100 kg
Hash     : 3373254562361698194
PrevHash : 8958039863731247492
...

Verification : VALIDE

>>> Fichier genere : blockchain.html
>>> Ouvre blockchain.html dans Firefox !
```

### Ouvrir la visualisation

```bash
firefox blockchain.html
# ou
xdg-open blockchain.html
```

> ⚠️ **Connexion internet requise** pour afficher le QR code
> (l'image est générée par api.qrserver.com)

---


### En 30 secondes

> *"J'ai implémenté une blockchain en C avec une liste chaînée.
> Chaque `struct Block` contient le hash du bloc précédent dans `prevHash`,
> et pointe vers le suivant avec `*next`.
> Si quelqu'un modifie un bloc, son hash change et `verifyChain()` le détecte.
> Mon programme C génère automatiquement un fichier HTML avec un QR code
> réel que n'importe quel client peut scanner pour voir tout le parcours du produit."*

### Les points clés à retenir

| Concept | Explication simple |
|---------|-------------------|
| **Liste chaînée** | Chaque bloc pointe vers le suivant avec `struct Block *next` |
| **Hash** | Empreinte numérique unique calculée avec l'algo djb2 |
| **Lien cryptographique** | `prevHash` = hash du bloc précédent → impossible de tricher |
| **Vérification** | `verifyChain()` recalcule tous les hashs et compare |
| **QR Code** | Généré dynamiquement par le C via l'API qrserver.com |
| **generateHTML()** | Le programme C écrit lui-même le fichier HTML avec `fprintf()` |

### La différence avec le projet C++ de ton ami

| Ton projet C | Projet C++ de l'ami |
|-------------|---------------------|
| `struct Block` + `*next` = liste chaînée manuelle | `class Block` + `std::vector` = tableau automatique |
| `computeHash()` algo djb2 | `calculateHash()` algo djb2 (identique !) |
| `generateHTML()` → visualisation web + QR | `printChain()` → terminal seulement |
| `malloc` / `free` manuel | Gestion mémoire automatique C++ |

---

## 📁 Structure des fichiers

```
projet/
│
├── main.c           ← Le code source C (tout est là-dedans)
│
└── blockchain.html  ← Généré automatiquement par ./blockchain
                       (tu n'as pas besoin de le modifier)
```

---

## 🔑 Résumé en une image

```
main.c
  │
  ├── struct Block { ..., char hash[32], char prevHash[32], struct Block *next }
  │         ↑ La structure de base — un nœud de la liste chaînée
  │
  ├── computeHash()    → Calcule l'empreinte numérique (algo djb2)
  ├── createBlock()    → Crée un nœud avec malloc + calcule son hash
  ├── addBlock()       → Ajoute un nœud à la fin de la liste
  ├── verifyChain()    → Vérifie que personne n'a triché
  ├── printChain()     → Affiche dans le terminal
  └── generateHTML()   → Écrit blockchain.html avec QR code intégré
              │
              ▼
        blockchain.html  (ouvert dans Firefox)
              │
              ├── Fond bleu ciel #87ceeb
              ├── 6 cartes (Genesis → Farmer → Lab → Transport → Customs → Supermarket)
              ├── Flèches "next" entre chaque carte
              ├── Hash affiché sur chaque carte
              └── QR Code scannable → historique complet du produit P001
```

---

*Généré automatiquement par `./blockchain` · Hash djb2 · Liste chaînée en C*
