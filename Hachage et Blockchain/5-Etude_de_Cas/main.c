/*
 * ============================================================
 *  BLOCKCHAIN TRACABILITE AGROALIMENTAIRE
 *  Genere blockchain.html avec QR code integre
 *
 *  Compilation : cc  main.c
 *  Lancer      :  ./a.out && firefox blockchain.html
 * ============================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ============================================================
 *  HASH  djb2
 * ============================================================ */
static void computeHash(const char *input, char out[32])
{
    unsigned long h = 5381;
    for (int i = 0; input[i]; i++)
        h = ((h << 5) + h) + (unsigned char)input[i];
    sprintf(out, "%lu", h);
}

/* ============================================================
 *  STRUCTURE  liste chainee
 * ============================================================ */
typedef struct Block {
    int          index;
    char         productID[20];
    char         actor[50];
    char         action[150];
    int          quantity;
    char         prevHash[32];
    char         hash[32];
    struct Block *next;
} Block;

typedef struct {
    Block *head;
    Block *tail;
} Blockchain;

/* ============================================================
 *  CREER / INIT / AJOUTER
 * ============================================================ */
Block* createBlock(int index, const char *productID,
                   const char *actor, const char *action,
                   int quantity, const char *prevHash)
{
    Block *b = (Block*)malloc(sizeof(Block));
    b->index = index; b->quantity = quantity; b->next = NULL;
    strncpy(b->productID, productID, 19); b->productID[19]='\0';
    strncpy(b->actor,     actor,     49); b->actor[49]='\0';
    strncpy(b->action,    action,   149); b->action[149]='\0';
    strncpy(b->prevHash,  prevHash,  31); b->prevHash[31]='\0';
    char buf[512];
    sprintf(buf,"%d%s%s%s%d%s",b->index,b->productID,b->actor,
            b->action,b->quantity,b->prevHash);
    computeHash(buf, b->hash);
    return b;
}

Blockchain* newBlockchain()
{
    Blockchain *bc = (Blockchain*)malloc(sizeof(Blockchain));
    bc->head = createBlock(0,"P001","System","Genesis Block",0,"00000000");
    bc->tail = bc->head;
    return bc;
}

void addBlock(Blockchain *bc, const char *product,
              const char *actor, const char *action, int qty)
{
    Block *n = createBlock(bc->tail->index+1,product,actor,action,qty,bc->tail->hash);
    bc->tail->next = n;
    bc->tail = n;
}

/* ============================================================
 *  VERIFIER
 * ============================================================ */
int verifyChain(Blockchain *bc)
{
    Block *prev=NULL, *cur=bc->head;
    while(cur){
        char buf[512], exp[32];
        sprintf(buf,"%d%s%s%s%d%s",cur->index,cur->productID,cur->actor,
                cur->action,cur->quantity,cur->prevHash);
        computeHash(buf,exp);
        if(strcmp(exp,cur->hash)!=0) return 0;
        if(prev && strcmp(cur->prevHash,prev->hash)!=0) return 0;
        prev=cur; cur=cur->next;
    }
    return 1;
}

/* ============================================================
 *  AFFICHAGE TERMINAL
 * ============================================================ */
void printChain(Blockchain *bc)
{
    Block *cur=bc->head;
    while(cur){
        printf("---------------------\n");
        printf("Index    : %d\n", cur->index);
        printf("Actor    : %s\n", cur->actor);
        printf("Action   : %s\n", cur->action);
        if(cur->quantity>0) printf("Quantity : %d kg\n", cur->quantity);
        printf("Hash     : %s\n", cur->hash);
        printf("PrevHash : %s\n", cur->prevHash);
        cur=cur->next;
    }
}

/* ============================================================
 *  STYLE PAR ACTEUR
 * ============================================================ */
typedef struct { const char *accent,*light,*icon,*label; } Style;
Style getStyle(int i){
    Style s[]={
        {"#6366f1","#eef2ff","&#9733;",   "GENESIS"},
        {"#16a34a","#f0fdf4","&#127806;", "BLOC 1"},
        {"#d97706","#fffbeb","&#128300;", "BLOC 2"},
        {"#ea580c","#fff7ed","&#128659;", "BLOC 3"},
        {"#7c3aed","#f5f3ff","&#128115;", "BLOC 4"},
        {"#0891b2","#ecfeff","&#127978;", "BLOC 5"},
    };
    if(i<0||i>5) i=0;
    return s[i];
}

/* ============================================================
 *  ECRIRE FLECHE / BLOC
 * ============================================================ */
void writeArrow(FILE *f){
    fprintf(f,
        "  <div class=\"arrow\">"
        "<div class=\"arrow-line\"></div>"
        "<div class=\"arrow-tip\"></div>"
        "<span class=\"arrow-label\">next</span>"
        "</div>\n\n");
}

void writeBlock(FILE *f, Block *b, int ok){
    Style s=getStyle(b->index);
    fprintf(f,
"  <div class=\"block\" style=\"--accent:%s;--accent-light:%s;\">\n"
"    <div class=\"block-top\"></div>\n"
"    <div class=\"block-body\">\n"
"      <div class=\"block-head\">\n"
"        <div class=\"block-left\">\n"
"          <div class=\"block-icon\">%s</div>\n"
"          <div><div class=\"block-actor\">%s</div>"
"<div class=\"block-label\">%s</div></div>\n"
"        </div>\n"
"        <span class=\"badge %s\">%s</span>\n"
"      </div>\n"
"      <hr class=\"sep\">\n"
"      <div class=\"info-grid\">\n"
"        <div class=\"info-item\"><label>Produit</label><span>%s</span></div>\n"
"        <div class=\"info-item\"><label>Quantit&eacute;</label><span>%s</span></div>\n"
"        <div class=\"info-item full\"><label>Action</label><span>%s</span></div>\n"
"      </div>\n"
"      <div class=\"hash-row\">\n"
"        <span class=\"hash-label\">Hash</span>"
"<span class=\"hash-val\">%s</span>\n"
"        <span class=\"hash-label\">Prev</span>"
"<span class=\"hash-prev\">%s</span>\n"
"      </div>\n"
"    </div>\n"
"  </div>\n\n",
        s.accent, s.light, s.icon,
        b->actor, s.label,
        ok?"ok":"err", ok?"&#10003; OK":"&#10007; ERR",
        b->productID,
        b->quantity>0?"100 kg":"&mdash;",
        b->action,
        b->hash, b->prevHash
    );
}

/* ============================================================
 *  CONSTRUIRE L'URL DU QR CODE  (encode les infos du produit)
 *  On utilise qrserver.com — API gratuite, aucune install
 *  Le QR code encode un JSON lisible par n'importe quel scanner
 * ============================================================ */
void buildQRUrl(Blockchain *bc, char *url, int urlSize)
{
    /* Contenu encode dans le QR :
     * Produit P001 | Chaque etape | Hash final
     * Format texte simple lisible par le telephone */
    char content[1200];
    int  pos = 0;

    pos += snprintf(content+pos, sizeof(content)-pos,
        "TRACABILITE PRODUIT P001%%0A"
        "Maroc - Europe%%0A"
        "---%%0A");

    Block *cur = bc->head;
    while(cur && pos < 1000){
        if(cur->index > 0){
            pos += snprintf(content+pos, sizeof(content)-pos,
                "%d. %s : %s%%0A",
                cur->index, cur->actor, cur->action);
        }
        cur = cur->next;
    }
    pos += snprintf(content+pos, sizeof(content)-pos,
        "---%%0A"
        "Hash final : %s%%0A"
        "BLOCKCHAIN VALIDE",
        bc->tail->hash);

    snprintf(url, urlSize,
        "https://api.qrserver.com/v1/create-qr-code/"
        "?size=220x220&margin=10&color=0c2461&bgcolor=ffffff"
        "&data=%s",
        content);
}

/* ============================================================
 *  GENERER LE HTML
 * ============================================================ */
void generateHTML(Blockchain *bc, const char *filename)
{
    FILE *f = fopen(filename, "w");
    if(!f){ printf("Erreur: impossible de creer %s\n", filename); return; }

    int   valid = verifyChain(bc);
    char  qrUrl[2000];
    buildQRUrl(bc, qrUrl, sizeof(qrUrl));

    /* ── HEAD + CSS ── */
    fprintf(f,
"<!DOCTYPE html>\n"
"<html lang=\"fr\">\n"
"<head>\n"
"<meta charset=\"UTF-8\">\n"
"<title>Blockchain - Tracabilite Agroalimentaire</title>\n"
"<link href=\"https://fonts.googleapis.com/css2?family=IBM+Plex+Mono:wght@400;600&family=IBM+Plex+Sans:wght@300;400;600&display=swap\" rel=\"stylesheet\">\n"
"<style>\n"
"  *{box-sizing:border-box;margin:0;padding:0;}\n"
"  body{background:#87ceeb;font-family:'IBM Plex Sans',sans-serif;color:#1a1d23;min-height:100vh;padding:48px 20px 80px;}\n"
"  header{text-align:center;margin-bottom:48px;}\n"
"  header h1{font-size:2.2rem;font-weight:700;letter-spacing:1px;color:#0c2461;}\n"
"  header p{margin-top:8px;font-size:1.15rem;color:#1a3a6b;font-weight:500;}\n"
"  .line{width:48px;height:3px;background:#2563eb;border-radius:2px;margin:14px auto 0;}\n"
"  .status{display:flex;align-items:center;justify-content:center;gap:10px;max-width:640px;\n"
"    margin:0 auto 44px;padding:16px 28px;border-radius:10px;font-size:1.1rem;font-weight:700;}\n"
"  .status.ok {background:#f0fdf4;border:1.5px solid #16a34a;color:#16a34a;}\n"
"  .status.err{background:#fef2f2;border:1.5px solid #dc2626;color:#dc2626;}\n"
"  .chain{display:flex;flex-direction:column;align-items:center;max-width:640px;margin:0 auto;}\n"
"  .block{width:100%%;background:#fff;border:1.5px solid #e5e7eb;border-radius:12px;\n"
"    overflow:hidden;box-shadow:0 1px 4px rgba(0,0,0,.06);transition:box-shadow .2s,transform .2s;}\n"
"  .block:hover{box-shadow:0 4px 16px rgba(0,0,0,.10);transform:translateY(-2px);}\n"
"  .block-top{height:5px;background:var(--accent,#2563eb);}\n"
"  .block-body{padding:20px 24px;}\n"
"  .block-head{display:flex;align-items:center;justify-content:space-between;margin-bottom:16px;}\n"
"  .block-left{display:flex;align-items:center;gap:10px;}\n"
"  .block-icon{width:36px;height:36px;border-radius:8px;background:var(--accent-light,#eff6ff);\n"
"    display:flex;align-items:center;justify-content:center;font-size:1.1rem;}\n"
"  .block-actor{font-size:1.25rem;font-weight:700;color:#1a1d23;}\n"
"  .block-label{font-size:.85rem;color:#6b7280;font-family:'IBM Plex Mono',monospace;margin-top:2px;}\n"
"  .badge{font-size:.85rem;font-weight:700;padding:5px 14px;border-radius:20px;font-family:'IBM Plex Mono',monospace;}\n"
"  .badge.ok {background:#f0fdf4;color:#16a34a;border:1px solid #bbf7d0;}\n"
"  .badge.err{background:#fef2f2;color:#dc2626;border:1px solid #fecaca;}\n"
"  .sep{border:none;border-top:1px solid #f3f4f6;margin:0 0 16px;}\n"
"  .info-grid{display:grid;grid-template-columns:1fr 1fr;gap:12px 24px;margin-bottom:16px;}\n"
"  .info-grid .full{grid-column:1/-1;}\n"
"  .info-item label{display:block;font-size:.82rem;text-transform:uppercase;\n"
"    letter-spacing:.8px;color:#6b7280;margin-bottom:4px;}\n"
"  .info-item span{font-size:1.05rem;color:#1a1d23;font-weight:600;}\n"
"  .hash-row{display:flex;align-items:center;gap:8px;flex-wrap:wrap;\n"
"    padding-top:12px;border-top:1px solid #f3f4f6;}\n"
"  .hash-label{font-size:.8rem;text-transform:uppercase;letter-spacing:.8px;color:#6b7280;font-weight:600;}\n"
"  .hash-val {font-family:'IBM Plex Mono',monospace;font-size:.92rem;color:#059669;\n"
"    background:#f0fdf4;padding:4px 10px;border-radius:5px;border:1px solid #d1fae5;}\n"
"  .hash-prev{font-family:'IBM Plex Mono',monospace;font-size:.92rem;color:#374151;\n"
"    background:#f9fafb;padding:4px 10px;border-radius:5px;border:1px solid #e5e7eb;}\n"
"  .arrow{display:flex;flex-direction:column;align-items:center;padding:4px 0;}\n"
"  .arrow-line{width:4px;height:30px;background:#1a56db;border-radius:2px;}\n"
"  .arrow-tip{width:0;height:0;border-left:12px solid transparent;\n"
"    border-right:12px solid transparent;border-top:18px solid #1a56db;}\n"
"  .arrow-label{font-family:'IBM Plex Mono',monospace;font-size:.85rem;\n"
"    color:#0c2461;font-weight:700;margin-top:4px;}\n"
"  .null-end{margin-top:12px;font-family:'IBM Plex Mono',monospace;\n"
"    font-size:1rem;color:#dc2626;font-weight:700;}\n"

"  /* ── QR CODE SECTION ── */\n"
"  .qr-section{\n"
"    max-width:640px;margin:48px auto 0;\n"
"    background:#fff;border-radius:16px;\n"
"    border:1.5px solid #e5e7eb;\n"
"    box-shadow:0 4px 20px rgba(0,0,0,.08);\n"
"    overflow:hidden;\n"
"  }\n"
"  .qr-header{\n"
"    background:#0c2461;\n"
"    padding:18px 28px;\n"
"    display:flex;align-items:center;gap:14px;\n"
"  }\n"
"  .qr-header-icon{font-size:1.8rem;}\n"
"  .qr-header-text h2{font-size:1.2rem;font-weight:700;color:#fff;}\n"
"  .qr-header-text p{font-size:.9rem;color:#93c5fd;margin-top:2px;}\n"
"  .qr-body{\n"
"    padding:32px 28px;\n"
"    display:flex;\n"
"    align-items:center;\n"
"    gap:36px;\n"
"  }\n"
"  .qr-img-wrap{\n"
"    flex-shrink:0;\n"
"    background:#f8faff;\n"
"    border:2px solid #dbeafe;\n"
"    border-radius:12px;\n"
"    padding:12px;\n"
"    display:flex;align-items:center;justify-content:center;\n"
"  }\n"
"  .qr-img-wrap img{\n"
"    width:200px;height:200px;\n"
"    display:block;\n"
"    border-radius:4px;\n"
"  }\n"
"  .qr-info{flex:1;}\n"
"  .qr-info h3{font-size:1.05rem;font-weight:700;color:#0c2461;margin-bottom:14px;}\n"
"  .qr-steps{display:flex;flex-direction:column;gap:10px;}\n"
"  .qr-step{\n"
"    display:flex;align-items:flex-start;gap:10px;\n"
"    padding:10px 14px;\n"
"    background:#f0f7ff;\n"
"    border-radius:8px;\n"
"    border-left:3px solid #2563eb;\n"
"  }\n"
"  .qr-step .step-num{\n"
"    width:22px;height:22px;border-radius:50%;\n"
"    background:#2563eb;color:#fff;\n"
"    font-size:.75rem;font-weight:700;\n"
"    display:flex;align-items:center;justify-content:center;\n"
"    flex-shrink:0;margin-top:1px;\n"
"  }\n"
"  .qr-step .step-txt{\n"
"    font-size:.92rem;color:#1e3a5f;font-weight:500;line-height:1.4;\n"
"  }\n"
"  .qr-footer{\n"
"    padding:14px 28px;\n"
"    background:#f0f7ff;\n"
"    border-top:1px solid #dbeafe;\n"
"    display:flex;align-items:center;gap:8px;\n"
"  }\n"
"  .qr-footer span{font-size:.82rem;color:#1e3a5f;font-weight:500;}\n"
"  .qr-badge{\n"
"    background:#0c2461;color:#fff;\n"
"    font-size:.75rem;font-weight:700;\n"
"    padding:3px 10px;border-radius:20px;\n"
"    font-family:'IBM Plex Mono',monospace;\n"
"    margin-left:auto;\n"
"  }\n"

"  footer{text-align:center;margin-top:48px;font-size:.95rem;color:#0c2461;line-height:2;font-weight:500;}\n"
"  footer code{font-family:'IBM Plex Mono',monospace;background:rgba(255,255,255,.7);\n"
"    padding:2px 8px;border-radius:4px;font-size:.88rem;color:#0c2461;}\n"
"</style>\n"
"</head>\n"
"<body>\n\n"
    );

    /* ── HEADER ── */
    fprintf(f,
"<header>\n"
"  <h1>Blockchain Tra&#231;abilit&#233; Agroalimentaire</h1>\n"
"  <p>Maroc &rarr; Europe &nbsp;&middot;&nbsp; Produit P001 &nbsp;&middot;&nbsp; Oranges</p>\n"
"  <div class=\"line\"></div>\n"
"</header>\n\n"
    );

    /* ── STATUS ── */
    if(valid)
        fprintf(f,"<div class=\"status ok\">&#10003; &nbsp; Blockchain VALIDE &#8212; Tous les blocs sont int&egrave;gres</div>\n\n");
    else
        fprintf(f,"<div class=\"status err\">&#10007; &nbsp; Blockchain INVALIDE &#8212; Falsification d&eacute;tect&eacute;e !</div>\n\n");

    /* ── BLOCS ── */
    fprintf(f,"<div class=\"chain\">\n\n");
    Block *cur=bc->head;
    while(cur){
        char buf[512],exp[32];
        sprintf(buf,"%d%s%s%s%d%s",cur->index,cur->productID,cur->actor,
                cur->action,cur->quantity,cur->prevHash);
        computeHash(buf,exp);
        writeBlock(f,cur,strcmp(exp,cur->hash)==0);
        if(cur->next) writeArrow(f);
        cur=cur->next;
    }
    fprintf(f,
"  <div class=\"null-end\">next = NULL &#8212; fin de la liste chain&eacute;e</div>\n\n"
"</div>\n\n"
    );

    /* ── QR CODE SECTION ── */
    fprintf(f,
"<!-- QR CODE -->\n"
"<div class=\"qr-section\">\n"
"\n"
"  <div class=\"qr-header\">\n"
"    <div class=\"qr-header-icon\">&#128247;</div>\n"
"    <div class=\"qr-header-text\">\n"
"      <h2>QR Code Traçabilité — Produit P001</h2>\n"
"      <p>Scannez pour consulter l'historique complet du produit</p>\n"
"    </div>\n"
"  </div>\n"
"\n"
"  <div class=\"qr-body\">\n"
"\n"
"    <!-- QR code genere via API qrserver.com -->\n"
"    <div class=\"qr-img-wrap\">\n"
"      <img src=\"%s\"\n"
"           alt=\"QR Code Tracabilite P001\"\n"
"           title=\"Scanner pour voir l historique blockchain du produit P001\">\n"
"    </div>\n"
"\n"
"    <div class=\"qr-info\">\n"
"      <h3>Comment ça fonctionne ?</h3>\n"
"      <div class=\"qr-steps\">\n"
"        <div class=\"qr-step\">\n"
"          <div class=\"step-num\">1</div>\n"
"          <div class=\"step-txt\">Le supermarché appose ce QR code sur le produit</div>\n"
"        </div>\n"
"        <div class=\"qr-step\">\n"
"          <div class=\"step-num\">2</div>\n"
"          <div class=\"step-txt\">Le client scanne avec son téléphone</div>\n"
"        </div>\n"
"        <div class=\"qr-step\">\n"
"          <div class=\"step-num\">3</div>\n"
"          <div class=\"step-txt\">Il voit tout le parcours : Agriculteur → Labo → Transport → Douanes</div>\n"
"        </div>\n"
"        <div class=\"qr-step\">\n"
"          <div class=\"step-num\">4</div>\n"
"          <div class=\"step-txt\">Hash final vérifie l'intégrité — données infalsifiables</div>\n"
"        </div>\n"
"      </div>\n"
"    </div>\n"
"\n"
"  </div>\n"
"\n"
"  <div class=\"qr-footer\">\n"
"    <span>&#128274; &nbsp; Données sécurisées par la Blockchain &nbsp;&middot;&nbsp; Hash : <strong>%s</strong></span>\n"
"    <span class=\"qr-badge\">BLOCKCHAIN VALIDE</span>\n"
"  </div>\n"
"\n"
"</div>\n\n",
        qrUrl,
        bc->tail->hash
    );

    /* ── FOOTER ── */
    fprintf(f,
"<footer>\n"
"  <p>G&eacute;n&eacute;r&eacute; par le programme C</p>\n"
"  <p><code>struct Block &#123; ... struct Block *next; &#125;</code></p>\n"
"  <p>Hash : algorithme djb2 &nbsp;&middot;&nbsp; Linked List en C &nbsp;&middot;&nbsp; QR Code : api.qrserver.com</p>\n"
"</footer>\n\n"
"</body>\n"
"</html>\n"
    );

    fclose(f);
    printf(">>> Fichier genere : %s\n", filename);
}

/* ============================================================
 *  LIBERER
 * ============================================================ */
void freeBlockchain(Blockchain *bc)
{
    Block *cur=bc->head;
    while(cur){Block *n=cur->next;free(cur);cur=n;}
    free(bc);
}

/* ============================================================
 *  MAIN
 * ============================================================ */
int main(void)
{
    printf("=== Construction de la blockchain ===\n\n");

    Blockchain *foodChain = newBlockchain();
    addBlock(foodChain,"P001","Farmer",      "Harvest - Parcelle A3 - Agadir",        100);
    addBlock(foodChain,"P001","Laboratory",  "Phytosanitary Test OK - Certifie UE",   100);
    addBlock(foodChain,"P001","Transporter", "Transport Agadir-Casablanca - Temp 4C", 100);
    addBlock(foodChain,"P001","Customs",     "Documents valides - Export autorise",   100);
    addBlock(foodChain,"P001","Supermarket", "Reception Paris - QR code genere",      100);

    printChain(foodChain);
    printf("\nVerification : %s\n\n",
           verifyChain(foodChain)?"VALIDE":"INVALIDE");

    generateHTML(foodChain,"blockchain.html");
    printf(">>> Ouvre blockchain.html dans Firefox !\n");
    printf(">>> Le QR code necessite une connexion internet\n");

    freeBlockchain(foodChain);
    return 0;
}
