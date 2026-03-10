# 02 - Gestion des Tags

##  Qu'est-ce qu'un tag ?

Un **tag** est une variable nommée qui occupe une plage de bits dans le buffer CAN (TxData). C'est l'interface principale pour lire et écrire des données.

**Analogie** : C'est comme des variables struct, mais mappées directement dans un buffer binaire.


---

##  Créer un tag : set_tag()

### Prototype

```c
int set_tag(const char* tag_name, uint8_t n_bit);
```

### Paramètres

| Paramètre | Type | Description |
|-----------|------|-------------|
| `tag_name` | `const char*` | Nom unique du tag (ex. "SEQ", "HB") |
| `n_bit` | `uint8_t` | Nombre de bits occupés (1 à 16) |

### Valeur de retour

Consulter `documentation/API_REFERENCE.md` pour les codes d'erreur.

### Exemples

```c
// Initialisation des tags (dans CAN_setup_TAG())
void CAN_setup_TAG(void) {
    set_tag("SEQ", 3);        // 3 bits pour numéro séquence
    set_tag("HB", 1);         // 1 bit pour watchdog
    set_tag("CI", 2);         // 2 bits pour continuité
    set_tag("AB", 7);         // 7 bits pour batterie (0-100%)
    set_tag("N2OPS", 16);     // 16 bits pour pression (0-65535 Pa)
    set_tag("TIME", 16);      // 16 bits pour timestamp (0-65535 ms)
}
```

---

##  Écrire une valeur : set_value()

### Prototype

```c
int set_value(const char* tag_name, uint32_t value);
```

### Paramètres

| Paramètre | Type | Description |
|-----------|------|-------------|
| `tag_name` | `const char*` | Nom du tag à modifier |
| `value` | `uint32_t` | Nouvelle valeur à écrire |

### Valeur de retour

- **CAN_TG_SUCCESS** (0) - Succès
- **CAN_TG_ERROR_VALUE_TO_BIG** - La valeur dépasse les `n_bit` bits du tag
- Autres codes d'erreur - Tag non trouvé, etc.

### Comportement important

 **La valeur est tronquée si elle dépasse la capacité du tag.**

Par exemple :
```c
set_tag("SEQ", 3);      // 3 bits -> plage 0 à 7
set_value("SEQ", 10);   //  Erreur : 10 > 2^3 - 1 = 7
set_value("SEQ", 5);    //  Ok : écrit 0b101
```

### Exemples d'utilisation

```c
// Dans la boucle principale ou handler
static int seq_counter = 0;

void update_telemetry(void) {
    // Incrémenter et envoyer le numéro de séquence
    seq_counter = (seq_counter + 1) % 8;  // Cycle avec 3 bits (0-7)
    if(set_value("SEQ", seq_counter) != CAN_TG_SUCCESS) {
        // Erreur...
    }

    // Envoyer le statut batterie
    uint8_t battery_percent = read_battery();  // 0-100
    if(set_value("AB", battery_percent) != CAN_TG_SUCCESS) {
        // Erreur...
    }

    // Envoyer une pression
    uint16_t pressure = read_sensor();  // 0-65535
    if(set_value("N2OPS", pressure) != CAN_TG_SUCCESS) {
        // Erreur...
    }

    // Envoyer le timestamp
    uint16_t time_ms = get_timestamp();
    set_value("TIME", time_ms);
}
```

---

##  Lire une valeur : get_value()

### Prototype

```c
int get_value(const char* tag_name, uint32_t* out_value);
```

### Paramètres

| Paramètre | Type | Description |
|-----------|------|-------------|
| `tag_name` | `const char*` | Nom du tag à lire |
| `out_value` | `uint32_t*` | Pointeur vers variable de sortie |

### Valeur de retour

- **CAN_TG_SUCCESS** (0) - Succès, `*out_value` est valide
- Négatif - Code d'erreur

### Exemples

```c
uint32_t val_seq;
if(get_value("SEQ", &val_seq) == CAN_TG_SUCCESS) {
    printf("Séquence: %u\n", val_seq);
}

uint32_t val_battery;
if(get_value("AB", &val_battery) == CAN_TG_SUCCESS) {
    printf("Batterie: %u %%\n", val_battery);
}

uint32_t val_pressure;
if(get_value("N2OPS", &val_pressure) == CAN_TG_SUCCESS) {
    printf("Pression N2O: %u Pa\n", val_pressure);
}
```

---

##  Workflow complet : set / get / send

### Scénario d'exemple

```c
#include "TAGCAN.h"
#include "stm32f1xx_hal.h"

CAN_HandleTypeDef hcan;
uint8_t TxData[8];
CAN_TxHeaderTypeDef TxHeader;
int32_t TxMailbox;

// Étape 1: Initialisation (une fois au démarrage)
void init_system(void) {
    init_com();  // Appelle init_data_container(), init_tag_manager(), set_tag(), etc.
}

// Étape 2: Boucle de mise à jour périodique
void periodic_10ms_task(void) {
    static uint8_t seq = 0;
    
    // Préparer les données
    seq = (seq + 1) % 8;
    set_value("SEQ", seq);
    set_value("HB", 1);  // Heartbeat actif
    set_value("AB", read_adc_battery());
    set_value("EDC", get_command_state());
    set_value("TIME", HAL_GetTick());

    // Envoyer le frame CAN
    if(HAL_CAN_AddTxMessage(&hcan, &TxHeader, TxData, &TxMailbox) != HAL_OK) {
        // Erreur d'envoi...
    }
}

// Étape 3: Callback de réception (déclenchée par interruption)
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
    CAN_RxHeaderTypeDef RxHeader;
    uint8_t RxData[8];

    // Recevoir le mensaje
    if(HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData) == HAL_OK) {
        // Copier dans le buffer de TAGCAN
        can_simulate_send_receive(RxData, NULL);  // ou autre traitement

        // Lire les valeurs reçues
        uint32_t remote_seq, remote_battery;
        get_value("SEQ", &remote_seq);
        get_value("AB", &remote_battery);
        
        printf("Nœud distant: SEQ=%u, Batterie=%u%%\n", remote_seq, remote_battery);
    }
}

int main(void) {
    HAL_Init();
    MX_CAN_Init();
    init_system();

    while(1) {
        periodic_10ms_task();
        // ...
    }
}
```

---

##  Bonnes pratiques

|  À faire |  À éviter |
|-----------|-----------|
| Vérifier la valeur de retour de `set_value()` | Ignorer les codes d'erreur |
| Vérifier la valeur de retour de `get_value()` | Utiliser `out_value` sans vérification |
| Nommer les tags de façon explicite | Utiliser des noms génériques ("VAL1", "VAL2") |
| Dimensionner les tags selon les besoins | Allouer trop de bits (gaspillage) |
| Prévoir le cycle des valeurs (ex: seq 0-7 avec 3 bits) | Dépasser la limite d'un tag sans gérer |
| Documenter votre schéma de tags | Laisser du code sans commentaires |

---

##  Schéma de tags recommandé

Pour un système similaire à l'exemple, structurez les tags comme ceci en commentaire :

```c
/*
 * SCHÉMA DE TAGS CAN (ID 0x123)
 * ══════════════════════════════════════════════════════════════
 * Byte 0: [HB(1) | SEQ(3) | CI(2) | XX(2)]
 * Byte 1: [AB(7) | YY(1)]
 * Byte 2-3: [EDC(4) | PADDING(12)]
 * Byte 4-5: [N2OPS(16)]       -- Pression N2O (Pa)
 * Byte 6-7: [TLC(12) | N2ODVS(1) | N2OIV(1) | N2OMV(1) | XX(1)]
 *
 * Total: 15 bits utilisés, reste pour extension
 */
```

---

Voir aussi :
- [03_DONNEES_CAN.md](03_DONNEES_CAN.md) - Utilisation avancée du buffer
- [05_EXEMPLES_COMPLETS.md](05_EXEMPLES_COMPLETS.md) - Exemples concrets
