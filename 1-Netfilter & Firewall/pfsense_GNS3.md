# 🔒 Configuration d'un Pare-feu pfSense
### Rapport de Travaux Pratiques — Sécurité Réseau

> **Objectif :** Configurer un pare-feu pfSense pour sécuriser une architecture réseau composée d'une DMZ, d'un réseau interne, d'un réseau d'administration et d'un réseau WAN (clients externes).

---

## 📋 Table des Matières

1. [Réalisation de la Topologie](#1-réalisation-de-la-topologie)
2. [Accès à l'Interface pfSense](#2-accès-à-linterface-pfsense)
3. [Étape 1 — Règles LAN (Admin)](#3-étape-1--règles-lan-admin)
4. [Étape 2 — Règles OPT1 (DMZ)](#4-étape-2--règles-opt1-dmz)
5. [Étape 3 — Règles WAN](#5-étape-3--règles-wan)
6. [Étape 4 — Règles OPT2 (Réseau Interne)](#6-étape-4--règles-opt2-réseau-interne)
7. [Étape 5 — NAT (Network Address Translation)](#7-étape-5--nat-network-address-translation)
8. [Étape 6 — Règles LAN Avancées & Sécurité](#8-étape-6--règles-lan-avancées--sécurité)
9. [Étape 7 — Tests de Connectivité](#9-étape-7--tests-de-connectivité)
10. [Récapitulatif Final](#10-récapitulatif-final)

---

## 1. Réalisation de la Topologie

### Architecture Réseau

| Interface | Réseau | Rôle |
|-----------|--------|------|
| **WAN** (em0) | `172.17.0.0/24` | Clients externes — Client1 : `172.17.0.10`, Client2 : `172.17.0.11` |
| **LAN / Admin** (em2) | `192.168.1.0/24` | Administration + Supervision |
| **OPT1 / DMZ** (em1) | `192.168.0.0/24` | Serveurs publics (Web, FTP, DNS, Mail) |
| **OPT2 / Interne** (em3) | `192.168.2.0/24` | Réseau interne |

### Adressage des Serveurs DMZ

| Serveur | Adresse IP | Service | Port |
|---------|-----------|---------|------|
| Serveur Web | `192.168.0.3` | Apache HTTP/HTTPS | 8000 / 4443 |
| Serveur FTP | `192.168.0.2` | vsftpd | 21 |
| Serveur DNS | `192.168.0.5` | BIND9 | 53 |
| Serveur Mail | `192.168.0.4` | Postfix SMTP | 25 |

<img width="1120" height="813" alt="Topologie réseau GNS3" src="https://github.com/user-attachments/assets/216e553d-d564-48a1-bcd4-53560197e062" />

<img width="685" height="325" alt="Interfaces pfSense" src="https://github.com/user-attachments/assets/c2c03d7e-119b-42b8-9d27-d08f8ccbd701" />

---

## 2. Accès à l'Interface pfSense

La VM **Debian-Parefeu** est configurée avec l'IP `192.168.1.2` sur le réseau Admin.

### Configuration réseau permanente (`/etc/network/interfaces`)

```
auto lo
iface lo inet loopback

auto enp2s0
iface enp2s0 inet static
    address 192.168.1.2
    netmask 255.255.255.0
    gateway 192.168.1.1
```

Redémarrer le réseau :
```bash
systemctl restart networking
```

Accès depuis Firefox sur la VM Debian :
```
https://192.168.1.1
```

| Login | Password |
|-------|----------|
| `admin` | `pfsense` |

---

## 3. Étape 1 — Règles LAN (Admin)

**Chemin :** `Firewall → Rules → LAN`

- ✅ **Anti-Lockout Rule** — protège l'accès à l'interface pfSense (ne pas modifier)
- ✅ **Default allow LAN to any (IPv4)** — autorise tout le trafic LAN sortant
- ✅ **Default allow LAN IPv6** — idem en IPv6

<img width="1212" height="507" alt="Règles LAN pfSense" src="https://github.com/user-attachments/assets/a6ebcc91-e4bf-47c4-a5c8-54e53f58cad2" />

---

## 4. Étape 2 — Règles OPT1 (DMZ)

**Chemin :** `Firewall → Rules → OPT1`

---

### Règle 1 — HTTP (port 80) vers Serveur Web

| Champ | Valeur |
|-------|--------|
| Action | Pass |
| Protocol | TCP |
| Source | Any |
| Destination | `192.168.0.3 /32` |
| Port | 80 (HTTP) |
| Description | Autoriser HTTP vers Web |

<img width="1193" height="681" alt="Création règle HTTP DMZ" src="https://github.com/user-attachments/assets/c9b91400-5ca3-4289-8990-ace4d2948b8d" />

<img width="1142" height="641" alt="Règle HTTP DMZ sauvegardée" src="https://github.com/user-attachments/assets/e135f16c-5bb7-4bee-a338-363b13b646b8" />

---

### Règle 2 — HTTPS (port 443) vers Serveur Web

| Champ | Valeur |
|-------|--------|
| Action | Pass |
| Protocol | TCP |
| Source | Any |
| Destination | `192.168.0.3 /32` |
| Port | 443 (HTTPS) |
| Description | Autoriser HTTPS vers Web |

<img width="1140" height="664" alt="Création règle HTTPS DMZ" src="https://github.com/user-attachments/assets/4ce555df-61f5-4982-9815-bc4699bac63a" />

<img width="1139" height="568" alt="Règle HTTPS DMZ sauvegardée" src="https://github.com/user-attachments/assets/a1e0a59c-7678-4c8c-974a-9021b67e60e1" />

---

### Règle 3 — FTP (port 21) vers Serveur FTP

| Champ | Valeur |
|-------|--------|
| Action | Pass |
| Protocol | TCP |
| Source | Any |
| Destination | `192.168.0.2 /32` |
| Port | 21 (FTP) |
| Description | Autoriser FTP |

<img width="1139" height="568" alt="Création règle FTP DMZ" src="https://github.com/user-attachments/assets/a314cc06-07b2-4176-90b6-a7acdec46351" />

<img width="1165" height="517" alt="Règle FTP DMZ" src="https://github.com/user-attachments/assets/1bae4861-b73d-4034-bdb1-01574a60d7d7" />

---

### Règle 4 — DNS (port 53) vers Serveur DNS

| Champ | Valeur |
|-------|--------|
| Action | Pass |
| Protocol | **TCP/UDP** |
| Source | Any |
| Destination | `192.168.0.5 /32` |
| Port | 53 (DNS) |
| Description | Autoriser DNS |

> ⚠️ Le DNS utilise **TCP et UDP** — sélectionner `TCP/UDP`.

<img width="1145" height="563" alt="Règle DNS DMZ" src="https://github.com/user-attachments/assets/6012bd99-dc56-49c8-9dd9-9ef2f0cbc97a" />

---

### Règle 5 — SMTP (port 25) vers Serveur Mail

| Champ | Valeur |
|-------|--------|
| Action | Pass |
| Protocol | TCP |
| Source | Any |
| Destination | `192.168.0.4 /32` |
| Port | 25 (SMTP) |
| Description | Autoriser Mail SMTP |

<img width="1138" height="495" alt="Création règle SMTP DMZ" src="https://github.com/user-attachments/assets/9a0345c8-2aba-4b5e-996b-4c3a02984629" />

<img width="1143" height="466" alt="Règle SMTP DMZ sauvegardée" src="https://github.com/user-attachments/assets/61e50bdc-4e63-4ce9-aa0c-06154ed7a061" />

---

### ✅ Récapitulatif OPT1 (DMZ) — Toutes les règles créées

<img width="594" height="243" alt="Liste complète règles DMZ" src="https://github.com/user-attachments/assets/fa48336a-03a7-4cb9-8455-da63ebfbe33e" />

---

### Règle Flottante — Limitation du Ping vers FTP (2 req/sec)

**Chemin :** `Firewall → Rules → Floating`

| Champ | Valeur |
|-------|--------|
| Action | Pass |
| Interface | OPT1 |
| Direction | Any |
| Protocol | ICMP — Echo Request |
| Destination | `192.168.0.2 /32` |
| Max. src. states | 2 |
| Description | Ping limité 2/sec FTP |

<img width="1160" height="224" alt="Règle limitation ping FTP" src="https://github.com/user-attachments/assets/2c275de5-6a88-4210-bf24-12c27816d36d" />

---

## 5. Étape 3 — Règles WAN

**Chemin :** `Firewall → Rules → WAN`

Ces règles permettent aux clients externes (`172.17.0.0/24`) d'accéder aux services de la DMZ.

---

### Règle WAN 1 — HTTP vers Serveur Web

<img width="1154" height="439" alt="Règle WAN HTTP" src="https://github.com/user-attachments/assets/76dc2998-af9c-4fcf-b10d-d7a446bdbffb" />

---

### Règle WAN 2 — HTTPS vers Serveur Web

<img width="1137" height="609" alt="Règle WAN HTTPS" src="https://github.com/user-attachments/assets/a4cb530a-5a4d-4ff8-930a-e152352c7209" />

---

### Règle WAN 3 — FTP vers Serveur FTP

<img width="1137" height="609" alt="Règle WAN FTP" src="https://github.com/user-attachments/assets/ce57c43c-213d-449b-aebd-a00dc16481da" />

---

### Règle WAN 4 — DNS vers Serveur DNS

<img width="1154" height="586" alt="Règle WAN DNS" src="https://github.com/user-attachments/assets/8f1cffa5-24a1-4d82-bc2a-66615ed01032" />

---

### Règle WAN 5 — SMTP vers Serveur Mail

<img width="1137" height="609" alt="Règle WAN SMTP" src="https://github.com/user-attachments/assets/cc9eba03-b638-4f82-8d4c-4cd675e79f67" />

---

### ✅ Récapitulatif WAN — Toutes les règles créées

<img width="1165" height="323" alt="Liste complète règles WAN" src="https://github.com/user-attachments/assets/4491fbf6-acf8-4cf5-b0cb-e64be48339db" />

---

## 6. Étape 4 — Règles OPT2 (Réseau Interne)

**Chemin :** `Firewall → Rules → OPT2`

| # | Protocol | Source | Destination | Description |
|---|----------|--------|-------------|-------------|
| 1 | TCP | `192.168.2.0 /24` | `192.168.0.0 /24` | Interne vers DMZ |
| 2 | Any | `192.168.2.0 /24` | Any | Interne vers Internet |

<img width="1148" height="300" alt="Règles OPT2 réseau interne" src="https://github.com/user-attachments/assets/31b14db8-8b4b-43c2-9831-15d6f182eaf2" />

---

## 7. Étape 5 — NAT (Network Address Translation)

**Chemin :** `Firewall → NAT → Port Forward`

> ⚠️ Le serveur Web écoute sur les ports **8000** (HTTP) et **4443** (HTTPS) en local. Le NAT traduit les ports standard 80/443 vers ces ports internes.

| # | Interface | Protocol | Port WAN | Serveur DMZ | Port Local | Description |
|---|-----------|----------|----------|-------------|------------|-------------|
| 1 | WAN | TCP | 80 | `192.168.0.3` | **8000** | NAT HTTP vers Web |
| 2 | WAN | TCP | 443 | `192.168.0.3` | **4443** | NAT HTTPS vers Web |
| 3 | WAN | TCP | 21 | `192.168.0.2` | 21 | NAT FTP |
| 4 | WAN | TCP/UDP | 53 | `192.168.0.5` | 53 | NAT DNS |
| 5 | WAN | TCP | 25 | `192.168.0.4` | 25 | NAT SMTP vers Mail |

<img width="1162" height="491" alt="Règles NAT Port Forward" src="https://github.com/user-attachments/assets/cb08cb45-7854-4c71-95b3-24b92d42e37d" />

---

## 8. Étape 6 — Règles LAN Avancées & Sécurité

### 8.1 SSH Admin + Ping Supervision

**Chemin :** `Firewall → Rules → LAN`

| Règle | Source | Destination | Port | Description |
|-------|--------|-------------|------|-------------|
| SSH Admin | `192.168.1.2 /32` | This Firewall | 22 | SSH Admin vers pfSense |
| Ping Supervision | `192.168.1.3 /32` | This Firewall | ICMP | Ping Supervision vers pfSense |

<img width="1168" height="365" alt="Règles LAN SSH et Supervision" src="https://github.com/user-attachments/assets/59aee079-63be-4795-bde7-dc842d15f5ac" />

---

### 8.2 Suivi de Connexions (Stateful Firewall)

pfSense est un pare-feu **stateful** par défaut. Il gère automatiquement les états `ESTABLISHED` et `RELATED`. ✅ Aucune règle supplémentaire nécessaire.

---

### 8.3 Anti-Spoofing

**Chemin :** `Interfaces → WAN`

- ✅ **Block private networks and loopback addresses**
- ✅ **Block bogon networks**

<img width="1154" height="331" alt="Anti-spoofing WAN option 1" src="https://github.com/user-attachments/assets/a25b0703-f0f0-446b-b469-74d2d99ab4db" />

<img width="1168" height="131" alt="Anti-spoofing WAN option 2" src="https://github.com/user-attachments/assets/64d098b8-258e-4311-8e5c-4c6033f42a59" />

<img width="1151" height="597" alt="Résultat anti-spoofing appliqué" src="https://github.com/user-attachments/assets/01ad2d9e-9c1b-4191-959d-ddb182f7bf1b" />

---

### 8.4 Protection contre les Scans de Port

**Chemin :** `System → Advanced → Firewall & NAT` → Firewall Optimization Options → **Conservative**

**Chemin :** `Firewall → Rules → Floating`

| Champ | Valeur |
|-------|--------|
| Action | Block |
| Interface | WAN — Direction : in |
| Protocol | TCP |
| Max. src. conn. rate | 10 / second |
| Description | Protection scan de port |

<img width="1170" height="398" alt="Règle protection scan de port" src="https://github.com/user-attachments/assets/3250de73-de02-4e14-981e-216c6083f094" />

---

### 8.5 Protection contre le Ping de la Mort

**Chemin :** `Firewall → Rules → Floating`

| Champ | Valeur |
|-------|--------|
| Action | Block |
| Interface | WAN — Direction : in |
| Protocol | ICMP — Echo Request |
| Max. src. states | 1 |
| Description | Protection ping de mort |

<img width="1170" height="398" alt="Règle protection ping de mort" src="https://github.com/user-attachments/assets/6f315fc1-a6bc-4a87-a0d2-44085303c253" />

---

## 9. Étape 7 — Tests de Connectivité

### Tests ping depuis la VM Debian (`192.168.1.2`)

```bash
ping 192.168.0.3   # Serveur Web
ping 192.168.0.2   # Serveur FTP
ping 192.168.0.5   # Serveur DNS
ping 192.168.0.4   # Serveur Mail
```

<img width="703" height="533" alt="Résultats tests ping DMZ" src="https://github.com/user-attachments/assets/20543ce7-d646-4a83-9e0c-74d82cb0b134" />

### 🎉 Tous les serveurs DMZ répondent avec succès !

| Destination | IP | Temps de réponse | Statut |
|-------------|-----|-----------------|--------|
| Serveur Web | `192.168.0.3` | 2.12 ms | ✅ OK |
| Serveur FTP | `192.168.0.2` | 1.29 ms | ✅ OK |
| Serveur DNS | `192.168.0.5` | 2.21 ms | ✅ OK |
| Serveur Mail | `192.168.0.4` | 1.92 ms | ✅ OK |

---

## 10. Récapitulatif Final

| Étape | Configuration | Statut |
|-------|--------------|--------|
| 1 | Règles LAN Admin (SSH + Ping Supervision) | ✅ |
| 2 | Règles OPT1 DMZ (HTTP, HTTPS, FTP, DNS, SMTP) | ✅ |
| 3 | Règles WAN (HTTP, HTTPS, FTP, DNS, SMTP) | ✅ |
| 4 | Règles OPT2 Réseau Interne (DMZ + Internet) | ✅ |
| 5 | NAT Port Forward (HTTP→8000, HTTPS→4443, FTP, DNS, SMTP) | ✅ |
| 6 | SSH Admin + Ping Supervision | ✅ |
| 7 | Suivi de connexions stateful | ✅ (natif pfSense) |
| 8 | Anti-Spoofing WAN (Private + Bogon networks) | ✅ |
| 9 | Limitation ping 2/sec vers FTP | ✅ |
| 10 | Protection scan de port | ✅ |
| 11 | Protection ping de mort | ✅ |
| 12 | Tests de connectivité (ping DMZ) | ✅ |

---

*Rapport réalisé dans le cadre d'un TP de configuration de pare-feu pfSense — Sécurité Réseau.*
