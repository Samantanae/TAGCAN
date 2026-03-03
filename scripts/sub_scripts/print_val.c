#include "../../include/sub_include/print_val.h"
#include "../../include/sub_include/tag_manager.h"
#include "../../include/sub_include/data_container.h"
#include <stdio.h>
#include <stdint.h>

/**
 * @bref prints the binary representation of an 8-bit unsigned integer.
 *
 * @param val The `val` [from 0 to 255]. The function prints the binary
 * representation of this value by iterating over each bit.
 */
void print_bin_8(uint8_t val)
{
    for(int i = 7; i >= 0 ; i--)
    {
        printf("%c", (val & (1 << i)) ? '1' : '0');
    }
}

/**
 * prints the binary representation of each byte in the `TxData`
 * array.
 */
void print_all_data_bin(void)
{
    printf("DATA: \t");
    for (int i = 0; i < N_BYTES; i++)
    {
        print_bin_8(TxData[i]);
        printf("\t");
    }
    printf("\n");
}



