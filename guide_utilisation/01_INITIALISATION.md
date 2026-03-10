# 01 - Initialisation de TAGCAN

##  Objectif

Configurer le système TAGCAN et le bus CAN pour pouvoir communiquer avec d'autres nœuds CAN.

##  Vue d'ensemble du processus

L'initialisation suit ces étapes :

```
1. init_com() (point d'appel unique)
   ├─ CAN_init_TAGCAN()
   │  ├─ init_data_container()     -> Efface le buffer TxData
   │  └─ init_tag_manager()        -> Initialise la gestion des tags
   │
   ├─ CAN_setup_TAG()              -> Crée les tags applicatifs
   │  (set_tag pour chaque variable)
   │
   ├─ CAN_setup_DF()               -> Configure le header CAN
   │  (ID, type de frame, DLC)
   │
   └─ HAL_CAN_ActivateNotification -> Active les interruptions RX
```

##  Détail de chaque étape

### Étape 1 : init_data_container()

Réinitialise le buffer de données CAN à zéro.

```c
void init_data_container(void);
```

| Paramètre | Type | Description |
|-----------|------|-------------|
| - | - | (aucun) |

**Effet** :
- Vide complètement le buffer `TxData[8]`
- Doit être appelé (au moins une fois) lors du démarrage
- Atention: un appel en cours d'exécution réinitialiseras les valeurs à 0.
---

### Étape 2 : init_tag_manager()

Initialise le gestionnaire de tags et efface tous les tags existants.

```c
void init_tag_manager(void);
```

**Note** : Après cet appel, tous les tags doivent être redéfinis avec `set_tag()`.

---

### Étape 3 : CAN_setup_TAG()

Crée les tags applicatifs en fonction de vos besoins.

```c
void CAN_setup_TAG(void) {
    set_tag("SEQ", 3);        // Compteur de séquences (3 bits)
    set_tag("HB", 1);         // Watchdog HeartBeat (1 bit)
    set_tag("CI", 2);         // Continuité igniter (2 bits)
    set_tag("AB", 7);         // Alim. Batterie en % (7 bits)
    set_tag("EDC", 4);        // États des commandes (4 bits)
    set_tag("N2OPS", 16);     // Capteur pression N2O (16 bits)
    set_tag("TLC", 12);       // Thrust Load Cell (12 bits)
    set_tag("N2ODVS", 1);     // État soupape Dump N2O (1 bit)
    set_tag("N2OIV", 1);      // Valve igniter N2O (1 bit)
    set_tag("N2OMV", 1);      // Soupape principale N2O (1 bit)
    set_tag("TIME", 16);      // Timestamp en ms (16 bits)
}
```

**Paramètres de set_tag()** :

```c
int set_tag(const char* tag_name, uint8_t n_bit);
```

| Paramètre | Type | Description |
|-----------|------|-------------|
| `tag_name` | `const char*` | Nom unique du tag (ex. "SEQ") |
| `n_bit` | `uint8_t` | Nombre de bits (1 à 16) |

**Valeur de retour** :
- Code de succès ou code d'erreur
- À consulter pour débugger (voir `documentation/API_REFERENCE.md`)

---

### Étape 4 : CAN_setup_DF()

Configure le header de transmission CAN (ID, type de frame, etc.).

```c
void CAN_setup_DF(int32_t std_id) {
    std_id &= 0x7FFU;                // Masque pour ID standard 11 bits
    TxHeader.IDE = CAN_ID_STD;       // Utiliser ID standard (non étendu)
    TxHeader.StdId = std_id;         // Assigner l'ID CAN
    TxHeader.RTR = CAN_RTR_DATA;     // Mode données (non RTR)
    TxHeader.DLC = 8;                // Longueur = 8 octets
}
```

**Points importants** :
- `IDE = CAN_ID_STD` -> utilise les IDs standards (11 bits)
- `RTR = CAN_RTR_DATA` -> frame de données (pas une requête)
- `DLC = 8` -> longueur fixe du payload (8 octets)
- `StdId` -> l'identifiant unique du message CAN

---

### Étape 5 : Activer les interruptions

```c
if(HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK) {
    Error_Handler();
}
```

**Explications** :
- Permet de recevoir les messages CAN par interruption (plutôt que polling)
- `CAN_IT_RX_FIFO0_MSG_PENDING` -> déclenche une IRQ à la réception d'un message dans FIFO0
- Nécessite l'implémentation du callback : `void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)`

---

##  Code complet d'initialisation

```c
#include "com_utils.h"
#include "TAGCAN.h"

void init_com(void) {
    // Étape 1 : Initialiser les conteneurs de données
    CAN_init_TAGCAN();

    // Étape 2 : Créer les tags applicatifs
    CAN_setup_TAG();

    // Étape 3 : Configurer le header CAN
    CAN_setup_DF(0x123);  // ID CAN = 0x123

    // Étape 4 : Activer la réception par interruption
    if(HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK) {
        Error_Handler();
    }

    //  Système TAGCAN prêt !
}

// À appeler une seule fois dans main() au démarrage
int main(void) {
    HAL_Init();
    MX_CAN_Init();      // Initialisation du module CAN matériel

    init_com();         // Initialisation TAGCAN

    // Boucle principale...
    while(1) {
        // Votre code
    }
}
```

---

##  Points d'attention

|  Point | Détail |
|---------|--------|
| **Ordre d'appel** | Toujours : `init_data_container()` -> `init_tag_manager()` -> `set_tag()` |
| **Appel unique** | `init_com()` doit être appelée **une seule fois** au démarrage |
| **Réinitialisation** | Si vous appelez `init_tag_manager()` à nouveau, tous les tags doivent être recréés |
| **Configuration matérielle** | Le module CAN STM32 (HAL) doit être initialisé **avant** TAGCAN |
| **ID CAN unique** | Chaque nœud du réseau doit avoir un ID CAN unique |

---

##  Vérification

Après initialisation :
-  `TxData[8]` est rempli de zéros
-  Les tags sont créés et prêts à l'emploi
-  Le header `TxHeader` est configuré avec votre ID CAN
-  Les interruptions CAN sont actives
-  Prêt pour `set_value()` et `get_value()`

---

Voir aussi :
- [02_GESTION_TAGS.md](02_GESTION_TAGS.md) - Utiliser les tags
- [03_DONNEES_CAN.md](03_DONNEES_CAN.md) - Gérer les données CAN
