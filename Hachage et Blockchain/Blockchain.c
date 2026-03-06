/*
STRUCT           → définir le bloc
       ↓
create_block()   → créer + allouer un bloc
       ↓
compute_hash()   → calculer le hash du bloc
       ↓
add_block()      → ajouter un bloc à la chaîne
       ↓
print_block()    → afficher un bloc
       ↓
print_chain()    → afficher toute la blockchain
       ↓
verify_chain()   → vérifier que personne n'a triché
*/

#include<stdio.h>
#include<stdlib.h>
#include<openssl/sha.h>

typedef struct Block
 {
    int indice;
    char data[256];
    char prev_hash[65];
    char hash[65];
    struct Block *next;
}Block;

Block* CreeBlock(int indice,char data[256], char prevHash[65])
{
    int i ;
    int j ;
    Block *b = malloc (sizeof(Block));
    if(b == NULL)
   { 
        printf("!!!404--Erreur!!!!");
        return NULL;
    }
    b->indice=indice;
    strncpy(b->data,      data,      256);
    strncpy(b->prev_hash, prevHash,  65);
    b->next = NULL;

    return b;
}

void ComputeHash(Block *b)
{
    char tab[1000];
    int i;
    int pos = 0;

    for (i = 0; b->data[i] != '\0'; i++)
    {
        tab[pos] = b->data[i];
        pos++;
    }
    for (i = 0; b->prev_hash[i] != '\0'; i++)
    {
        tab[pos] = b->prev_hash[i];
        pos++;
    }
    tab[pos] = b->indice;
    pos++;
    tab[pos] = '\0';

    unsigned char affichage[32];
    SHA256((unsigned char*)tab, pos, affichage);  
    //khass nraj3hom l hexadecimal SHA256 katretounr ma bin 0 l 256
    char hex[] = "0123456789abcdef";
    for (i = 0; i < 32; i++)
    {
        b->hash[i * 2]     = hex[affichage[i] / 16];
        b->hash[i * 2 + 1] = hex[affichage[i] % 16];
    }
    b->hash[64] = '\0';
    printf("Hash : %s\n", b->hash);
}
void addBlock(Block *b, char data[256])
{
    Block *ptr = b;
    while (ptr->next != NULL)
        ptr = ptr->next;

    int nouvel_indice = ptr->indice + 1;
    Block *nouveau = CreeBlock(nouvel_indice, data, ptr->hash);
    ComputeHash(nouveau);
    ptr->next = nouveau;
}
void print_block(Block *b)
{
    printf("indice    : %d\n",  b->indice);
    printf("data      : %s\n",  b->data);
    printf("prev_hash : %s\n",  b->prev_hash);
    printf("hash      : %s\n",  b->hash);
    
}
void print_chain(Block *head)
{
    Block *current = head;
    while (current != NULL)
    {
        print_block(current);
        current = current->next;
    }
}
int verify_chain(Block *head)
{
    Block *prev    = head;
    Block *current = head->next;

    while (current != NULL)
    {
        int i;
        int ok = 1;
        for (i = 0; i < 64; i++)
        {
            if (current->prev_hash[i] != prev->hash[i])
            {
                ok = 0;
                break;
            }
        }

        if (ok == 0)
        {
            printf("Bloc %d corrompu !\n", current->indice);
            return 0;
        }

        prev    = current;
        current = current->next;
    }
    printf(" Blockchain valide !\n");
    return 1;
}
int main()
{
    
    Block *jadid = CreeBlock(0, "Genesis Block","0000000000000000000000000000000000000000000000000000000000000000");//64 dyal 0
    ComputeHash(jadid);

    print_chain(jadid);

    return 0;
}
