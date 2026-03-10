# 04 — Filtrage CAN

## 🎯 Objectif

Configurer les filtres CAN pour recevoir **uniquement** les messages pertinents et ignorer le reste du trafic.

---

## 📚 Contexte

Le bus CAN est partagé. Un nœud peut recevoir des messages destinés à d'autres nœuds. Les **filtres CAN** permettent :

- Réduire la charge CPU (moins d'interruptions inutiles)
- Améliorer la latence (traiter uniquement les messages pertinents)
- Séparer logiquement le trafic

---

## 🔧 Fonction de filtrage

### Prototype

```c
CAN_FilterTypeDef* CAN_setup_Filter(int32_t minimum_id, int32_t maximum_id, int filterBank);
```

### Paramètres

| Paramètre | Type | Description |
|-----------|------|-------------|
| `minimum_id` | `int32_t` | ID CAN minimum à accepter |
| `maximum_id` | `int32_t` | ID CAN maximum à accepter |
| `filterBank` | `int` | Numéro de banc de filtre (0-27 disponibles) |

### Valeur de retour

Pointeur vers la structure `CAN_FilterTypeDef` configurée.

---

## 📋 Comment ça marche ?

### Le filtre masque (IDMASK)

```c
CAN_FilterTypeDef canfilterconfig;
canfilterconfig.FilterMode = CAN_FILTERMODE_IDMASK;  // Mode masque
```

**Mode masque** : Les bits du filtre agissent comme un masque binaire.

```
ID reçu :     0011 0101
Filtre :      1111 1111  (masque)
Résultat :    Accepter si bits importants Match
```

---

## 🔨 Exemple de configuration

### Cas 1 : Accepter une plage d'IDs (0x100 à 0x120)

```c
CAN_FilterTypeDef* filter1 = CAN_setup_Filter(0x100, 0x120, 0);
// Accepte les IDs: 0x100, 0x101, 0x102, ..., 0x120
```

### Cas 2 : Accepter un ID exact (0x123)

```c
CAN_FilterTypeDef* filter2 = CAN_setup_Filter(0x123, 0x123, 1);
// Accepte UNIQUEMENT: 0x123
```

### Cas 3 : Accepter tous les messages

```c
CAN_FilterTypeDef* filter3 = CAN_setup_Filter(0x000, 0x7FF, 2);
// Accepte les IDs: 0x000 à 0x7FF (tous les IDs standards 11 bits)
```

---

## ⚙️ Détails de la configuration interne

Voici ce que fait `CAN_setup_Filter()` en détail :

```c
CAN_FilterTypeDef* CAN_setup_Filter(int32_t minimum_id, int32_t maximum_id, int filterBank) {
    CAN_FilterTypeDef canfilterconfig;

    // Activer le filtre
    canfilterconfig.FilterActivation = CAN_FILTER_ENABLE;

    // Sélectionner le numéro de banc (0-27)
    canfilterconfig.FilterBank = filterBank;

    // Affecter à FIFO0 (interrupt CAN_IT_RX_FIFO0_MSG_PENDING)
    canfilterconfig.FilterFIFOAssignment = CAN_FILTER_FIFO0;

    // Configurer les seuils ID haut et bas
    canfilterconfig.FilterIdHigh = maximum_id << 5;    // Maximum
    canfilterconfig.FilterMaskIdHigh = maximum_id << 5; // Masque
    
    canfilterconfig.FilterIdLow = minimum_id;           // Minimum
    canfilterconfig.FilterMaskIdLow = minimum_id;       // Masque

    // Mode de filtrage (masque binaire)
    canfilterconfig.FilterMode = CAN_FILTERMODE_IDMASK;

    // Résolution: 32 bits (combinaison High + Low)
    canfilterconfig.FilterScale = CAN_FILTERSCALE_32BIT;

    // Assigner les bancs de filtrage au CAN1 (non CAN2)
    canfilterconfig.SlaveStartFilterBank = 20;

    // Appliquer la configuration matérielle
    HAL_CAN_ConfigFilter(&hcan, &canfilterconfig);

    return &canfilterconfig;
}
```

---

## 🔌 Exemple d'intégration complet

### Initialisation avec filtrage

```c
#include "TAGCAN.h"
#include "stm32f1xx_hal.h"

extern CAN_HandleTypeDef hcan;

void init_com(void) {
    // 1. Initialiser TAGCAN
    CAN_init_TAGCAN();
    CAN_setup_TAG();
    CAN_setup_DF(0x123);  // Mon ID CAN

    // 2. Configurer les filtres
    // Filtre 1: Accepter les messages du capteur de pression (0x200)
    CAN_setup_Filter(0x200, 0x200, 0);

    // Filtre 2: Accepter les messages de commande (0x100 à 0x110)
    CAN_setup_Filter(0x100, 0x110, 1);

    // Filtre 3: Messages d'urgence (0x050)
    CAN_setup_Filter(0x050, 0x050, 2);

    // 3. Activer les interruptions
    if(HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK) {
        Error_Handler();
    }
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
    CAN_RxHeaderTypeDef RxHeader;
    uint8_t RxData[8];

    HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData);

    // Traiter selon l'ID reçu
    switch(RxHeader.StdId) {
        case 0x200:
            handle_pressure_sensor(RxData);
            break;
        case 0x100:
        case 0x101:
        case 0x102:
        // ... jusqu'à 0x110
            handle_command(RxData);
            break;
        case 0x050:
            handle_emergency(RxData);
            break;
        default:
            // Ne devrait jamais arriver ici grâce aux filtres
            break;
    }
}
```

---

## 🧮 Limitation des bancs de filtrage

Le STM32F1 propose :

| Ressource | Limite |
|-----------|--------|
| Bancs de filtrage disponibles | 28 (0-27) |
| Bancs réservés à CAN1 | 14 (0-13) |
| Bancs réservés à CAN2 | 14 (14-27) |

Chaque filtre peut gérer :
- **1 ID exact** (FilterMode = IDMASK)
- **Une plage d'IDs** (FilterMode = IDLIST)

---

## 🎯 Stratégies de filtrage recommandées

### Stratégie 1 : Filtrage par catégorie (Recommandé)

```c
// Tous les capteurs: 0x200-0x20F (1 filtre)
CAN_setup_Filter(0x200, 0x20F, 0);

// Toutes les commandes: 0x100-0x10F (1 filtre)
CAN_setup_Filter(0x100, 0x10F, 1);

// Tout le diagnostic: 0x300-0x3FF (1 filtre)
CAN_setup_Filter(0x300, 0x3FF, 2);
```

**Avantages** :
- Utilise peu de filtres
- Facile à maintenir
- Extensible

---

### Stratégie 2 : Un filtre par source

```c
// Capteur 1: 0x200
CAN_setup_Filter(0x200, 0x200, 0);

// Capteur 2: 0x201
CAN_setup_Filter(0x201, 0x201, 1);

// Capteur 3: 0x202
CAN_setup_Filter(0x202, 0x202, 2);

// ... etc
```

**Avantages** :
- Granularité fine
- Gestion précise

**Inconvénients** :
- Consomme rapidement les bancs de filtrage

---

## ⚠️ Points d'attention

| ⚠️ Point | Détail |
|---------|--------|
| **Ordre d'application** | Les filtres doivent être configurés AVANT d'activer les interruptions |
| **Limite de bancs** | 14 filtres max pour CAN1. Bien planifier! |
| **FIFO0 vs FIFO1** | `CAN_setup_Filter()` utilise FIFO0. À adapter si vous utilisez FIFO1 |
| **Pas de filtre = tout accepter** | Si aucun filtre n'est configuré, tous les messages passent |
| **Mode IDLIST vs IDMASK** | La fonction utilise IDMASK (plages). Pour listes, modifier le code |
| **Décalage des bits (<<5)** | Note: `maximum_id << 5` → convertit l'ID de 11 bits au format STM32 |

---

## 🐛 Débuggage des filtres

```c
// Vérifier quels messages arrivent
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
    CAN_RxHeaderTypeDef RxHeader;
    uint8_t RxData[8];

    HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData);

    // Afficher le message reçu
    printf("ID: 0x%03X, DLC: %u\n", RxHeader.StdId, RxHeader.DLC);
    for(int i = 0; i < RxHeader.DLC; i++) {
        printf("  Data[%d] = 0x%02X\n", i, RxData[i]);
    }
}
```

Si vous ne voyez **aucun message** malgré l'activité CAN :
- ✓ Vérifier que les filtres acceptent l'ID envoyé
- ✓ Vérifier que les interruptions sont activées
- ✓ Vérifier que le module CAN est initialisé

Si vous voyez **trop de messages** :
- ✓ Ajouter des filtres plus restrictifs
- ✓ Vérifier que les filtres ne sont pas inversés (min > max)

---

## 📋 Comparaison : Avec/Sans filtres

### Sans filtres (accepte tout)

```c
// Aucun CAN_setup_Filter() appelé
while(1) {
    // ❌ Reçoit TOUS les messages du bus
    // → CPU chargé, latence mauvaise
}
```

### Avec filtres (recommandé)

```c
// Dans init_com()
CAN_setup_Filter(0x200, 0x20F, 0);  // Capteurs
CAN_setup_Filter(0x100, 0x10F, 1);  // Commandes

while(1) {
    // ✅ Reçoit UNIQUEMENT les messages 0x200-0x20F et 0x100-0x10F
    // → CPU libre, latence optimale
}
```

---

Voir aussi :
- [03_DONNEES_CAN.md](03_DONNEES_CAN.md) — Buffer de données
- [05_EXEMPLES_COMPLETS.md](05_EXEMPLES_COMPLETS.md) — Système complet
