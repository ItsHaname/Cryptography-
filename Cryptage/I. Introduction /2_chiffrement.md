# Chiffrement et Déchiffrement

---

## Le Chiffrement

Le chiffrement est l'opération qui transforme un **texte en clair P** (le message original, lisible) en un **texte chiffré C** (le message illisible) grâce à une fonction mathématique :

> **C = Ek(P)**

| Symbole | Signification |
|---------|--------------|
| **E** | Algorithme de chiffrement (*Encryption*) |
| **k** | La clé secrète utilisée pour chiffrer |
| **P** | Le texte en clair (*Plain text*) |
| **C** | Le texte chiffré (*Cipher text*) |

**Exemple concret :**

```
P = BONJOUR
Clé k = décalage de 3 (comme César)
C = ERQMRXU
```
<img width="664" height="345" alt="image" src="https://github.com/user-attachments/assets/c02a1d21-e735-4534-a853-f8ccbbc80279" />

---

## Le Déchiffrement

C'est l'opération inverse. Le destinataire légitime reçoit C et le déchiffre grâce à une clé k' :

> **P = Dk'(C)**

| Symbole | Signification |
|---------|--------------|
| **D** | Algorithme de déchiffrement (*Decryption*) |
| **k'** | Clé de déchiffrement |
| **C → P** | On retrouve le message original |

---

## Schéma global

```
Alice                                         Bob
  |                                             |
  P  →  [Ek]  →  C  ──(réseau)──►  C  →  [Dk'] →  P
          ↑                                   ↑
        clé k                              clé k'
```
<img width="658" height="328" alt="image" src="https://github.com/user-attachments/assets/b342c87b-f427-468e-bc20-15544173fa4c" />

---

## Point important : k et k'

La relation entre les deux clés définit le **type de cryptosystème** :

| Type | Clés | Exemple |
|------|------|---------|
| **Symétrique** | k = k' (même clé) | DES, OTP |
| **Asymétrique** | k ≠ k' (clés différentes) | RSA |

> 💡 **En résumé** : le chiffrement *verrouille* le message, le déchiffrement *l'ouvre* — mais seul celui qui possède la bonne clé peut le faire.
