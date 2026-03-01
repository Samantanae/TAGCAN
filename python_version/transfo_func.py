


def prep_value(value:int, start_bit:int, size:int, verbose=False):
    """ DESUAIS
    move the value to the coresponding number of slot

    Args:
        value (int): the value to put in the CAN
        start_bit (int): the number of the bit (0 = 1xxxxxxx ?, 8 = xxxx xxx1 ? ) TODO: à vérif
    """
    if verbose:
        print("value(befor): ", value, beau_print_mask(value))
    if size == 8:
        return value
    value << (8 - size - start_bit)
    if verbose:
        print("value(after): ", value, beau_print_mask(value))
    return value

def make_binary_mask(start_bit:int, number_of_bit:int)->int:   
    val = int("1" * number_of_bit, 2)
    return prepa_val(val, start_bit, number_of_bit)
    
def prepa_val(val, start_bit, size):
    # decalage du masque pour aller au niveau du tags
    decale_val = 8 - size - start_bit
    val <<= decale_val
    # par defaux, le reste (en python) est remplie avec des '0' lors de la comparaison avec 2 valeurs, affin que ceux-ci corresponde en taille.
    # et en c, comme de base 'int8_t' est choisi, pas besoin d'ajouté des '0' à gauche du mask.
    return val