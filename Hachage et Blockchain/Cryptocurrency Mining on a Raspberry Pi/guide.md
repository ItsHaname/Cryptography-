# 🍓 Cryptocurrency Mining on Raspberry Pi 5

![Raspberry Pi](https://img.shields.io/badge/Raspberry%20Pi-5-C51A4A?style=for-the-badge&logo=raspberrypi&logoColor=white)
![OS](https://img.shields.io/badge/OS-Raspberry%20Pi%20OS%20Lite%2064--bit-A22846?style=for-the-badge&logo=linux&logoColor=white)
![Architecture](https://img.shields.io/badge/Architecture-ARM64-0078D4?style=for-the-badge)
![Crypto](https://img.shields.io/badge/Crypto-Monero%20XMR-FF6600?style=for-the-badge)
![Miner](https://img.shields.io/badge/Miner-XMRig-00ADEF?style=for-the-badge)

> **Projet académique — Cours Hachage & Blockchain — Pr. EL KIRAM**

> Inspiré de la vidéo NetworkChuck : https://www.youtube.com/watch?v=hHtGN_JzoP8

---

## 📋 Table des Matières

- [À propos du projet](#-à-propos-du-projet)
- [Théorie — Hachage](#-théorie--hachage)
- [Théorie — Blockchain](#-théorie--blockchain)
- [Théorie — Proof of Work](#-théorie--proof-of-work)
- [Théorie — Cryptocurrency Mining](#-théorie--cryptocurrency-mining)
- [Matériel requis](#-matériel-requis)
- [Étape 1 — Installation OS](#-étape-1--installation-os)
- [Étape 2 — Connexion SSH](#-étape-2--connexion-ssh)
- [Étape 3 — Configuration système](#-étape-3--configuration-système)
- [Étape 4 — Installation XMRig](#-étape-4--installation-xmrig)
- [Étape 5 — Wallet Monero](#-étape-5--wallet-monero)
- [Étape 6 — Lancer le Mining](#-étape-6--lancer-le-mining)
- [Étape 7 — Mining au démarrage](#-étape-7--mining-au-démarrage)
- [Scripts pratiques](#-scripts-pratiques)
- [Surveillance et monitoring](#-surveillance-et-monitoring)
- [Est-ce rentable ?](#-est-ce-rentable-)
- [Lien avec le cours](#-lien-avec-le-cours)
- [Ressources](#-ressources)

---

## 🎯 À propos du projet

Ce projet transforme un **Raspberry Pi 5** en machine de minage de cryptomonnaie Monero (XMR), en appliquant concrètement tous les concepts du cours Hachage & Blockchain.

### Objectifs pédagogiques

| Objectif | Description |
|----------|-------------|
| 🔐 Hachage | Voir SHA-256 / RandomX en action réelle |
| ⛏️ PoW | Simuler et observer le Proof of Work |
| 🔗 Blockchain | Voir comment les blocs sont validés |
| 🍓 Linux ARM64 | Maîtriser Raspberry Pi via SSH |
| 💰 Cryptos | Expérimenter le minage réel avec XMRig |

### Architecture

```
┌──────────────────────────────────────────┐
│            RASPBERRY PI 5                │
│                                          │
│  [XMRig] ──▶ [RandomX Hash] ──▶ [Pool]  │
│                                    │     │
│              [MINING POOL]         │     │
│           Récompense partagée ◀────┘     │
│                    │                     │
│              [WALLET XMR]                │
│         Récompenses reçues               │
└──────────────────────────────────────────┘
```

---

## 🔐 Théorie — Hachage

> **Cours Pr. EL KIRAM — Axe 1 & 2**

### Définition

Une **fonction de hachage** transforme n'importe quelle entrée en une empreinte (hash) de taille fixe.

```
"Bonjour"          ──▶  [SHA-256]  ──▶  7c6f4d3e2b1a... (64 car.)
"Fichier de 10 Go" ──▶  [SHA-256]  ──▶  9a8b7c6d5e4f... (64 car.)
```

### Les 4 propriétés fondamentales

**1. Sens unique (One-Way)**
```bash
echo -n "Bonjour" | sha256sum   # Calculer → facile ✅
# Retrouver M depuis F(M)        → IMPOSSIBLE ❌
```

**2. Non-collision**
```
M != M'  ──▶  F(M) != F(M')    toujours vrai
```

**3. Effet Avalanche**
```bash
echo -n "Bonjour" | sha256sum   # → 7c6f4d3e...
echo -n "bonjour" | sha256sum   # → 9a8b7c6d...  totalement différent !
```

**4. Déterminisme**
```bash
echo -n "Bonjour" | sha256sum   # Aujourd'hui  → 7c6f4d3e...
echo -n "Bonjour" | sha256sum   # Dans 10 ans  → 7c6f4d3e... (identique)
```

### Les algorithmes du cours

| Algorithme | Taille | Statut |
|-----------|--------|--------|
| MD5 | 128 bits (32 car.) | ⚠️ Intégrité fichiers uniquement |
| SHA-1 | 160 bits (40 car.) | ⚠️ Déconseillé depuis 2017 |
| **SHA-256** | **256 bits (64 car.)** | **✅ Standard actuel — Bitcoin** |
| SHA-512 | 512 bits (128 car.) | ✅ Haute sécurité |
| RandomX | 256 bits | ✅ Monero — optimisé CPU |

### Tester sur Raspberry Pi

```bash
echo -n "Bonjour" | md5sum      # 128 bits — 32 caractères
echo -n "Bonjour" | sha1sum     # 160 bits — 40 caractères
echo -n "Bonjour" | sha256sum   # 256 bits — 64 caractères (Bitcoin)
echo -n "Bonjour" | sha512sum   # 512 bits — 128 caractères

# Vérifier l'intégrité d'un fichier
md5sum fichier.txt > fichier.md5
md5sum -c fichier.md5
# → fichier.txt: OK ✅
```

---

## 🔗 Théorie — Blockchain

> **Cours Pr. EL KIRAM — Axe 4**

### Définition

> **"La blockchain est une base de données distribuée, décentralisée et immuable qui enregistre des transactions dans des blocs liés entre eux."**

```
DISTRIBUÉE    →  copiée sur des milliers de machines
DÉCENTRALISÉE →  personne ne contrôle seul
IMMUABLE      →  impossible de modifier ce qui est écrit
```

### Structure d'un bloc

```
Bloc N
├─ EN-TÊTE
│   ├─ Version              → version du protocole
│   ├─ Hash bloc précédent  → lien avec la chaîne (CRUCIAL)
│   ├─ Merkle Root          → résumé de toutes les transactions
│   ├─ Timestamp            → date et heure exacte
│   ├─ Bits (difficulté)    → combien de zéros requis dans le hash
│   └─ Nonce                → nombre trouvé par le mineur ⛏️
├─ CORPS
│   └─ Liste des transactions
└─ Hash du bloc             → commence par 000000...
```

### La chaîne de blocs

```
[BLOC 1 Genesis]──▶[BLOC 2]──▶[BLOC 3]──▶[BLOC 4]
 Hash: AAA123       Hash préc: AAA123
                    Hash: BBB456
```

> Modifier Bloc 2 → hash change → Bloc 3 invalide → toute la chaîne cassée → réseau détecte immédiatement !

### Le Merkle Root

```
      [Merkle Root]
            │
     ┌──────┴──────┐
   [H12]         [H34]
  ┌──┴──┐       ┌──┴──┐
[H1]  [H2]   [H3]  [H4]
  │     │      │     │
 T1    T2     T3    T4
```

Si T2 modifiée → H2 change → H12 change → Merkle Root change → **bloc invalidé**.

### Les 4 caractéristiques

| Caractéristique | Description |
|----------------|-------------|
| **Décentralisation** | Pas d'autorité centrale, réseau valide collectivement |
| **Immutabilité** | Bloc validé = quasi impossible à modifier |
| **Transparence** | Transactions visibles publiquement |
| **Sécurité** | Fonctions de hachage + mécanismes de consensus |

### Types de blockchain

| Type | Accès | Exemple |
|------|-------|---------|
| 🔓 Publique | Tout le monde | Bitcoin, Monero |
| 🔐 Privée | Une organisation | Hyperledger |
| 🤝 Consortium | Plusieurs orgs | R3 Corda |

---

## ⛏️ Théorie — Proof of Work

> **Cours Pr. EL KIRAM — Axe 3**

### Principe

```
ÉTAPE 1 → Transactions regroupées dans un bloc
              ↓
ÉTAPE 2 → Mineurs cherchent un Nonce tel que :
          SHA256(bloc + Nonce) commence par 000000...
              ↓
ÉTAPE 3 → Le PREMIER qui trouve → valide le bloc
              ↓
ÉTAPE 4 → Tout le réseau vérifie facilement
              ↓
ÉTAPE 5 → Bloc ajouté à la blockchain
              ↓
ÉTAPE 6 → Mineur reçoit une récompense 🪙
```

### Simulation bash — Slide 13 du cours

```bash
#!/bin/bash
i=0
while true; do
    content=$(head -c 32 /dev/urandom | base64)
    echo "$content" > test.txt
    hash=$(sha256sum test.txt | awk '{print $1}')
    if [[ $hash == 0* ]]; then
        echo "✅ Trouvé après $i essais ! Hash: $hash"
        break
    fi
    ((i++))
done
```

### PoW vs PoS

| Critère | PoW (Bitcoin) | PoS (Ethereum) |
|---------|--------------|----------------|
| Validation | Résoudre calcul | Bloquer des cryptos |
| Énergie | ❌ Très élevée | ✅ Très faible |
| Matériel | ❌ ASIC coûteux | ✅ PC normal |
| Punition | Électricité perdue | Slashing (perte mise) |

---

## 💰 Théorie — Cryptocurrency Mining

### C'est quoi ?

Le **mining** est le processus par lequel des ordinateurs valident les transactions et créent de nouvelles cryptomonnaies en échange d'une récompense.

### Pourquoi Monero (XMR) sur Raspberry Pi ?

```
Bitcoin  → SHA-256  → Conçu pour ASIC → ❌ Pi inutile
Monero   → RandomX  → Conçu pour CPU  → ✅ Pi compétitif !
```

| Crypto | Algorithme | Pi adapté ? | Raison |
|--------|-----------|------------|--------|
| Bitcoin BTC | SHA-256 | ❌ Non | Nécessite ASIC |
| Ethereum ETH | PoS | ❌ Non | Pas de mining |
| **Monero XMR** | **RandomX** | **✅ Oui** | **CPU-friendly** |
| Duino-Coin | SHA-1 | ✅ Parfait | Conçu pour Pi |

### Solo Mining vs Mining Pool

```
SOLO   → 100% récompense si trouvé, mais probabilité quasi 0% ❌

POOL :
[Pi 1] ─┐
[Pi 2] ──┼──▶ [POOL] ──▶ Bloc trouvé ──▶ Récompense partagée ✅
[Pi N] ─┘
```

> Recommandé : toujours utiliser un **Mining Pool** avec un Raspberry Pi.

---

## 🔧 Matériel requis

```
OBLIGATOIRE :
✅ Raspberry Pi 5 (4GB ou 8GB)
✅ Carte MicroSD 32GB min. (Classe 10 / A2)
✅ Alimentation officielle Pi 5 (5V / 5A USB-C)
✅ Câble Ethernet

FORTEMENT RECOMMANDÉ :
✅ Boîtier avec ventilateur actif (mining = chauffe !)
✅ Dissipateurs thermiques
```

### Températures à respecter

| Température | État |
|-------------|------|
| < 60°C | ✅ Parfait |
| 60-70°C | 🔶 Normal sous charge |
| 70-80°C | ⚠️ Réduire max-threads-hint |
| > 80°C | ❌ DANGER — arrêter immédiatement ! |

---

## 💿 Étape 1 — Installation OS

### OS recommandé

```
Nom   : Raspberry Pi OS Lite 64-bit (Bookworm)
Arch  : ARM64 (aarch64) ✅
SSH   : Activé (connexion sans écran)
```

**Téléchargement Raspberry Pi Imager :**
```
https://www.raspberrypi.com/software/
```

### Flash avec Raspberry Pi Imager

```
1. CHOOSE DEVICE  → Raspberry Pi 5
2. CHOOSE OS      → Raspberry Pi OS (other)
                  → Raspberry Pi OS Lite (64-bit) ✅
3. CHOOSE STORAGE → Ta carte MicroSD
4. Cliquer ⚙️ Paramètres avancés
```

### Configuration avancée ⚙️ — OBLIGATOIRE

```
✅ hostname    : raspberrypi-miner
✅ Enable SSH  : OUI  ← connexion sans écran
✅ Username    : pi
✅ Password    : [mot de passe fort]
✅ WiFi        : (si pas Ethernet)
✅ Timezone    : Africa/Casablanca
✅ Keyboard    : fr
```

```
5. SAVE → WRITE → YES
6. Insère la SD dans le Pi 5
7. Branche Ethernet + alimentation
8. Attends 2 minutes
```

---

## 🔌 Étape 2 — Connexion SSH

```bash
# Connexion depuis ton PC
ssh pi@raspberrypi-miner.local

# Si ça ne marche pas → utiliser l'IP directement
ssh pi@192.168.X.X

# Vérifier l'architecture ARM64
uname -m
# → aarch64 ✅

# Vérifications système
free -h               # RAM disponible
df -h /               # Espace disque
vcgencmd measure_temp # Température CPU
```

---

## ⚙️ Étape 3 — Configuration système

```bash
# Mise à jour complète
sudo apt update && sudo apt upgrade -y

# Configuration Raspberry Pi
sudo raspi-config
# → System Options → Expand Filesystem ✅
# → Performance → GPU Memory → 16 (pas d'écran)
# → Finish → Reboot
```

```bash
# Reconnexion après reboot
ssh pi@raspberrypi-miner.local

# Installation des dépendances
sudo apt install -y git build-essential cmake     libssl-dev libhwloc-dev libuv1-dev wget curl htop screen

# Désactiver Bluetooth (inutile)
sudo systemctl disable bluetooth

# Activer huge pages (optimise XMRig)
echo "vm.nr_hugepages=1280" | sudo tee -a /etc/sysctl.conf
sudo sysctl -p

# GPU mémoire minimale
echo "gpu_mem=16" | sudo tee -a /boot/firmware/config.txt

sudo reboot
```

---

## ⚙️ Étape 4 — Installation XMRig

**XMRig** est le logiciel de mining Monero le plus performant pour ARM64.

### Option A — Compiler depuis le source (recommandé)

```bash
git clone https://github.com/xmrig/xmrig.git
cd xmrig && mkdir build && cd build

cmake .. -DARM_TARGET=8 -DWITH_OPENCL=OFF -DWITH_CUDA=OFF -DWITH_HWLOC=ON

make -j4

./xmrig --version
```

### Option B — Binaire précompilé ARM64

```bash
wget https://github.com/xmrig/xmrig/releases/latest/download/xmrig-linux-static-aarch64.tar.gz
tar -xvf xmrig-linux-static-aarch64.tar.gz
cd xmrig-*/
./xmrig --version
```

---

## 👛 Étape 5 — Wallet Monero

```
Wallet Web MyMonero (option simple) :
1. Va sur : https://mymonero.com
2. Clique "Create Account"
3. SAUVEGARDE ta seed phrase (13 mots) sur papier !
4. Copie ton adresse XMR (commence par "4...")
```

### Sécurité — OBLIGATOIRE

```
À SAUVEGARDER :   seed phrase + adresse publique XMR
NE JAMAIS PARTAGER : seed phrase + clé privée + mot de passe
```

---

## 🚀 Étape 6 — Lancer le Mining

### Choisir un Mining Pool

| Pool | URL | Frais |
|------|-----|-------|
| **SupportXMR** | `pool.supportxmr.com:3333` | 0.6% |
| MoneroOcean | `gulf.moneroocean.stream:10032` | 0% |
| MineXMR | `pool.minexmr.com:443` | 1% |

### Créer config.json

```bash
nano ~/xmrig/config.json
```

```json
{
    "autosave": true,
    "donate-level": 1,
    "pools": [
        {
            "url": "pool.supportxmr.com:3333",
            "user": "TON_ADRESSE_XMR_ICI",
            "pass": "raspberry_pi_5",
            "keepalive": true,
            "tls": false
        }
    ],
    "cpu": {
        "enabled": true,
        "max-threads-hint": 75,
        "priority": 2,
        "huge-pages": true
    },
    "log-file": "/home/pi/xmrig.log",
    "print-time": 60
}
```

### Lancer XMRig

```bash
cd ~/xmrig/build
./xmrig --config=../config.json
```

### Sortie terminal attendue

```
* POOL #1    pool.supportxmr.com:3333
* CPU        ARM Cortex-A76, 4 cores, AES
* THREADS    3, RandomX, huge pages 100%

[12:00:10] speed  10s: 75.3 H/s  accepted: 0/0
[12:00:20] speed  10s: 78.1 H/s  accepted: 1/1 ✅
[12:00:30] speed  10s: 80.2 H/s  accepted: 2/2 ✅
```

---

## 🔄 Étape 7 — Mining au démarrage

```bash
sudo nano /etc/systemd/system/xmrig-mining.service
```

```ini
[Unit]
Description=XMRig Monero Miner
After=network-online.target
Wants=network-online.target

[Service]
ExecStart=/home/pi/xmrig/build/xmrig --config=/home/pi/xmrig/config.json
Restart=always
RestartSec=10
User=pi
Nice=10

[Install]
WantedBy=multi-user.target
```

```bash
sudo systemctl daemon-reload
sudo systemctl enable xmrig-mining
sudo systemctl start xmrig-mining
sudo systemctl status xmrig-mining
```

---

## 📜 Scripts pratiques

### Script 1 — Simulation PoW (Slide 13 du cours)

```bash
#!/bin/bash
# pow_simulation.sh — Cours Pr. EL KIRAM
i=0
echo "Raspberry Pi 5 — Simulation Proof of Work"
START=$(date +%s%N)

while true; do
    content=$(head -c 32 /dev/urandom | base64)
    echo "$content" > /tmp/bloc_test.txt
    hash=$(sha256sum /tmp/bloc_test.txt | awk '{print $1}')

    if [[ $hash == 0* ]]; then
        END=$(date +%s%N)
        ELAPSED=$(( (END - START) / 1000000 ))
        echo "BLOC TROUVE apres $i essais en ${ELAPSED}ms"
        echo "SHA256 : $hash"
        break
    fi
    (( i % 100 == 0 )) && echo "Essai $i → ${hash:0:20}..."
    ((i++))
done
```

```bash
chmod +x pow_simulation.sh && ./pow_simulation.sh
```

---

### Script 2 — Mini Blockchain complète

```bash
#!/bin/bash
# mini_blockchain.sh — Cours Pr. EL KIRAM
echo "MINI BLOCKCHAIN — Raspberry Pi 5"

creer_bloc() {
    local INDEX=$1 DONNEES=$2 HASH_PREC=$3
    local TS=$(date '+%Y-%m-%d %H:%M:%S')
    local HASH=$(echo "${HASH_PREC}${DONNEES}${TS}" | sha256sum | awk '{print $1}')
    echo ""
    echo "BLOC $INDEX"
    echo "  Donnees   : $DONNEES"
    echo "  Timestamp : $TS"
    echo "  Hash prec : ${HASH_PREC:0:20}..."
    echo "  Hash      : ${HASH:0:20}..."
    echo "$HASH"
}

H0="0000000000000000"
H1=$(creer_bloc 1 "Genesis Block" "$H0")
H2=$(creer_bloc 2 "Alice envoie 5 XMR a Bob" "$H1")
H3=$(creer_bloc 3 "Bob envoie 2 XMR a Sara" "$H2")
H4=$(creer_bloc 4 "Sara envoie 1 XMR a Alice" "$H3")

echo ""
echo "Blockchain creee avec succes — 4 blocs !"
```

```bash
chmod +x mini_blockchain.sh && ./mini_blockchain.sh
```

---

### Script 3 — Comparaison algorithmes de hachage

```bash
#!/bin/bash
# hash_comparison.sh — Cours Pr. EL KIRAM
TEXTE="Blockchain Raspberry Pi 5"
echo "COMPARAISON DES ALGORITHMES — Texte : '$TEXTE'"
echo "================================================"
echo "MD5    (128 bits) :" ; echo -n "$TEXTE" | md5sum
echo "SHA-1  (160 bits) :" ; echo -n "$TEXTE" | sha1sum
echo "SHA-256(256 bits) :" ; echo -n "$TEXTE" | sha256sum
echo "SHA-512(512 bits) :" ; echo -n "$TEXTE" | sha512sum
echo ""
echo "EFFET AVALANCHE :"
echo "Bonjour :" ; echo -n "Bonjour" | sha256sum
echo "bonjour :" ; echo -n "bonjour" | sha256sum
echo "→ 1 seul caractere different = hash totalement different !"
```

```bash
chmod +x hash_comparison.sh && ./hash_comparison.sh
```

---

### Script 4 — Vérification d'intégrité

```bash
#!/bin/bash
# integrity_check.sh — Cours Pr. EL KIRAM
echo "VERIFICATION D'INTEGRITE — MD5"
echo "================================"

echo "Document important"   > doc1.txt
echo "Fichier de config"    > doc2.txt
echo "Donnees critiques"    > doc3.txt

echo "Calcul des empreintes MD5..."
md5sum doc1.txt doc2.txt doc3.txt > checksums.md5

echo "Verification initiale (fichiers intacts) :"
md5sum -c checksums.md5

echo ""
echo "Simulation modification de doc2.txt par un attaquant..."
echo "Donnees modifiees !" >> doc2.txt

echo "Verification apres modification :"
md5sum -c checksums.md5
echo "→ doc2.txt detecte comme modifie ! Propriete d'integrite OK ✅"

rm -f doc1.txt doc2.txt doc3.txt checksums.md5
```

```bash
chmod +x integrity_check.sh && ./integrity_check.sh
```

---

## 📊 Surveillance et monitoring

### Moniteur en temps réel

```bash
#!/bin/bash
# monitor_mining.sh
while true; do
    clear
    echo "============================================"
    echo "    RASPBERRY PI 5 — MINING MONITOR"
    echo "============================================"
    echo " Temperature : $(vcgencmd measure_temp)"
    echo " RAM         : $(free -h | grep Mem | awk '{print $3" / "$2}')"
    echo " CPU         : $(top -bn1 | grep 'Cpu(s)' | awk '{print $2}')%"
    echo " Uptime      : $(uptime -p)"
    if systemctl is-active --quiet xmrig-mining; then
        echo " XMRig       : ACTIF ✅"
    else
        echo " XMRig       : ARRETE ❌"
    fi
    echo "--------------------------------------------"
    echo " Derniers logs XMRig :"
    tail -3 /home/pi/xmrig.log 2>/dev/null || echo " (aucun log)"
    echo "============================================"
    sleep 5
done
```

```bash
chmod +x monitor_mining.sh && ./monitor_mining.sh
```

### Commandes utiles au quotidien

```bash
# Gestion du service
sudo systemctl status xmrig-mining     # Statut
sudo systemctl start xmrig-mining      # Démarrer
sudo systemctl stop xmrig-mining       # Arrêter
sudo systemctl restart xmrig-mining    # Redémarrer

# Logs et monitoring
journalctl -u xmrig-mining -f          # Logs en direct
vcgencmd measure_temp                   # Température CPU
htop                                    # Ressources système
grep "accepted" ~/xmrig.log | tail -20  # Shares acceptées

# Stats en ligne
# Va sur https://supportxmr.com → colle ton adresse XMR
```

---

## 💰 Est-ce rentable ?

### Bilan mensuel estimé

| Élément | Valeur |
|---------|--------|
| Hashrate Pi 5 | ~75 H/s RandomX |
| Consommation | ~10W = 7.2 kWh/mois |
| Coût électricité (~0.13€/kWh) | ~0.94€/mois |
| Revenu XMR estimé | ~0.03€/mois |
| **Bilan mensuel** | **~-0.91€/mois ❌** |

> Ce projet n'est **pas rentable financièrement** avec un seul Pi 5.

### La vraie valeur — L'apprentissage

```
✅ Comprendre le hachage SHA-256 / RandomX en pratique
✅ Maîtriser le Proof of Work (cours Pr. EL KIRAM)
✅ Maîtriser Linux ARM64 et SSH
✅ Configurer des services systemd sur Linux
✅ Expérimenter la blockchain en conditions réelles
✅ Projet concret pour ton portfolio et CV
✅ Base solide en cybersécurité et cryptographie
```

---

## 🎓 Lien avec le cours

| Concept — Cours Pr. EL KIRAM | Application dans ce projet |
|------------------------------|---------------------------|
| Fonction de hachage | RandomX calcule des hash en permanence |
| Sens unique | Impossible de deviner le Nonce sans tout essayer |
| Non-collision | Chaque bloc a un hash unique |
| Effet avalanche | Nonce+1 → hash totalement différent |
| SHA-256 | Algorithme central du PoW Bitcoin |
| Proof of Work | XMRig cherche le bon Nonce en continu |
| Nonce | XMRig l'incrémente des millions de fois/seconde |
| Merkle Root | Résumé des transactions dans chaque bloc |
| Décentralisation | Le Pi est un nœud parmi des milliers dans le monde |
| Blockchain publique | Monero = blockchain publique accessible à tous |
| Récompense mining | XMR reçus si share acceptée par le pool |
| Mining Pool | Groupe de nœuds décentralisés qui collaborent |

---

## 🔗 Ressources

| Ressource | Lien |
|-----------|------|
| Vidéo NetworkChuck | https://youtube.com/watch?v=hHtGN_JzoP8 |
| XMRig GitHub | https://github.com/xmrig/xmrig |
| Pool SupportXMR | https://supportxmr.com |
| Wallet MyMonero | https://mymonero.com |
| Prix Monero XMR | https://coinmarketcap.com/currencies/monero |
| Raspberry Pi Imager | https://raspberrypi.com/software |
| Monero officiel | https://getmonero.org |

---

> **Projet académique — Cours Hachage & Blockchain — Pr. EL KIRAM**
> Raspberry Pi 5 — Raspberry Pi OS Lite 64-bit — ARM64
