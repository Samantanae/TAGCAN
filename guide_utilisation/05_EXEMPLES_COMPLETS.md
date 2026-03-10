# 05 — Exemples Complets

## 📘 Exemples d'intégration pratiques

Vous trouverez ici des exemples réalistes exploitant l'ensemble du système TAGCAN.

---

## 📌 Exemple 1 : Système de télémétrie simple

### Scénario

Un véhicule envoie périodiquement :
- Son numéro de séquence (3 bits)
- Son état (heartbeat, 1 bit)
- Le % de batterie (7 bits)
- La pression du réservoir (16 bits)
- Un timestamp (16 bits)

### Code complet

```c
// =============================================================================
// telemetrie.h
// =============================================================================

#ifndef TELEMETRIE_H_
#define TELEMETRIE_H_

#include "TAGCAN.h"
#include <stdint.h>

typedef struct {
    uint8_t seq;
    uint8_t heartbeat;
    uint8_t battery_percent;
    uint16_t reservoir_pressure;
    uint16_t timestamp_ms;
} telemetry_data_t;

// Initialiser le système de télémétrie
void telemetry_init(void);

// Envoyer un paquet de télémétrie
void telemetry_send(const telemetry_data_t *data);

// Traiter un paquet reçu
void telemetry_handle_rx(const uint8_t rx_data[8]);

#endif /* TELEMETRIE_H_ */


// =============================================================================
// telemetrie.c
// =============================================================================

#include "telemetrie.h"
#include "stm32f1xx_hal.h"

extern CAN_HandleTypeDef hcan;
extern uint8_t TxData[8], RxData[8];
extern CAN_TxHeaderTypeDef TxHeader;
extern int32_t TxMailbox;

#define TELEMETRY_CAN_ID 0x100

void telemetry_init(void) {
    // Initialiser TAGCAN
    init_data_container();
    init_tag_manager();

    // Créer les tags
    set_tag("SEQ", 3);          // Numéro de séquence (0-7)
    set_tag("HB", 1);           // Heartbeat (0 ou 1)
    set_tag("BATTERY", 7);      // % batterie (0-100)
    set_tag("PRESSURE", 16);    // Pression réservoir (0-65535 Pa)
    set_tag("TIME", 16);        // Timestamp (0-65535 ms)

    // Configurer le header CAN
    TxHeader.IDE = CAN_ID_STD;
    TxHeader.StdId = TELEMETRY_CAN_ID;
    TxHeader.RTR = CAN_RTR_DATA;
    TxHeader.DLC = 8;

    // Configurer le filtre pour recevoir les télémétries des autres (ex: 0x101-0x10F)
    CAN_setup_Filter(0x101, 0x10F, 0);

    // Activer les interruptions
    if(HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK) {
        Error_Handler();
    }

    printf("[Telemetry] Initialized on CAN ID 0x%03X\n", TELEMETRY_CAN_ID);
}

void telemetry_send(const telemetry_data_t *data) {
    // Réinitialiser le buffer
    init_data_container();

    // Écrire les données
    set_value("SEQ", data->seq);
    set_value("HB", data->heartbeat);
    set_value("BATTERY", data->battery_percent);
    set_value("PRESSURE", data->reservoir_pressure);
    set_value("TIME", data->timestamp_ms);

    // Envoyer sur le bus
    if(HAL_CAN_AddTxMessage(&hcan, &TxHeader, TxData, &TxMailbox) != HAL_OK) {
        printf("[Telemetry] ERROR: Failed to send CAN message\n");
    } else {
        printf("[Telemetry] TX: SEQ=%u, BAT=%u%%, PRESSURE=%u Pa\n",
               data->seq, data->battery_percent, data->reservoir_pressure);
    }
}

void telemetry_handle_rx(const uint8_t rx_data[8]) {
    // Copier les données reçues dans TxData pour utiliser TAGCAN
    memcpy(TxData, rx_data, 8);

    // Lire via les tags
    uint32_t seq, hb, battery, pressure, timestamp;
    get_value("SEQ", &seq);
    get_value("HB", &hb);
    get_value("BATTERY", &battery);
    get_value("PRESSURE", &pressure);
    get_value("TIME", &timestamp);

    printf("[Telemetry] RX: SEQ=%u, HB=%u, BAT=%u%%, PRESSURE=%u Pa, TIME=%u ms\n",
           (uint8_t)seq, (uint8_t)hb, (uint8_t)battery, (uint16_t)pressure, (uint16_t)timestamp);
}

// =============================================================================
// Callback d'interruption CAN
// =============================================================================

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
    CAN_RxHeaderTypeDef RxHeader;
    uint8_t RxDataLocal[8];

    if(HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxDataLocal) == HAL_OK) {
        // Traiter selon l'ID CAN reçu
        if((RxHeader.StdId >= 0x101) && (RxHeader.StdId <= 0x10F)) {
            telemetry_handle_rx(RxDataLocal);
        }
    }
}

// =============================================================================
// main.c - Boucle principale
// =============================================================================

// Tâche d'émission (10 ms)
void task_telemetry_10ms(void) {
    static uint8_t seq = 0;
    static uint32_t last_time = 0;

    telemetry_data_t data = {
        .seq = (seq++) % 8,
        .heartbeat = 1,
        .battery_percent = read_adc_battery(),
        .reservoir_pressure = read_sensor_pressure(),
        .timestamp_ms = HAL_GetTick() & 0xFFFF
    };

    if((HAL_GetTick() - last_time) >= 10) {
        telemetry_send(&data);
        last_time = HAL_GetTick();
    }
}

int main(void) {
    HAL_Init();
    MX_CAN_Init();
    
    telemetry_init();

    // Timer pour appeler task_telemetry_10ms() tous les 10ms
    // ...

    while(1) {
        task_telemetry_10ms();
    }
}
```

---

## 📌 Exemple 2 : Système de commande avec réponse

### Scénario

- **Maître envoie commandes** sur ID 0x200 (allumer/éteindre servos)
- **Esclaves répondent** sur ID 0x201-0x203 (confirmation + état)

### Code complet

```c
// =============================================================================
// commandes.h
// =============================================================================

#ifndef COMMANDES_H_
#define COMMANDES_H_

#include "TAGCAN.h"
#include <stdint.h>

// IDs CAN
#define CMD_CAN_ID_TX       0x200
#define CMD_CAN_ID_RX_BASE  0x201

// Commandes possibles
typedef enum {
    CMD_SERVO_OFF = 0,
    CMD_SERVO_ON = 1,
    CMD_SERVO_PULSE = 2
} cmd_type_t;

typedef struct {
    uint8_t device_id;      // Quel servomoteur ? (1-3)
    cmd_type_t command;     // Quelle commande ?
    uint8_t parameter;      // Paramètre optionnel (ex: pulse width)
} command_t;

typedef struct {
    uint8_t device_id;
    uint8_t status;         // 0=erreur, 1=ok
    uint8_t feedback;       // Retour d'état
} response_t;

void commands_init(void);
void commands_send(const command_t *cmd);
void commands_handle_response(uint8_t device_id, const uint8_t rx_data[8]);

#endif


// =============================================================================
// commandes.c
// =============================================================================

#include "commandes.h"
#include "stm32f1xx_hal.h"

extern CAN_HandleTypeDef hcan;
extern uint8_t TxData[8];
extern CAN_TxHeaderTypeDef TxHeader;
extern int32_t TxMailbox;

void commands_init(void) {
    // Initialiser TAGCAN
    init_data_container();
    init_tag_manager();

    // Tags pour les commandes ENVOYÉES
    set_tag("DEV_ID", 2);       // ID du périphérique (0-3)
    set_tag("CMD_TYPE", 2);     // Type de commande (0-3)
    set_tag("PARAM", 8);        // Paramètre (0-255)

    // Configuration du header TX
    TxHeader.IDE = CAN_ID_STD;
    TxHeader.StdId = CMD_CAN_ID_TX;
    TxHeader.RTR = CAN_RTR_DATA;
    TxHeader.DLC = 8;

    // Filtre pour les réponses des esclaves (0x201-0x203)
    CAN_setup_Filter(0x201, 0x203, 0);

    // Interruptions
    if(HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK) {
        Error_Handler();
    }

    printf("[Commands] Initialized\n");
}

void commands_send(const command_t *cmd) {
    init_data_container();

    set_value("DEV_ID", cmd->device_id);
    set_value("CMD_TYPE", cmd->command);
    set_value("PARAM", cmd->parameter);

    if(HAL_CAN_AddTxMessage(&hcan, &TxHeader, TxData, &TxMailbox) != HAL_OK) {
        printf("[Commands] ERROR: Send failed\n");
    } else {
        printf("[Commands] TX: Device=%u, Command=%u, Param=%u\n",
               cmd->device_id, cmd->command, cmd->parameter);
    }
}

void commands_handle_response(uint8_t device_id, const uint8_t rx_data[8]) {
    memcpy(TxData, rx_data, 8);

    uint32_t status, feedback;
    get_value("DEV_ID", NULL);  // Ou extraire directement
    get_value("STATUS", &status);
    get_value("FEEDBACK", &feedback);

    printf("[Commands] RX Response from Device %u: Status=%u, Feedback=%u\n",
           device_id, (uint8_t)status, (uint8_t)feedback);
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
    CAN_RxHeaderTypeDef RxHeader;
    uint8_t RxDataLocal[8];

    if(HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxDataLocal) == HAL_OK) {
        uint8_t device_id = RxHeader.StdId - CMD_CAN_ID_RX_BASE + 1;
        if((RxHeader.StdId >= 0x201) && (RxHeader.StdId <= 0x203)) {
            commands_handle_response(device_id, RxDataLocal);
        }
    }
}

// =============================================================================
// main.c - Exemple d'utilisation
// =============================================================================

int main(void) {
    HAL_Init();
    MX_CAN_Init();

    commands_init();

    // Envoyer la commande "Allumer servomoteur 1"
    command_t cmd1 = {
        .device_id = 1,
        .command = CMD_SERVO_ON,
        .parameter = 0
    };
    commands_send(&cmd1);

    // Attendre la réponse (via callback HAL_CAN_RxFifo0MsgPendingCallback)
    HAL_Delay(100);

    // Envoyer la commande "Servomoteur 2, position pulse 100"
    command_t cmd2 = {
        .device_id = 2,
        .command = CMD_SERVO_PULSE,
        .parameter = 100
    };
    commands_send(&cmd2);

    while(1) {
        // Boucle principale
    }
}
```

---

## 📌 Exemple 3 : Test en boucle locale (sans matériel CAN)

### Scénario

Tester la logique TAGCAN **sans** bus CAN physique.

```c
// =============================================================================
// test_tagcan_local.c
// =============================================================================

#include "TAGCAN.h"
#include <stdio.h>
#include <string.h>

uint8_t TxData[8] = {0};
uint8_t RxData[8] = {0};

void test_local_loop(void) {
    printf("=== TEST BOUCLE LOCALE TAGCAN ===\n\n");

    // Étape 1: Initialiser
    init_data_container();
    init_tag_manager();

    set_tag("SEQ", 3);
    set_tag("SENSOR_A", 8);
    set_tag("SENSOR_B", 12);

    printf("✓ Initialisé\n\n");

    // Étape 2: Écrire les données
    printf("Écriture des données...\n");
    set_value("SEQ", 5);
    set_value("SENSOR_A", 42);
    set_value("SENSOR_B", 1000);

    // Afficher le buffer brut
    printf("Buffer TxData: ");
    for(int i = 0; i < 8; i++) {
        printf("%02X ", TxData[i]);
    }
    printf("\n\n");

    // Étape 3: Simuler envoi → réception
    printf("Simulation envoi/réception...\n");
    can_simulate_send_receive(TxData, RxData);

    // Vérifier
    printf("Buffer RxData: ");
    for(int i = 0; i < 8; i++) {
        printf("%02X ", RxData[i]);
    }
    printf("\n\n");

    // Étape 4: Lire les données reçues
    printf("Lecture des données reçues...\n");
    memcpy(TxData, RxData, 8);  // Copier pour lecture

    uint32_t seq, sens_a, sens_b;
    get_value("SEQ", &seq);
    get_value("SENSOR_A", &sens_a);
    get_value("SENSOR_B", &sens_b);

    printf("SEQ = %u\n", (uint8_t)seq);
    printf("SENSOR_A = %u\n", (uint8_t)sens_a);
    printf("SENSOR_B = %u\n", (uint16_t)sens_b);

    // Vérifier
    if(seq == 5 && sens_a == 42 && sens_b == 1000) {
        printf("\n✅ TEST RÉUSSI!\n");
    } else {
        printf("\n❌ TEST ÉCHOUÉ!\n");
    }
}

int main(void) {
    test_local_loop();
    return 0;
}
```

**Sortie attendue** :
```
=== TEST BOUCLE LOCALE TAGCAN ===

✓ Initialisé

Écriture des données...
Buffer TxData: XX XX XX XX XX XX XX XX

Simulation envoi/réception...
Buffer RxData: XX XX XX XX XX XX XX XX

Lecture des données reçues...
SEQ = 5
SENSOR_A = 42
SENSOR_B = 1000

✅ TEST RÉUSSI!
```

---

## 📌 Exemple 4 : Debug / Monitoring CAN

### Scénario

Logger tous les messages CAN reçus pour du monitoring.

```c
// =============================================================================
// can_monitor.h
// =============================================================================

#ifndef CAN_MONITOR_H_
#define CAN_MONITOR_H_

#include <stdint.h>

typedef struct {
    uint32_t id;
    uint8_t dlc;
    uint8_t data[8];
    uint32_t timestamp_ms;
} can_frame_t;

#define CAN_MONITOR_BUFFER_SIZE 100

void can_monitor_init(void);
void can_monitor_log(const can_frame_t *frame);
void can_monitor_dump(void);

#endif


// =============================================================================
// can_monitor.c
// =============================================================================

#include "can_monitor.h"
#include "stm32f1xx_hal.h"
#include <stdio.h>
#include <string.h>

static can_frame_t monitor_buffer[CAN_MONITOR_BUFFER_SIZE];
static uint32_t monitor_index = 0;

void can_monitor_init(void) {
    monitor_index = 0;
    memset(monitor_buffer, 0, sizeof(monitor_buffer));
    printf("[Monitor] Initialized\n");
}

void can_monitor_log(const can_frame_t *frame) {
    if(monitor_index >= CAN_MONITOR_BUFFER_SIZE) {
        monitor_index = 0;  // Circulaire
    }

    memcpy(&monitor_buffer[monitor_index], frame, sizeof(can_frame_t));
    monitor_index++;
}

void can_monitor_dump(void) {
    printf("\n=== CAN FRAME LOG ===\n");
    printf("| Index | ID    | DLC | Data                 | Timestamp |\n");
    printf("|-------|-------|-----|----------------------|-----------|\n");

    for(uint32_t i = 0; i < monitor_index && i < CAN_MONITOR_BUFFER_SIZE; i++) {
        const can_frame_t *frame = &monitor_buffer[i];

        printf("| %5u | 0x%03X | %u   | ", i, (unsigned)frame->id, frame->dlc);
        for(int j = 0; j < 8; j++) {
            printf("%02X ", frame->data[j]);
        }
        printf("| %9u |\n", (unsigned)frame->timestamp_ms);
    }
    printf("\n");
}

// =============================================================================
// Utilisation dans le callback CAN
// =============================================================================

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
    CAN_RxHeaderTypeDef RxHeader;
    uint8_t RxDataLocal[8];

    if(HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxDataLocal) == HAL_OK) {
        can_frame_t frame = {
            .id = RxHeader.StdId,
            .dlc = RxHeader.DLC,
            .timestamp_ms = HAL_GetTick()
        };
        memcpy(frame.data, RxDataLocal, 8);

        can_monitor_log(&frame);
    }
}

// =============================================================================
// main.c
// =============================================================================

int main(void) {
    HAL_Init();
    MX_CAN_Init();

    can_monitor_init();

    // ... votre logique CAN ...

    // Afficher le log après 10 secondes
    HAL_Delay(10000);
    can_monitor_dump();

    while(1) {
        // ...
    }
}
```

---

## 🔗 Checklist d'implémentation

- [ ] Inclure `TAGCAN.h`
- [ ] Appeler `init_data_container()`
- [ ] Appeler `init_tag_manager()`
- [ ] Créer les tags via `set_tag()`
- [ ] Configurer le header CAN via `CAN_setup_DF()`
- [ ] Configurer les filtres via `CAN_setup_Filter()`
- [ ] Activer les interruptions
- [ ] Implémenter `HAL_CAN_RxFifo0MsgPendingCallback()`
- [ ] Utiliser `set_value()` pour écrire
- [ ] Utiliser `get_value()` pour lire
- [ ] Envoyer via `HAL_CAN_AddTxMessage()`
- [ ] Recevoir via `HAL_CAN_GetRxMessage()`
- [ ] Tester en boucle locale (optionnel mais recommandé)

---

Besoin d'aide ? Retour au [README.md](README.md)
