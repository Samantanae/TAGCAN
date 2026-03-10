# 03 - Manipulation du Buffer de Données CAN

##  Objectif

Comprendre comment gérer le buffer de données CAN (`TxData` et `RxData`) au-delà de la simple interface des tags.

---

##  Structure du buffer CAN

### Taille et format

```c
uint8_t TxData[8];    // Buffer de transmission (8 octets = CAN standard)
uint8_t RxData[8];    // Buffer de réception (8 octets)
```

Le buffer CAN est toujours de **8 octets** (64 bits) pour un frame standard CAN.

### Hiérarchie

| Niveau | Composant | Responsabilité |
|--------|-----------|-----------------|
| **Haut** | Application | Appelle `set_value()`, `get_value()` |
| **Moyen** | Data Container (TAGCAN) | Mappe les tags dans le buffer |
| **Bas** | Buffer `TxData[8]` / `RxData[8]` | Storiage binaire brut |
| **Très bas** | HAL CAN STM32 | Envoi/réception matérielle |

---

##  Fonctions de gestion du buffer

### 1 init_data_container()

Réinitialise le buffer à zéro.

```c
void init_data_container(void);
```

**Effet** :
- Tous les octets de `TxData[8]` -> 0
- Appelé manuellement si vous voulez "effacer" les données

**Exemple** :
```c
// Avant:  TxData = {0x12, 0x34, 0x56, ...}
init_data_container();
// Après:  TxData = {0x00, 0x00, 0x00, ...}
```

---

### 2 set_val_TxData()

Copie un tableau **externe** dans le buffer global `TxData`.

```c
void set_val_TxData(uint8_t *TxDataArray);
```

**Utile pour** :
- Charger des données pré-formatées
- Importer des données d'une autre source
- Tester sans passer par les tags

**Exemple** :
```c
uint8_t raw_data[8] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00, 0x11};
set_val_TxData(raw_data);  // Copie raw_data -> TxData

// Après cet appel:
// TxData[0] = 0xAA
// TxData[1] = 0xBB
// ... et cetera

//  Les tags ne voient PAS cette modification directement
// Vous travaillez maintenant "en dehors" du système de tags
```

---

### 3 can_simulate_send_receive()

Simule un envoi/réception CAN par copie buffer-à-buffer.

```c
void can_simulate_send_receive(uint8_t* tx_buffer, uint8_t* rx_buffer);
```

**Cas d'usage** :
- Tests de la logique sans matériel CAN
- Vérifier que les données écrites sont bien formatées
- Boucle locale pour déboguer

**Exemple** :
```c
uint8_t tx[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t rx[8];

// Écrire des données dans tx
set_value("SEQ", 3);
can_simulate_send_receive(TxData, rx);  // Simule envoi/réception

// rx contient maintenant une copie de TxData
memcpy(rx, TxData, 8);  // Équivalent à:
                         // can_simulate_send_receive(TxData, rx);
```

---

##  Workflow complet : Écriture -> Envoi -> Réception

### Émission (Tx)

```
Votre application
        ↓
    set_value("SEQ", 5)
    set_value("AB", 75)
    ...
        ↓
   TxData[8] (rempli par TAGCAN)
        ↓
   Transmission CAN (HAL)
```

### Code d'émission

```c
#include "TAGCAN.h"

extern CAN_HandleTypeDef hcan;
extern uint8_t TxData[8];
extern CAN_TxHeaderTypeDef TxHeader;
extern int32_t TxMailbox;

void send_can_frame(void) {
    // Étape 1: Réinitialiser le buffer (optionnel)
    // init_data_container();  // Si vous voulez un buffer vierge

    // Étape 2: Écrire les données via les tags
    set_value("SEQ", 5);
    set_value("HB", 1);
    set_value("AB", 75);
    set_value("N2OPS", 2500);

    // Étape 3: Envoyer via CAN
    HAL_Status = HAL_CAN_AddTxMessage(&hcan, &TxHeader, TxData, &TxMailbox);
    if(HAL_Status != HAL_OK) {
        printf("Erreur d'envoi CAN\n");
    }
}
```

### Réception (Rx)

```
    Bus CAN
        ↓
 Réception CAN (HAL)
        ↓
    RxData[8]
        ↓
  Votre application récupère les données
  via get_value() ou directement dans RxData[8]
```

### Code de réception

```c
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
    CAN_RxHeaderTypeDef RxHeader;
    uint8_t RxData[8];

    // Étape 1: Recevoir le message via HAL
    if(HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData) != HAL_OK) {
        return;  // Erreur
    }

    // Étape 2a: Utiliser les données directement (acccès brut)
    printf("Octet 0: 0x%02X\n", RxData[0]);
    printf("Octet 1: 0x%02X\n", RxData[1]);

    // OU

    // Étape 2b: Interpréter via les tags
    // (si vous avez le même schéma de tags côté récepteur)
    uint32_t seq, battery;
    if(RxData[0] & 0x38) {  // Bits 3-5 contiennent possibly SEQ
        // Extraire manuellement...
    }

    // Ou utiliser les fonctions TAGCAN si vous avez copié RxData dans TxData:
    memcpy(TxData, RxData, 8);  // Copier RxData -> TxData
    if(get_value("SEQ", &seq) == CAN_TG_SUCCESS) {
        printf("Numéro de séquence distant: %u\n", seq);
    }
}
```

---

##  Directement lire/écrire les octets

### Lecture directe

```c
// Accéder directement à un octet
uint8_t byte0 = TxData[0];  // Premier octet
uint8_t byte7 = TxData[7];  // Dernier octet

// Accéder à un bit spécifique
uint8_t bit3 = (TxData[0] >> 3) & 1;  // Bit 3 de l'octet 0
```

### Écriture directe

```c
// Écrire un octet entier
TxData[0] = 0xA5;  // Écrit 0b10100101

// Modifier un bit spécifique
TxData[0] |= (1 << 3);   // Mettre à 1 le bit 3
TxData[0] &= ~(1 << 3);  // Mettre à 0 le bit 3

// Bitmask pour plusieurs bits
TxData[0] = (TxData[0] & 0xF0) | 0x05;  // Garder les 4 bits hauts, définir bas à 0x05
```

---

##  Pièges courants

|  Piège | Description | Solution |
|---------|-------------|----------|
| **Mélanger accès direct et tags** | Écrire un octet directement, puis utiliser `get_value()` peut être confus | Soyez cohérent: ou tags, ou accès brut |
| **Oublier le `memcpy()`** | Recevoir `RxData`, mais essayer de lire avec `get_value()` sans copier | Toujours copier `RxData` -> `TxData` avant `get_value()` |
| **Buffer réutilisé** | Si `set_value()` change l'ordre de allocation des tags | Pas vraiment un piège, mais attention si tags réordonnés |
| **Débordement de bits** | Écrire un octet directement sans respecter le schéma de tags | Documentez votre layout de tags ! |

---

##  Tableau d'aide : Qui fait quoi ?

| Tâche | Fonction | Détails |
|------|----------|---------|
| Vider le buffer | `init_data_container()` | Remplace TxData par des zéros |
| Écrire une valeur nommée | `set_value(tag, val)` | Via l'interface des tags |
| Lire une valeur nommée | `get_value(tag, &val)` | Via l'interface des tags |
| Charger des données brutes | `set_val_TxData(arr)` | Copie un tableau externe |
| Tester localement | `can_simulate_send_receive()` | Simule une boucle locale |
| Envoyer sur le bus | `HAL_CAN_AddTxMessage()` | Transport matériel (HAL STM32) |
| Recevoir du bus | `HAL_CAN_GetRxMessage()` | Transport matériel (HAL STM32) |

---

##  Exemple : Envoi -> Réception -> Traitement

```c
#include "TAGCAN.h"
#include "stm32f1xx_hal.h"

extern CAN_HandleTypeDef hcan;
extern uint8_t TxData[8], RxData[8];
extern CAN_TxHeaderTypeDef TxHeader;
extern int32_t TxMailbox;

// Tâche d'émission (toutes les 10ms)
void task_tx_10ms(void) {
    static uint8_t seq = 0;
    seq = (seq + 1) % 8;

    // Écrire les données
    set_value("SEQ", seq);
    set_value("HB", 1);
    set_value("AB", read_battery_percent());

    // Envoyer
    HAL_CAN_AddTxMessage(&hcan, &TxHeader, TxData, &TxMailbox);
}

// Callback de réception (IRQ)
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
    CAN_RxHeaderTypeDef RxHeader;
    uint8_t RxDataLocal[8];

    HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxDataLocal);

    // Voir directement les octets reçus
    printf("Reçu: ");
    for(int i = 0; i < 8; i++) {
        printf("%02X ", RxDataLocal[i]);
    }
    printf("\n");

    // Si le même schéma de tags : interpréter via TAGCAN
    memcpy(TxData, RxDataLocal, 8);
    uint32_t remote_seq, remote_battery;
    get_value("SEQ", &remote_seq);
    get_value("AB", &remote_battery);
    printf("Séquence: %u, Batterie: %u%%\n", remote_seq, remote_battery);
}

int main(void) {
    HAL_Init();
    MX_CAN_Init();
    init_com();

    // Timer pour task_tx_10ms()
    // ...

    while(1) {
        // Boucle principale
    }
}
```

---

Voir aussi :
- [02_GESTION_TAGS.md](02_GESTION_TAGS.md) - Tags et variables nommées
- [04_FILTRAGE_CAN.md](04_FILTRAGE_CAN.md) - Filtrage des messages reçus
