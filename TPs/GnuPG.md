# 🔐 Génération et Publication de Clés GPG sur Ubuntu Server

**Auteur :** Hanane AIT BAH  
**Email :** h.aitbah8410@uca.ac.ma  
**Date :** 2026-03-31  
**Système :** Arch Linux / Ubuntu Server  
**Outil :** GnuPG 2.4.9  

---

## 📋 Description

Ce projet documente le processus complet de génération d'une paire de clés GPG (GNU Privacy Guard) et de leur publication sur un serveur de clés public (`keyserver.ubuntu.com`). GPG permet de chiffrer, déchiffrer et signer des données afin de garantir leur authenticité et confidentialité.

---

## 🛠️ Prérequis

- Ubuntu Server ou Arch Linux
- GnuPG installé (`gpg --version`)
- Connexion Internet

### Installation de GnuPG (si nécessaire)

```bash
sudo apt update && sudo apt install gnupg -y
```

Vérification :

```bash
gpg --version
# gpg (GnuPG) 2.4.9
```

---

## 🚀 Étapes Réalisées

### Étape 1 — Génération de la paire de clés

```bash
gpg --full-generate-key
```

<img width="612" height="608" alt="Screenshot etape1 generation" src="https://github.com/user-attachments/assets/4bc96b64-411a-4356-bf79-cb786aa145cc" />

**Paramètres choisis :**

| Paramètre        | Valeur                        |
|------------------|-------------------------------|
| Type de clé      | RSA and RSA (option 1)        |
| Taille           | 1024 bits                     |
| Expiration       | 0 (n'expire jamais)           |
| Nom              | Hanane AIT BAH                |
| Email            | h.aitbah8410@uca.ac.ma        |
| Commentaire      | key                           |

> ⚠️ **Note :** Une taille de 4096 bits est recommandée pour une sécurité optimale en production.

---

### Étape 2 — Vérification de la clé créée

```bash
gpg --list-keys
gpg --list-secret-keys
```

<img width="670" height="482" alt="Screenshot etape2 list keys" src="https://github.com/user-attachments/assets/c32bc7cc-6208-4834-866e-e507e5edbedf" />

**Résultat :**

```
pub   rsa1024 2026-03-31 [SC]
      4BE20301D02BBC2492895E0C6C0C6EF384F697B1
uid   [ultimate] hanane AIT BAH (key) <h.aitbah8410@uca.ac.ma>
sub   rsa1024 2026-03-31 [E]
```

---

### Étape 3 — Export de la clé publique

```bash
gpg --armor --export h.aitbah8410@uca.ac.ma > macle_publique.asc
cat macle_publique.asc
```

<img width="726" height="451" alt="Screenshot etape3 export" src="https://github.com/user-attachments/assets/5405f3ef-6251-4e31-a26b-09ada912082b" />

**Résultat :** Un fichier `macle_publique.asc` contenant le bloc PGP :

```
-----BEGIN PGP PUBLIC KEY BLOCK-----
...
-----END PGP PUBLIC KEY BLOCK-----
```

---

### Étape 4 — Récupération de l'ID long de la clé

```bash
gpg --list-keys --keyid-format LONG h.aitbah8410@uca.ac.ma
```

<img width="742" height="151" alt="Screenshot etape4 keyid" src="https://github.com/user-attachments/assets/0c22230e-07e2-4acd-9377-a9765c105d2b" />

**Résultat :**

```
pub   rsa1024/6C0C6EF384F697B1 2026-03-31 [SC]
      4BE20301D02BBC2492895E0C6C0C6EF384F697B1
uid   [ultimate] hanane AIT BAH (key) <h.aitbah8410@uca.ac.ma>
sub   rsa1024/C49CAEA235B2A3D4 2026-03-31 [E]
```

**Key ID :** `6C0C6EF384F697B1`

---

### Étape 5 — Publication sur le serveur de clés

```bash
gpg --keyserver keyserver.ubuntu.com --send-keys 6C0C6EF384F697B1
```

### Étape 6 — Vérification en ligne

```bash
gpg --keyserver keyserver.ubuntu.com --search-keys h.aitbah8410@uca.ac.ma
```

<img width="663" height="129" alt="Screenshot etape5 send verify" src="https://github.com/user-attachments/assets/15d4159d-ca19-441a-8b8a-0f6a1c58aa37" />

**Résultat confirmé :**

```
(1) hanane AIT BAH (key) <h.aitbah8410@uca.ac.ma>
    1024 bit RSA key 6C0C6EF384F697B1, created: 2026-03-31
```

✅ La clé est bien publique et accessible sur Internet.

---

## 🔑 Informations de la Clé

| Champ                   | Valeur                                      |
|-------------------------|---------------------------------------------|
| Empreinte (Fingerprint) | `4BE20301D02BBC2492895E0C6C0C6EF384F697B1`  |
| Key ID (long)           | `6C0C6EF384F697B1`                          |
| Sous-clé                | `C49CAEA235B2A3D4`                          |
| Type                    | RSA 1024 bits                               |
| Créée le                | 2026-03-31                                  |
| Expiration              | Aucune                                      |
| Keyserver               | keyserver.ubuntu.com                        |

---

## 🌐 Accès à la clé publique

La clé peut être importée par n'importe qui via :

```bash
gpg --keyserver keyserver.ubuntu.com --recv-keys 6C0C6EF384F697B1
```

Ou via l'URL :
```
https://keyserver.ubuntu.com/pks/lookup?op=get&search=0x6C0C6EF384F697B1
```

---

## ⚠️ Avertissement récurrent

Durant toutes les opérations, le message suivant est apparu :

```
can't connect to 'socket:///home/hanan/.gnupg/log-socket': Connection refused
```

Ce message est un **avertissement non bloquant** lié au démon `gpg-agent`. Il n'affecte pas le fonctionnement de GPG ni la validité des opérations effectuées.

---

## 📚 Commandes de référence rapide

```bash
# Générer une clé
gpg --full-generate-key

# Lister les clés publiques
gpg --list-keys

# Lister les clés privées
gpg --list-secret-keys

# Exporter la clé publique
gpg --armor --export <email> > macle.asc

# Envoyer sur un keyserver
gpg --keyserver keyserver.ubuntu.com --send-keys <KEY_ID>

# Rechercher une clé sur un keyserver
gpg --keyserver keyserver.ubuntu.com --search-keys <email>

# Importer une clé depuis un keyserver
gpg --keyserver keyserver.ubuntu.com --recv-keys <KEY_ID>

# Chiffrer un fichier
gpg --encrypt --recipient <email> fichier.txt

# Signer un fichier
gpg --sign fichier.txt
```

---

<img width="1023" height="353" alt="image" src="https://github.com/user-attachments/assets/b9f3e174-592a-489b-9599-a7fe9187b293" />

Firefox — Utiliser ta clé GPG
Firefox en lui-même ne gère pas directement les clés GPG. Il faut passer par une extension. Voici comment :

<img width="1392" height="513" alt="image" src="https://github.com/user-attachments/assets/563d7b4e-3053-4574-8bcc-9a1cab24fe11" />
<img width="1388" height="921" alt="image" src="https://github.com/user-attachments/assets/1b1145b9-8356-4238-9f1d-63721e41c0b8" />
<img width="1388" height="921" alt="image" src="https://github.com/user-attachments/assets/ec245bf9-8783-47c4-ab37-1a80d90b8515" />
<img width="1091" height="635" alt="image" src="https://github.com/user-attachments/assets/5c1c07d0-097b-4aa5-a372-56ae327fb5c4" />





























