# Guide d'Utilisation de TAGCAN

## Aperçu

Ce guide pratique vous aidera à utiliser **TAGCAN**, une bibliothèque de gestion de bus CAN pour systèmes embarqués STM32. La documentation s'appuie sur l'exemple d'implémentation présent dans le dossier `exemple_utilisation/`.

## Structure du guide

- **[01_INITIALISATION.md](01_INITIALISATION.md)** - Mise en place du système TAGCAN et du bus CAN
- **[02_GESTION_TAGS.md](02_GESTION_TAGS.md)** - Création et gestion des tags (variables nommées)
- **[03_DONNEES_CAN.md](03_DONNEES_CAN.md)** - Manipulation du buffer de données CAN
- **[04_FILTRAGE_CAN.md](04_FILTRAGE_CAN.md)** - Configuration des filtres CAN
- **[05_EXEMPLES_COMPLETS.md](05_EXEMPLES_COMPLETS.md)** - Exemples pratiques et cas d'usage
- **[06_VISUALISATION.md](06_VISUALISATION.md)** - Fonctions de visualisation et outils de debug

## Points clés

### Qu'est-ce qu'un tag ?

Un **tag** est une variable nommée associée à une plage de bits dans le buffer CAN. Par exemple :
- `"SEQ"` -> 3 bits (numéro de séquence)
- `"HB"` -> 1 bit (heartbeat/watchdog)
- `"N2OPS"` -> 16 bits (capteur de pression N2O)

### Architecture générale

```
┌─────────────────────────────────────────────┐
│   Application utilisateur (main.c)          │
├─────────────────────────────────────────────┤
│   TAGCAN (Gestion des tags & données)       │
│  ├─ Tag Manager (init_tag_manager, set_tag) │
│  └─ Data Container (set_value, get_value)   │
├─────────────────────────────────────────────┤
│   Driver CAN STM32 (HAL)                    │
└─────────────────────────────────────────────┘
```

## Démarrage rapide

```c
// 1. Initialiser TAGCAN
void init_com(void) {
    CAN_init_TAGCAN();      // Initialise le gestionnaire de tags
    CAN_setup_TAG();        // Crée les tags applicatifs
    CAN_setup_DF(0x123);    // Configure le header CAN
}

// 2. Écrire une valeur dans un tag
set_value("SEQ", 5);        // Écrit 5 dans le tag "SEQ" (3 bits)

// 3. Lire une valeur depuis un tag
uint32_t val;
get_value("HB", &val);      // Lit la valeur du tag "HB"

// 4. Envoyer sur le bus CAN
HAL_CAN_AddTxMessage(&hcan, &TxHeader, TxData, &TxMailbox);
```

## Documentation de référence

Pour plus de détails sur l'API : voir [API_REFERENCE.md](../documentation/API_REFERENCE.md)

##  Fichiers sources associés

- `exemple_utilisation/com_utils.c` - Code d'exemple d'intégration
- `exemple_utilisation/com_utils.h` - Headers d'exemple
- `TAGCAN.h` - Point d'entrée principal

---

**Besoin d'aide ?** Consultez les exemples complets dans [05_EXEMPLES_COMPLETS.md](05_EXEMPLES_COMPLETS.md)
