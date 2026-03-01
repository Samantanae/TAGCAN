def beau_print_mask(mask)->str:
    if isinstance(mask, int):
        mask = bin(mask)[2:]
    while len(mask) < 8:
        mask = "0" + mask
    return mask

