#include <stdio.h>

#include "../include/in_tag_fonc.h"
#include "../include/config_value.h"

#include "tests.h"
#include "test_utils.h"

void test_tag_func() {
    printf("=== Testing in_tag_fonc.h ===\n\n");
    const char* gt_vide = "";   // cas gt vide
    const char* test_gt2 = ";4_maval;4_";    // cas 4 bits restants
    const char* test_gt3 = ";4_;4_maval";    // cas bizard (peux probable) 4 bit restant avant.
    const char* test_gt4 = ";4_maval;3_;1_"; // cas de subdivisions forcé. (pas l'espace pour 4 bit, espace pour 1 de 3 bits et 1 de 1 bits)
    const char* test_gt5 = ";4_maval;3_ce;1_";    // cas de 1 bit restant seulement
    const char* tc1_gt = ";4_maval;1_;3_ab"; // cas bizard de 1 bits restant
    const char* gt_p1 = ";4_maval;1_plein;3_ab";    // cas de gt plein
    const char* gt_p2 = ";4_maval;4_va";   // cas de gt plein (cas 2)
    const char* gt_p3 = ";1_va;3_vae;3_vt;1_vy"; // cas de gt plein (cas 3)
    // ===== have_space_for_value tests =====
    printf("\ttest de gt vide (cas d'insertion de valeurs de x bits)\n");
    TEST_EQ_INT("\t\t8 bits", have_space_for_value(gt_vide,8),1);
    TEST_EQ_INT("\t\t4 bits", have_space_for_value(gt_vide,4),1);
    TEST_EQ_INT("\t\t3 bits", have_space_for_value(gt_vide,3),1);
    TEST_EQ_INT("\t\t1 bits", have_space_for_value(gt_vide,1),1);
    printf("\ttest de gt plein (cas 1) (espace dispo de valeurs de x bits\n");
    TEST_EQ_INT("\t\t8 bits",have_space_for_value(gt_p1,8), 0);
    TEST_EQ_INT("\t\t4 bits",have_space_for_value(gt_p1,4), 0);
    TEST_EQ_INT("\t\t3 bits",have_space_for_value(gt_p1,3), 0);
    TEST_EQ_INT("\t\t1 bits",have_space_for_value(gt_p1,1), 0);

    printf("\ttest de gt plein (cas 2) (espace dispo de valeurs de x bits\n");
    TEST_EQ_INT("\t\t8 bits",have_space_for_value(gt_p2,8),0);
    TEST_EQ_INT("\t\t4 bits",have_space_for_value(gt_p2,4), 0);
    TEST_EQ_INT("\t\t3 bits",have_space_for_value(gt_p2,3), 0);
    TEST_EQ_INT("\t\t1 bits",have_space_for_value(gt_p2,1), 0);
    printf("\ttest de gt plein (cas 3) (espace dispo de valeurs de x bits\n");
    TEST_EQ_INT("\t\t8 bits",have_space_for_value(gt_p3,8), 0);
    TEST_EQ_INT("\t\t4 bits",have_space_for_value(gt_p3,4), 0);
    TEST_EQ_INT("\t\t3 bits",have_space_for_value(gt_p3,3), 0);
    TEST_EQ_INT("\t\t1 bits",have_space_for_value(gt_p3,1), 0);
    printf("\ttest cas de 4 bits restant \n");
    TEST_EQ_INT("\t\t8 bits",have_space_for_value(test_gt2,8), 0);
    TEST_EQ_INT("\t\t4 bits",have_space_for_value(test_gt2,4), 1);
    TEST_EQ_INT("\t\t3 bits",have_space_for_value(test_gt2,3), 1);
    TEST_EQ_INT("\t\t1 bits",have_space_for_value(test_gt2,1), 1);
    printf("\ttest cas de 3 bits et 1 bits restant \n");
    TEST_EQ_INT("\t\t8 bits",have_space_for_value(test_gt4,8), 0);
    TEST_EQ_INT("\t\t4 bits",have_space_for_value(test_gt4,4), 0);
    TEST_EQ_INT("\t\t3 bits",have_space_for_value(test_gt4,3), 1);
    TEST_EQ_INT("\t\t1 bits",have_space_for_value(test_gt4,1), 1);
    printf("\ttest cas de 1 bits restant \n");
    TEST_EQ_INT("\t\t8 bits",have_space_for_value(test_gt5,8), 0);
    TEST_EQ_INT("\t\t4 bits",have_space_for_value(test_gt5,4), 0);
    TEST_EQ_INT("\t\t3 bits",have_space_for_value(test_gt5,3), 0);
    TEST_EQ_INT("\t\t1 bits",have_space_for_value(test_gt5,1), 1);

    // ===== TEST get_size_one_tag ====
    printf("test get_size_one_tag\n");
    TEST_EQ_INT("\t valeurs de 3 bits",get_size_one_tag("3_of9"),3);
    TEST_EQ_INT("\t valeurs de 1 bits",get_size_one_tag("1_ogf449"),1);
    TEST_EQ_INT("\t valeurs de 4 bits",get_size_one_tag("4_o5f9"),4);
    TEST_EQ_INT("\t valeurs de 8 bits",get_size_one_tag("8_of9"),8);
    TEST_EQ_INT("\t valeurs de 16 bits (a)",get_size_one_tag("16a_oft529"),16);
    TEST_EQ_INT("\t valeurs de 16 bits (b)",get_size_one_tag("16b_ot5f9"),16);
    
    char tg1[300] = "";
    TEST_EQ_INT("test split_8to4", split_8to4(tg1), 1);
    
    printf("test set_tag_to_spe_val\n");
    char val1[200] = "";
    char val2[200] = ";8_b";
    //TEST_EQ_INT("\t '' to '8_b'",,)
    // ===== SUMMARY =====
    printf("=== SUMMARY ===\n");
    printf("Passed: %d/%d\n", passed_tests, total_tests);
    printf("Failed: %d/%d\n", total_tests - passed_tests, total_tests);

    if (passed_tests == total_tests) {
        printf("\n All tests passed!\n");
    } else {
        printf("\n Some tests failed!\n");
    }
}
