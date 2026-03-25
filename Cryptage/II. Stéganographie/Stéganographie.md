# II. Stéganographie

La stéganographie, contrairement à la cryptographie, ne cherche pas à rendre un message illisible — elle cherche à **cacher l'existence même du message**.

---

## Exp1 — L'esclave d'Histiée (499 av. J.-C.)

Histiée, tyran de Milet, était retenu prisonnier par le roi des Perses. Pour envoyer un message secret, il eut une idée ingénieuse :

1. Il fit **raser le crâne** d'un esclave
2. Il **écrivit le message** sur son crâne chauve
3. Il attendit que les **cheveux repoussent**
4. Il envoya l'esclave — les gardes le fouillèrent mais ne trouvèrent rien

> 💡 Le message était invisible car caché sous les cheveux. Personne ne soupçonnait l'esclave d'être un porteur de message.

---

## Exp2 & 3 — Messages cachés par acrostiche

Un **acrostiche** est un texte dont on lit les **premières lettres de chaque ligne** pour former un message caché.

**Exemple Exp2 :**

```
La logique structure nos raisonnements les plus subtils.
Au-delà des apparences, chaque détail compte réellement.
Bien souvent, la vérité se dissimule dans la forme.
Les esprits attentifs remarquent des motifs invisibles.
On pourrait croire à un simple paragraphe académique.
Cependant, certaines lettres ont été choisies avec soin.
Kilomètres de textes peuvent cacher une simple idée.
Cherchez avec méthode et patience.
Habilement structuré, le message attend d'être découvert.
Analyser chaque niveau avant de conclure.
Ignorez le superflu et concentrez-vous sur l'essentiel.
Ne vous fiez jamais uniquement à la surface.
```

➡️ Premières lettres : **L A B L O C K C H A I N**

**Exemple Exp3 :**

```
Souvent les apparences sont trompeuses.
Entre les lignes se cache parfois un secret.
Certains messages ne se révèlent qu'aux plus attentifs.
Rien n'est laissé au hasard dans l'écriture.
En observant les premières lettres, on comprend mieux.
Tout devient alors plus clair.
```

➡️ Premières lettres : **S E C R E T**

---

## Exp4 — Chiffre de Trithème (Moyen-Âge)

Au Moyen-Âge, l'abbé **Jean Trithème** (1462–1516) inventa un système pour cacher un message dans un **texte religieux** :

- Il associa des **sentences religieuses** à chaque lettre de l'alphabet
- Les mots "inutiles" servent de liant entre les sentences
- Le message est extrait en relevant la **lettre correspondant à chaque sentence**

> 💡 Le texte ressemble à une prière normale — personne ne soupçonne qu'il cache un message.

---

## Exp5 — Dissimulation dans les pixels d'une image (LSB)

C'est la technique moderne la plus utilisée. Elle exploite le **bit de poids faible** (*Least Significant Bit*) de chaque pixel.

### Principe

Chaque pixel d'une image est codé en binaire (ex. rouge = 8 bits). Modifier le dernier bit change à peine la couleur — l'œil humain ne voit pas la différence.

```
Valeur originale du pixel rouge : 00010001  (= 17)
On change le dernier bit        : 00010000  (= 16)
Différence visuelle             : imperceptible ✅
```

### Comment cacher la lettre "A" (ASCII = 65 = 01000001)

```
Pixels originaux  :  ...1  ...1  ...0  ...0  ...0  ...0  ...0  ...1
Bits du message A :    0     1     0     0     0     0     0     1
Pixels modifiés   :  ...0  ...1  ...0  ...0  ...0  ...0  ...0  ...1
```

> 💡 L'image semble identique à l'œil nu, mais elle contient un message secret dans ses pixels.

---

## Récapitulatif

| Exemple | Époque | Technique |
|---------|--------|-----------|
| Esclave d'Histiée | 499 av. J.-C. | Message écrit sur le crâne rasé |
| Acrostiche (Exp2 & 3) | Classique | Premières lettres de chaque ligne |
| Chiffre de Trithème | Moyen-Âge | Sentences codant des lettres |
| LSB (Exp5) | Moderne | Bit de poids faible des pixels |

> 💡 **Différence fondamentale** : la cryptographie rend le message *illisible*, la stéganographie le rend *invisible*.
