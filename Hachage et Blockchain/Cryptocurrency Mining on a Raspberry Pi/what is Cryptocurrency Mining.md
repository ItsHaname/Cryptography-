# 💰 Cryptocurrency Mining on a Raspberry Pi
> **Basé sur la vidéo YouTube de NetworkChuck**  
> 🎥 `https://www.youtube.com/watch?v=hHtGN_JzoP8`  
> Plateforme : **Raspberry Pi 5 — ARM64**

---

## 📋 Table des Matières

1. [Introduction — De quoi parle la vidéo ?](#1-introduction--de-quoi-parle-la-vidéo-)
2. [C'est quoi la Cryptocurrency ?](#2-cest-quoi-la-cryptocurrency-)
3. [C'est quoi le Cryptocurrency Mining ?](#3-cest-quoi-le-cryptocurrency-mining-)
4. [Pourquoi PAS Bitcoin ?](#4-pourquoi-pas-bitcoin-)
5. [Pourquoi Monero (XMR) ?](#5-pourquoi-monero-xmr-)
6. [Ce dont tu as besoin](#6-ce-dont-tu-as-besoin)
7. [ÉTAPE 1 — Préparer le Raspberry Pi](#7-étape-1--préparer-le-raspberry-pi)
8. [ÉTAPE 2 — Installer XMRig (logiciel de mining)](#8-étape-2--installer-xmrig-logiciel-de-mining)
9. [ÉTAPE 3 — Créer un Wallet Monero](#9-étape-3--créer-un-wallet-monero)
10. [ÉTAPE 4 — Lancer le Mining](#10-étape-4--lancer-le-mining)
11. [Lancer le Mining au démarrage](#11-lancer-le-mining-au-démarrage)
12. [Surveiller ton Mining](#12-surveiller-ton-mining)
13. [Est-ce profitable ?](#13-est-ce-profitable-)
14. [Lien avec le cours Blockchain](#14-lien-avec-le-cours-blockchain)

---

## 1. Introduction — De quoi parle la vidéo ?

La vidéo de **NetworkChuck** (youtuber spécialisé en cybersécurité et réseau) montre **étape par étape** comment transformer un Raspberry Pi en machine de mining de cryptomonnaie.

### Plan de la vidéo (timestamps officiels)

| Timestamp | Contenu |
|-----------|---------|
| `0:00` | Introduction |
| `2:21` | Pourquoi on ne mine PAS Bitcoin |
| `2:48` | Ce dont tu as besoin |
| `3:10` | C'est quoi le Cryptocurrency Mining ? |
| `12:02` | **ÉTAPE 1** — Préparer le Raspberry Pi |
| `14:51` | **ÉTAPE 2** — Installer le logiciel de mining |
| `16:24` | **ÉTAPE 3** — Créer un wallet Monero |
| `18:34` | **ÉTAPE 4** — Lancer le mining avec XMRig |

---

## 2. C'est quoi la Cryptocurrency ?

Une **cryptomonnaie** est une monnaie **numérique et décentralisée** qui utilise la cryptographie pour :

- ✅ Sécuriser les transactions
- ✅ Contrôler la création de nouvelles unités
- ✅ Vérifier le transfert d'actifs
- ✅ Fonctionner sans banque centrale

### Les plus connues

| Crypto | Symbole | Algorithme | Créée en |
|--------|---------|------------|----------|
| Bitcoin | BTC | SHA-256 (PoW) | 2009 |
| **Monero** | **XMR** | **RandomX (CPU)** | **2014** |
| Ethereum | ETH | Ethash → PoS | 2015 |
| Duino-Coin | DUCO | SHA-1 simplifié | 2019 |

---

## 3. C'est quoi le Cryptocurrency Mining ?

Le **mining** (minage) est le processus par lequel des ordinateurs :

```
RÔLE 1 → Valident les transactions sur la blockchain
RÔLE 2 → Sécurisent le réseau contre les fraudes
RÔLE 3 → Créent de nouvelles cryptomonnaies
RÔLE 4 → Maintiennent la décentralisation
```

### Comment ça marche concrètement ?

```
ÉTAPE 1 — Des transactions sont diffusées sur le réseau
              ↓
ÉTAPE 2 — Les mineurs les regroupent dans un bloc
              ↓
ÉTAPE 3 — Chaque mineur cherche un "Nonce" magique :
          SHA256(bloc + Nonce) = 000000...abc
              ↓
ÉTAPE 4 — Le PREMIER qui trouve → valide le bloc
              ↓
ÉTAPE 5 — Il reçoit une récompense en cryptomonnaie 🪙
```

### Le Proof of Work en pratique

```bash
# Ce que fait ton Raspberry Pi en permanence :
i=0
while true; do
    hash=$(sha256sum "transactions + nonce_$i")
    
    if [[ $hash == 000000* ]]; then
        echo "✅ Bloc trouvé ! Récompense reçue !"
        break
    fi
    ((i++))
done
```

> 💡 Le mineur qui trouve en premier remporte la récompense. Les autres ont travaillé pour rien. C'est une vraie course !

---

## 4. Pourquoi PAS Bitcoin ?

NetworkChuck explique clairement dès **2:21** pourquoi on ne mine pas Bitcoin :

```
Raspberry Pi 5        →   ~15 Hash/seconde
Réseau Bitcoin entier →   ~500 000 000 000 000 000 Hash/seconde

Probabilité de trouver un bloc = QUASI ZÉRO ❌
```

### Bitcoin nécessite des ASIC

```
ASIC Antminer S19 Pro
├─ Hashrate    : 110 TH/s  (110 000 000 000 000 H/s)
├─ Prix        : ~2000-3000$
└─ Conso       : 3250 Watts

Raspberry Pi 5
├─ Hashrate    : ~15 H/s
├─ Prix        : ~80€
└─ Conso       : 5-10 Watts
```

> 💡 **Analogie de NetworkChuck** : miner Bitcoin avec un Raspberry Pi c'est comme essayer de gagner la F1 avec un vélo. Techniquement possible de participer, mais jamais de gagner 🚲

---

## 5. Pourquoi Monero (XMR) ?

NetworkChuck choisit **Monero** pour 3 raisons solides :

### Raison 1 — Algorithme RandomX (CPU-friendly)

```
Bitcoin  → SHA-256  → Conçu pour ASIC → ❌ Pi inutile
Monero   → RandomX  → Conçu pour CPU  → ✅ Pi utile !
```

RandomX est spécialement conçu pour être **résistant aux ASIC**. Cela signifie que les CPU normaux (comme celui du Pi) sont compétitifs !

### Raison 2 — Confidentialité totale

Monero est la crypto la plus **privée et anonyme** :

| Crypto | Transactions visibles ? |
|--------|------------------------|
| Bitcoin | ✅ Publiques (tout le monde voit) |
| Ethereum | ✅ Publiques |
| **Monero** | **❌ Privées et anonymes** |

### Raison 3 — Mining démocratisé

```
Monero encourage ACTIVEMENT le mining sur CPU ordinaires
→ Tout le monde peut participer
→ Pas besoin de matériel coûteux
→ Parfait pour Raspberry Pi !
```

---

## 6. Ce dont tu as besoin

### Matériel (2:48 dans la vidéo)

```
✅ Raspberry Pi (4 ou 5 recommandé)
✅ Carte MicroSD (32 GB minimum)
✅ Alimentation officielle
✅ Connexion Internet (Ethernet de préférence)
✅ ☕ Un café (selon NetworkChuck — obligatoire !)
```

### Logiciels

```
✅ Raspberry Pi OS Lite 64-bit (ARM64)
✅ XMRig (logiciel de mining Monero)
✅ Un compte sur un Mining Pool
✅ Un Wallet Monero (portefeuille)
```

---

## 7. ÉTAPE 1 — Préparer le Raspberry Pi

> **Timestamp vidéo : 12:02**

### 1.1 — OS à installer

```
OS      : Raspberry Pi OS Lite 64-bit (Bookworm)
Arch    : ARM64 ✅
SSH     : Activé ✅ (tu travailles sans écran)
```

**Télécharger Raspberry Pi Imager :**
```
https://www.raspberrypi.com/software/
```

### 1.2 — Configuration SSH dans Imager

```
Dans les paramètres avancés (⚙️) :
✅ Enable SSH
✅ Username : pi
✅ Password : [ton mot de passe]
✅ Hostname : raspberrypi
```

### 1.3 — Se connecter via SSH

```bash
# Depuis ton PC
ssh pi@raspberrypi.local

# Vérifier l'architecture ARM64
uname -m
# → aarch64 ✅
```

### 1.4 — Mettre à jour le système

```bash
sudo apt update && sudo apt upgrade -y
```

### 1.5 — Installer les dépendances

```bash
sudo apt install -y git build-essential cmake \
     libssl-dev libhwloc-dev libuv1-dev \
     libgcc-s1 wget
```

---

## 8. ÉTAPE 2 — Installer XMRig (logiciel de mining)

> **Timestamp vidéo : 14:51**

**XMRig** est le logiciel de mining Monero le plus populaire et optimisé pour ARM64.

### Option A — Compiler depuis le source (recommandé ARM64)

```bash
# Cloner XMRig
git clone https://github.com/xmrig/xmrig.git
cd xmrig

# Créer le dossier de compilation
mkdir build && cd build

# Compiler pour ARM64 (Raspberry Pi 5)
cmake .. -DARM_TARGET=8 \
         -DWITH_OPENCL=OFF \
         -DWITH_CUDA=OFF
         
# Compiler avec les 4 cœurs du Pi
make -j4

# Vérifier que ça fonctionne
./xmrig --version
```

### Option B — Télécharger le binaire précompilé

```bash
# Aller sur GitHub releases XMRig
# https://github.com/xmrig/xmrig/releases

# Télécharger la version ARM64 Linux
wget https://github.com/xmrig/xmrig/releases/latest/download/xmrig-linux-static-aarch64.tar.gz

# Extraire
tar -xvf xmrig-linux-static-aarch64.tar.gz
cd xmrig-*

# Vérifier
./xmrig --version
```

---

## 9. ÉTAPE 3 — Créer un Wallet Monero

> **Timestamp vidéo : 16:24**

Tu as besoin d'un **wallet** (portefeuille) pour recevoir tes récompenses de mining.

### Option 1 — Wallet Web (plus simple)

```
1. Va sur : https://mymonero.com
2. Clique "Create Account"
3. SAUVEGARDE ta seed phrase (13 mots) ← TRÈS IMPORTANT !
4. Copie ton adresse XMR (commence par 4...)
```

### Option 2 — Wallet officiel sur le Pi

```bash
# Télécharger Monero CLI
wget https://downloads.getmonero.org/cli/linuxarm8

# Extraire
tar -xvf monero-linux-armv8-*.tar.bz2
cd monero-*

# Créer un nouveau wallet
./monero-wallet-cli --generate-new-wallet mon_wallet
```

### ⚠️ ATTENTION — Sécurité du wallet

```
🔐 Sauvegarde OBLIGATOIRE :
   → Seed phrase (25 mots)
   → Adresse publique XMR
   → Mot de passe du wallet

❌ Ne JAMAIS partager :
   → Ta seed phrase
   → Ta clé privée
   → Ton mot de passe
```

---

## 10. ÉTAPE 4 — Lancer le Mining

> **Timestamp vidéo : 18:34**

### 10.1 — Choisir un Mining Pool

NetworkChuck recommande de rejoindre un **pool** (groupe de mineurs) :

| Pool | URL | Frais | Minimum paiement |
|------|-----|-------|-----------------|
| **SupportXMR** | supportxmr.com | 0.6% | 0.1 XMR |
| MineXMR | minexmr.com | 1% | 0.004 XMR |
| MoneroOcean | moneroocean.stream | 0% | 0.003 XMR |
| Nanopool | xmr.nanopool.org | 1% | 0.1 XMR |

### 10.2 — Créer le fichier de configuration

```bash
# Dans le dossier xmrig
nano config.json
```

```json
{
    "autosave": true,
    "donate-level": 1,
    "pools": [
        {
            "url": "pool.supportxmr.com:443",
            "user": "TON_ADRESSE_XMR_ICI",
            "pass": "raspberry_pi_5",
            "tls": true,
            "keepalive": true
        }
    ],
    "cpu": {
        "enabled": true,
        "max-threads-hint": 75,
        "priority": 2
    },
    "log-file": "/home/pi/xmrig.log"
}
```

> 💡 `max-threads-hint: 75` = utilise 75% du CPU pour éviter la surchauffe

### 10.3 — Lancer XMRig

```bash
# Depuis le dossier xmrig/build (ou xmrig-*)
./xmrig --config=../config.json
```

### 10.4 — Ce que tu verras dans le terminal

```
* POOL #1    pool.supportxmr.com:443
* CPU        ARM Cortex-A76, 4 cores
* THREADS    3, RandomX

[2024-03-06 12:00:01] READY threads 3/4 (3) huge pages 100%
[2024-03-06 12:00:10] speed  10s: 75.3 H/s  accepted: 0/0
[2024-03-06 12:00:20] speed  10s: 78.1 H/s  accepted: 1/1 ✅
[2024-03-06 12:00:30] speed  10s: 80.2 H/s  accepted: 2/2 ✅
```

| Info | Signification |
|------|---------------|
| `H/s` | Hash par seconde (ta puissance de mining) |
| `accepted` | Shares acceptées par le pool ✅ |
| `rejected` | Shares rejetées ❌ |
| `huge pages 100%` | Optimisation mémoire active ✅ |

---

## 11. Lancer le Mining au démarrage

> NetworkChuck mentionne cette option dans sa description

Pour que le mining **démarre automatiquement** quand tu allumes le Pi :

```bash
# Créer un service systemd
sudo nano /etc/systemd/system/xmrig.service
```

```ini
[Unit]
Description=XMRig Monero Miner
After=network.target

[Service]
ExecStart=/home/pi/xmrig/build/xmrig --config=/home/pi/xmrig/config.json
Restart=always
User=pi
Nice=10

[Install]
WantedBy=multi-user.target
```

```bash
# Activer et démarrer le service
sudo systemctl enable xmrig
sudo systemctl start xmrig

# Vérifier le statut
sudo systemctl status xmrig

# Voir les logs en direct
journalctl -u xmrig -f
```

---

## 12. Surveiller ton Mining

### 12.1 — Surveiller la température (CRITIQUE !)

```bash
# Température en temps réel
watch -n 2 vcgencmd measure_temp

# Ou avec le script complet :
nano monitor_mining.sh
```

```bash
#!/bin/bash
# Moniteur Mining — Raspberry Pi 5
while true; do
    clear
    echo "🍓 RASPBERRY PI 5 — XMRIG MONITOR"
    echo "════════════════════════════════════"
    TEMP=$(vcgencmd measure_temp | cut -d= -f2)
    echo "🌡️  Température : $TEMP"
    
    # Alerte si trop chaud
    TEMP_NUM=$(echo $TEMP | cut -d\' -f1)
    if (( $(echo "$TEMP_NUM > 75" | bc -l) )); then
        echo "⚠️  ATTENTION : Température élevée !"
    fi
    
    echo "💾 RAM : $(free -h | grep Mem | awk '{print $3" / "$2}')"
    echo "⚡ CPU : $(top -bn1 | grep 'Cpu(s)' | awk '{print $2}')%"
    echo "🕐 Uptime : $(uptime -p)"
    echo "════════════════════════════════════"
    
    # Logs XMRig récents
    echo "📊 Dernières stats XMRig :"
    tail -5 /home/pi/xmrig.log 2>/dev/null
    
    sleep 5
done
```

```bash
chmod +x monitor_mining.sh
./monitor_mining.sh
```

### 12.2 — Températures recommandées

```
< 60°C   → ✅ Parfait
60-70°C  → 🔶 Normal sous charge
70-80°C  → ⚠️ Améliorer refroidissement
> 80°C   → ❌ DANGER — réduire max-threads-hint
```

### 12.3 — Voir tes stats sur le pool

```
Va sur : https://supportxmr.com
Colle ton adresse XMR dans la barre de recherche
→ Tu vois tes hashrates, shares, et revenus en temps réel !
```

---

## 13. Est-ce profitable ?

NetworkChuck est honnête sur ce point :

### Hashrate estimé Pi 5

```
Raspberry Pi 5 (4 cœurs, 75%) → ~60-80 H/s avec RandomX
```

### Revenus estimés

| Matériel | Hashrate | Revenu/jour | Revenu/mois |
|----------|----------|-------------|-------------|
| Pi 5 (1 unité) | 75 H/s | ~$0.001 | ~$0.03 |
| 10x Pi 5 | 750 H/s | ~$0.01 | ~$0.30 |
| PC gaming | 5000 H/s | ~$0.07 | ~$2.10 |
| Serveur dédié | 50000 H/s | ~$0.70 | ~$21 |

### Consommation électrique Pi 5

```
Conso mining    : ~8 Watts
Coût/heure      : ~0.001€ (à 0.13€/kWh)
Coût/mois       : ~0.06€

Revenu/mois     : ~0.03€
Perte/mois      : ~0.03€ ❌
```

### La vraie valeur — L'apprentissage !

> 💡 Comme le dit NetworkChuck : **"C'est crazy fun !"**

```
Ce que tu gagnes vraiment :
✅ Comprendre le mining en pratique
✅ Apprendre Linux et SSH
✅ Expérimenter la blockchain
✅ Comprendre les cryptomonnaies
✅ Projet fun pour ton CV / portfolio
```

---

## 14. Lien avec le cours Blockchain

> **Cours Pr. EL KIRAM — Hachage & Blockchain**

La vidéo de NetworkChuck illustre **parfaitement** tous les concepts du cours :

| Concept du cours | Ce que fait XMRig sur le Pi |
|-----------------|---------------------------|
| Fonction de hachage | RandomX hash en permanence |
| Propriété sens unique | Impossible de deviner le Nonce |
| Proof of Work (PoW) | XMRig cherche le bon Nonce |
| Nonce | XMRig l'incrémente des millions de fois |
| Bloc | Les transactions groupées à valider |
| Merkle Root | Résumé des transactions dans le bloc |
| Décentralisation | Le Pi est un nœud parmi des milliers |
| Récompense | XMR reçus si share acceptée |
| Mining Pool | Groupe de nœuds qui collaborent |

### Le script du cours (Slide 13) VS XMRig

```bash
# COURS (simplifié) :
while true; do
    hash=$(sha256sum "bloc + nonce")
    if [[ $hash == 0* ]]; then
        echo "✅ Trouvé !"
        break
    fi
done

# XMRIG (en vrai) :
# Fait exactement la même chose MAIS :
# → Algorithme RandomX au lieu de SHA-256
# → Milliards d'essais par seconde
# → Communique avec un pool en réseau
# → Gère les récompenses automatiquement
```

---

## 🎯 Résumé Final

```
VIDÉO NETWORKCHUCK — EN 4 ÉTAPES :

ÉTAPE 1 → Installer Raspberry Pi OS Lite 64-bit + SSH
ÉTAPE 2 → Compiler et installer XMRig (miner Monero)
ÉTAPE 3 → Créer un wallet Monero (adresse XMR)
ÉTAPE 4 → Lancer le mining avec un pool (supportxmr.com)

RÉSULTAT :
🍓 Raspberry Pi 5 mine du Monero 24h/24
📊 Stats visibles sur le dashboard du pool
🌡️ Température à surveiller (~70°C sous charge)
💰 Revenus symboliques mais expérience précieuse !
```

---

## 🔗 Ressources utiles

| Ressource | Lien |
|-----------|------|
| Vidéo NetworkChuck | `https://youtube.com/watch?v=hHtGN_JzoP8` |
| XMRig GitHub | `https://github.com/xmrig/xmrig` |
| Pool SupportXMR | `https://supportxmr.com` |
| Wallet MyMonero | `https://mymonero.com` |
| Raspberry Pi Imager | `https://raspberrypi.com/software` |
| Prix Monero (XMR) | `https://coinmarketcap.com/currencies/monero` |

---

*Basé sur la vidéo YouTube de NetworkChuck*  
*Cours Hachage & Blockchain — Pr. EL KIRAM*  
*Plateforme : Raspberry Pi 5 — Raspberry Pi OS Lite 64-bit (ARM64)*
