/*
 @licstart  The following is the entire license notice for the JavaScript code in this file.

 The MIT License (MIT)

 Copyright (C) 1997-2020 by Dimitri van Heesch

 Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 and associated documentation files (the "Software"), to deal in the Software without restriction,
 including without limitation the rights to use, copy, modify, merge, publish, distribute,
 sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all copies or
 substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

 @licend  The above is the entire license notice for the JavaScript code in this file
*/
var NAVTREE =
[
  [ "TAGCAN", "index.html", [
    [ "API Reference (Etat actuel)", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2documentation_2_a_p_i___r_e_f_e_r_e_n_c_e.html", [
      [ "Vue d'ensemble", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2documentation_2_a_p_i___r_e_f_e_r_e_n_c_e.html#autotoc_md1", null ],
      [ "Modules", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2documentation_2_a_p_i___r_e_f_e_r_e_n_c_e.html#autotoc_md2", null ],
      [ "Structures et etat global", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2documentation_2_a_p_i___r_e_f_e_r_e_n_c_e.html#autotoc_md3", [
        [ "<span class=\"tt\">TagDef</span> (<span class=\"tt\">include/sub_include/tag_manager.h</span>)", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2documentation_2_a_p_i___r_e_f_e_r_e_n_c_e.html#autotoc_md4", null ],
        [ "Variables globales", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2documentation_2_a_p_i___r_e_f_e_r_e_n_c_e.html#autotoc_md5", null ]
      ] ],
      [ "Fonctions principales", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2documentation_2_a_p_i___r_e_f_e_r_e_n_c_e.html#autotoc_md6", [
        [ "Gestion des tags", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2documentation_2_a_p_i___r_e_f_e_r_e_n_c_e.html#autotoc_md7", null ],
        [ "Valeurs (payload)", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2documentation_2_a_p_i___r_e_f_e_r_e_n_c_e.html#autotoc_md8", null ]
      ] ],
      [ "Debug / affichage", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2documentation_2_a_p_i___r_e_f_e_r_e_n_c_e.html#autotoc_md9", null ],
      [ "Codes de retour", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2documentation_2_a_p_i___r_e_f_e_r_e_n_c_e.html#autotoc_md10", [
        [ "Enum <span class=\"tt\">CAN_TG_STATUE</span> (<span class=\"tt\">include/config_value.h</span>)", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2documentation_2_a_p_i___r_e_f_e_r_e_n_c_e.html#autotoc_md11", null ],
        [ "Constantes globales (<span class=\"tt\">scripts/config_value.c</span>)", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2documentation_2_a_p_i___r_e_f_e_r_e_n_c_e.html#autotoc_md12", null ]
      ] ],
      [ "Notes importantes de comportement", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2documentation_2_a_p_i___r_e_f_e_r_e_n_c_e.html#autotoc_md13", null ]
    ] ],
    [ "01 - Initialisation de TAGCAN", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_201___i_n_i_t_i_a_l_i_s_a_t_i_o_n.html", [
      [ "Objectif", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_201___i_n_i_t_i_a_l_i_s_a_t_i_o_n.html#autotoc_md18", null ],
      [ "Vue d'ensemble du processus", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_201___i_n_i_t_i_a_l_i_s_a_t_i_o_n.html#autotoc_md19", null ],
      [ "Détail de chaque étape", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_201___i_n_i_t_i_a_l_i_s_a_t_i_o_n.html#autotoc_md20", [
        [ "Étape 1 : init_data_container()", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_201___i_n_i_t_i_a_l_i_s_a_t_i_o_n.html#autotoc_md21", null ]
      ] ],
      [ "- Atention: un appel en cours d'exécution réinitialiseras les valeurs à 0.", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_201___i_n_i_t_i_a_l_i_s_a_t_i_o_n.html#autotoc_md22", [
        [ "Étape 2 : init_tag_manager()", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_201___i_n_i_t_i_a_l_i_s_a_t_i_o_n.html#autotoc_md23", null ],
        [ "Étape 3 : CAN_setup_TAG()", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_201___i_n_i_t_i_a_l_i_s_a_t_i_o_n.html#autotoc_md25", null ],
        [ "Étape 4 : CAN_setup_DF()", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_201___i_n_i_t_i_a_l_i_s_a_t_i_o_n.html#autotoc_md27", null ],
        [ "Étape 5 : Activer les interruptions", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_201___i_n_i_t_i_a_l_i_s_a_t_i_o_n.html#autotoc_md29", null ]
      ] ],
      [ "Code complet d'initialisation", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_201___i_n_i_t_i_a_l_i_s_a_t_i_o_n.html#autotoc_md31", null ],
      [ "Points d'attention", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_201___i_n_i_t_i_a_l_i_s_a_t_i_o_n.html#autotoc_md33", null ],
      [ "Vérification", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_201___i_n_i_t_i_a_l_i_s_a_t_i_o_n.html#autotoc_md35", null ]
    ] ],
    [ "02 - Gestion des Tags", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_202___g_e_s_t_i_o_n___t_a_g_s.html", [
      [ "Qu'est-ce qu'un tag ?", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_202___g_e_s_t_i_o_n___t_a_g_s.html#autotoc_md38", null ],
      [ "Créer un tag : set_tag()", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_202___g_e_s_t_i_o_n___t_a_g_s.html#autotoc_md40", [
        [ "Prototype", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_202___g_e_s_t_i_o_n___t_a_g_s.html#autotoc_md41", null ],
        [ "Paramètres", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_202___g_e_s_t_i_o_n___t_a_g_s.html#autotoc_md42", null ],
        [ "Valeur de retour", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_202___g_e_s_t_i_o_n___t_a_g_s.html#autotoc_md43", null ],
        [ "Exemples", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_202___g_e_s_t_i_o_n___t_a_g_s.html#autotoc_md44", null ]
      ] ],
      [ "Écrire une valeur : set_value()", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_202___g_e_s_t_i_o_n___t_a_g_s.html#autotoc_md46", [
        [ "Prototype", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_202___g_e_s_t_i_o_n___t_a_g_s.html#autotoc_md47", null ],
        [ "Paramètres", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_202___g_e_s_t_i_o_n___t_a_g_s.html#autotoc_md48", null ],
        [ "Valeur de retour", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_202___g_e_s_t_i_o_n___t_a_g_s.html#autotoc_md49", null ],
        [ "Comportement important", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_202___g_e_s_t_i_o_n___t_a_g_s.html#autotoc_md50", null ],
        [ "Exemples d'utilisation", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_202___g_e_s_t_i_o_n___t_a_g_s.html#autotoc_md51", null ]
      ] ],
      [ "Lire une valeur : get_value()", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_202___g_e_s_t_i_o_n___t_a_g_s.html#autotoc_md53", [
        [ "Prototype", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_202___g_e_s_t_i_o_n___t_a_g_s.html#autotoc_md54", null ],
        [ "Paramètres", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_202___g_e_s_t_i_o_n___t_a_g_s.html#autotoc_md55", null ],
        [ "Valeur de retour", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_202___g_e_s_t_i_o_n___t_a_g_s.html#autotoc_md56", null ],
        [ "Exemples", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_202___g_e_s_t_i_o_n___t_a_g_s.html#autotoc_md57", null ]
      ] ],
      [ "Workflow complet : set / get / send", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_202___g_e_s_t_i_o_n___t_a_g_s.html#autotoc_md59", [
        [ "Scénario d'exemple", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_202___g_e_s_t_i_o_n___t_a_g_s.html#autotoc_md60", null ]
      ] ],
      [ "Bonnes pratiques", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_202___g_e_s_t_i_o_n___t_a_g_s.html#autotoc_md62", null ],
      [ "Schéma de tags recommandé", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_202___g_e_s_t_i_o_n___t_a_g_s.html#autotoc_md64", null ]
    ] ],
    [ "03 - Manipulation du Buffer de Données CAN", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_203___d_o_n_n_e_e_s___c_a_n.html", [
      [ "Objectif", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_203___d_o_n_n_e_e_s___c_a_n.html#autotoc_md67", null ],
      [ "Structure du buffer CAN", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_203___d_o_n_n_e_e_s___c_a_n.html#autotoc_md69", [
        [ "Taille et format", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_203___d_o_n_n_e_e_s___c_a_n.html#autotoc_md70", null ],
        [ "Hiérarchie", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_203___d_o_n_n_e_e_s___c_a_n.html#autotoc_md71", null ]
      ] ],
      [ "Fonctions de gestion du buffer", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_203___d_o_n_n_e_e_s___c_a_n.html#autotoc_md73", [
        [ "1 init_data_container()", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_203___d_o_n_n_e_e_s___c_a_n.html#autotoc_md74", null ],
        [ "2 set_val_TxData()", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_203___d_o_n_n_e_e_s___c_a_n.html#autotoc_md76", null ],
        [ "3 can_simulate_send_receive()", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_203___d_o_n_n_e_e_s___c_a_n.html#autotoc_md78", null ]
      ] ],
      [ "Workflow complet : Écriture -&gt; Envoi -&gt; Réception", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_203___d_o_n_n_e_e_s___c_a_n.html#autotoc_md80", [
        [ "Émission (Tx)", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_203___d_o_n_n_e_e_s___c_a_n.html#autotoc_md81", null ],
        [ "Code d'émission", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_203___d_o_n_n_e_e_s___c_a_n.html#autotoc_md82", null ],
        [ "Réception (Rx)", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_203___d_o_n_n_e_e_s___c_a_n.html#autotoc_md83", null ],
        [ "Code de réception", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_203___d_o_n_n_e_e_s___c_a_n.html#autotoc_md84", null ]
      ] ],
      [ "Directement lire/écrire les octets", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_203___d_o_n_n_e_e_s___c_a_n.html#autotoc_md86", [
        [ "Lecture directe", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_203___d_o_n_n_e_e_s___c_a_n.html#autotoc_md87", null ],
        [ "Écriture directe", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_203___d_o_n_n_e_e_s___c_a_n.html#autotoc_md88", null ]
      ] ],
      [ "Pièges courants", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_203___d_o_n_n_e_e_s___c_a_n.html#autotoc_md90", null ],
      [ "Tableau d'aide : Qui fait quoi ?", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_203___d_o_n_n_e_e_s___c_a_n.html#autotoc_md92", null ],
      [ "Exemple : Envoi -&gt; Réception -&gt; Traitement", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_203___d_o_n_n_e_e_s___c_a_n.html#autotoc_md94", null ]
    ] ],
    [ "04 - Filtrage CAN", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_204___f_i_l_t_r_a_g_e___c_a_n.html", [
      [ "Objectif", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_204___f_i_l_t_r_a_g_e___c_a_n.html#autotoc_md97", null ],
      [ "Contexte", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_204___f_i_l_t_r_a_g_e___c_a_n.html#autotoc_md99", null ],
      [ "Fonction de filtrage", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_204___f_i_l_t_r_a_g_e___c_a_n.html#autotoc_md101", [
        [ "Prototype", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_204___f_i_l_t_r_a_g_e___c_a_n.html#autotoc_md102", null ],
        [ "Paramètres", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_204___f_i_l_t_r_a_g_e___c_a_n.html#autotoc_md103", null ],
        [ "Valeur de retour", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_204___f_i_l_t_r_a_g_e___c_a_n.html#autotoc_md104", null ]
      ] ],
      [ "Comment ça marche ?", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_204___f_i_l_t_r_a_g_e___c_a_n.html#autotoc_md106", [
        [ "Le filtre masque (IDMASK)", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_204___f_i_l_t_r_a_g_e___c_a_n.html#autotoc_md107", null ]
      ] ],
      [ "Exemple de configuration", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_204___f_i_l_t_r_a_g_e___c_a_n.html#autotoc_md109", [
        [ "Cas 1 : Accepter une plage d'IDs (0x100 à 0x120)", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_204___f_i_l_t_r_a_g_e___c_a_n.html#autotoc_md110", null ],
        [ "Cas 2 : Accepter un ID exact (0x123)", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_204___f_i_l_t_r_a_g_e___c_a_n.html#autotoc_md111", null ],
        [ "Cas 3 : Accepter tous les messages", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_204___f_i_l_t_r_a_g_e___c_a_n.html#autotoc_md112", null ]
      ] ],
      [ "Détails de la configuration interne", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_204___f_i_l_t_r_a_g_e___c_a_n.html#autotoc_md114", null ],
      [ "Exemple d'intégration complet", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_204___f_i_l_t_r_a_g_e___c_a_n.html#autotoc_md116", [
        [ "Initialisation avec filtrage", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_204___f_i_l_t_r_a_g_e___c_a_n.html#autotoc_md117", null ]
      ] ],
      [ "Limitation des bancs de filtrage", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_204___f_i_l_t_r_a_g_e___c_a_n.html#autotoc_md119", null ],
      [ "Stratégies de filtrage recommandées", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_204___f_i_l_t_r_a_g_e___c_a_n.html#autotoc_md121", [
        [ "Stratégie 1 : Filtrage par catégorie (Recommandé)", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_204___f_i_l_t_r_a_g_e___c_a_n.html#autotoc_md122", null ],
        [ "Stratégie 2 : Un filtre par source", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_204___f_i_l_t_r_a_g_e___c_a_n.html#autotoc_md124", null ]
      ] ],
      [ "Points d'attention", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_204___f_i_l_t_r_a_g_e___c_a_n.html#autotoc_md126", null ],
      [ "Débuggage des filtres", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_204___f_i_l_t_r_a_g_e___c_a_n.html#autotoc_md128", null ],
      [ "Comparaison : Avec/Sans filtres", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_204___f_i_l_t_r_a_g_e___c_a_n.html#autotoc_md130", [
        [ "Sans filtres (accepte tout)", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_204___f_i_l_t_r_a_g_e___c_a_n.html#autotoc_md131", null ],
        [ "Avec filtres (recommandé)", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_204___f_i_l_t_r_a_g_e___c_a_n.html#autotoc_md132", null ]
      ] ]
    ] ],
    [ "05 - Exemples Complets", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_205___e286b88c2636fb4118cac9c95749c8f33.html", [
      [ "Exemples d'intégration pratiques", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_205___e286b88c2636fb4118cac9c95749c8f33.html#autotoc_md135", null ],
      [ "Exemple 1 : Système de télémétrie simple", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_205___e286b88c2636fb4118cac9c95749c8f33.html#autotoc_md137", [
        [ "Scénario", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_205___e286b88c2636fb4118cac9c95749c8f33.html#autotoc_md138", null ],
        [ "Code complet", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_205___e286b88c2636fb4118cac9c95749c8f33.html#autotoc_md139", null ]
      ] ],
      [ "Exemple 2 : Système de commande avec réponse", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_205___e286b88c2636fb4118cac9c95749c8f33.html#autotoc_md141", [
        [ "Scénario", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_205___e286b88c2636fb4118cac9c95749c8f33.html#autotoc_md142", null ],
        [ "Code complet", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_205___e286b88c2636fb4118cac9c95749c8f33.html#autotoc_md143", null ]
      ] ],
      [ "Exemple 3 : Test en boucle locale (sans matériel CAN)", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_205___e286b88c2636fb4118cac9c95749c8f33.html#autotoc_md145", [
        [ "Scénario", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_205___e286b88c2636fb4118cac9c95749c8f33.html#autotoc_md146", null ]
      ] ],
      [ "Exemple 4 : Debug / Monitoring CAN", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_205___e286b88c2636fb4118cac9c95749c8f33.html#autotoc_md148", [
        [ "Scénario", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_205___e286b88c2636fb4118cac9c95749c8f33.html#autotoc_md149", null ]
      ] ],
      [ "Checklist d'implémentation", "md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_205___e286b88c2636fb4118cac9c95749c8f33.html#autotoc_md151", null ]
    ] ],
    [ "Data Structures", "annotated.html", [
      [ "Data Structures", "annotated.html", "annotated_dup" ],
      [ "Data Structure Index", "classes.html", null ],
      [ "Data Fields", "functions.html", [
        [ "All", "functions.html", null ],
        [ "Variables", "functions_vars.html", null ]
      ] ]
    ] ],
    [ "Files", "files.html", [
      [ "File List", "files.html", "files_dup" ],
      [ "Globals", "globals.html", [
        [ "All", "globals.html", null ],
        [ "Functions", "globals_func.html", null ],
        [ "Variables", "globals_vars.html", null ],
        [ "Enumerations", "globals_enum.html", null ],
        [ "Enumerator", "globals_eval.html", null ],
        [ "Macros", "globals_defs.html", null ]
      ] ]
    ] ]
  ] ]
];

var NAVTREEINDEX =
[
"_c_a_n__utils_8c.html",
"md__c_1_2programation_2ulaval_2_g_a_u_l_2programmation_2_t_a_g_c_a_n_2guide__utilisation_204___f_i_l_t_r_a_g_e___c_a_n.html#autotoc_md124"
];

var SYNCONMSG = 'click to disable panel synchronization';
var SYNCOFFMSG = 'click to enable panel synchronization';
var LISTOFALLMEMBERS = 'List of all members';