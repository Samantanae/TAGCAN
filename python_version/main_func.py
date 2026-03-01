import tag_gestion_idea as tgi
import main_func_simu as mfs
from simu_logic import simu_NOT, simul_OR
from print_func import beau_print_mask
N_BYTES = 8

class GData:
    GT = [[""] for e in range(N_BYTES)]
    data = [0 for e in range(N_BYTES)]    # bytes

PRINT_ERROR_MODE = False
RAISE_ERROR_MODE = True

def print_all_data_bin():
    r = "tag:\t"
    for e in GData.GT:
        r += f"{e}\t"
    print(r)
    r = "DATA: \t"
    for e in GData.data:
        r += f"{beau_print_mask(e)}\t"
    print(r)

def set_tag(tag:str, size:int)->int:
    """aujouté un tags au gestionnaire de tags contenue dans GData

    Args:
        tag (str): le tags à ajouté
        size (int): la taille (en bits) de la valeur associer au tags
            peut être 1, 3, 4, 8 ou 16

    Returns:
        int: 1: réussi
            -1: pas asser d'espace
            -2: size invalide
    """
    # gérer le cas de 16 bits un peux séparément
    if size == 16:
        n_dispo = 0
        # get the nb bytes disponible for value of 8 bits
        for i in range(N_BYTES):
            if tgi.have_space_for_value(GData.GT[i][0], 8):
                n_dispo += 1
        if n_dispo < 2:
            return -1 # pas asser d'espace pour mettre le tag
        # si toujours là, il as asser d'espace
        mode_ = "a"
        for i in range(N_BYTES):
            if tgi.have_space_for_value(GData.GT[i][0], 8):
                if mode_ == "a":
                    mode_ = "b"
                    # partie a de la valeur à 16 bits
                    spe_val = f";16a_{tag}"
                    tgi.set_tag_to_spe_val(GData.GT[i], spe_val)
                elif mode_ == "b":
                    # partie b de la valeur à 16 bits
                    spe_val = f";16b_{tag}"
                    tgi.set_tag_to_spe_val(GData.GT[i], spe_val)
                    return 1    # l'ajout a été réussi
    elif size == 8:
        for i in range(N_BYTES):
            if tgi.have_space_for_value(GData.GT[i][0], 8):
                tgi.set_tag_to_spe_val(GData.GT[i], f";8_{tag}")
                return 1
        return -1 # pas asser de place 
    elif (size == 4) or (size == 3) or (size == 1):
        for i in range(N_BYTES):
            if tgi.have_space_for_value(GData.GT[i][0], size):
                tgi.add_tag(GData.GT[i], tag, size)
                return 1
        return -1 # pas asser d'espace
    return -2   # size invalide



def _valide_value(value, size_max):
    if value > size_max:
        raise ValueError(f"value max: [{size_max}] value enter: [{value}]")

def valide_value(value, size):
    """vérifier si la valeur n'ais pas trop élevé pour la taille de bits set pour cette dernière.

    Args:
        value (_type_): _description_
        size (_type_): _description_

    Returns:
        _type_: _description_
    """
    if size == 1: return _valide_value(value, 1)
    if size == 2: return _valide_value(value, 3)
    if size == 3: return _valide_value(value, 7)
    if size == 4: return _valide_value(value, 15)
    if size == 5: return _valide_value(value, 31)
    if size == 8: return _valide_value(value, 255)
    
def get_n_bytes(tag) -> int:
    """obtenir dans quel bytes (pour les valeur <= à 8 bits ) la valeurs est stoqué.

    Args:
        tag (_type_): le tag de la valeur

    Returns:
        int: l'index (entre 0 et 7) du numéro du bytes.
        renvoi -3  si le tag n'ais pas trouvé
    """
    n_byte = -1
    for i in range(N_BYTES):
        if tgi.containe_tag(GData.GT[i][0], tag):
            n_byte = i
            break
    if n_byte == -1:
        # tag non trouvé
        return -3
    return n_byte



def set_value(tag:str, value:int):
    """set the value of the tags. Only with lees than 8 bits value.
    other wise, use `set_value16`. (it's a limitation of c)

    Args:
        tag (str): the tag of the value
        value (int): the value to put there
    """
    # value: int8_t en c
    n_byte = get_n_bytes(tag)
    size = tgi.get_size_tag(GData.GT[n_byte][0],tag) 
    p0 = tgi.get_position_first_bits(GData.GT[n_byte][0], tag)
    valide_value(value, size)
    # création du mask
    n_masque = simu_NOT(make_binary_mask(p0, size))
    #print("mask:", n_masque)
    # déplacement de la valeur
    val_modif = prepa_val(value, p0, size)
    bytes_actif = GData.data[n_byte]
    #print("byte actif",beau_print_mask(bytes_actif))
    #print("val",beau_print_mask(value))
    #print("val_modif",beau_print_mask(val_modif))
    bytes_actif &= n_masque
    #print("byte après modif",beau_print_mask(bytes_actif))
    byte_new = simul_OR(bytes_actif, val_modif)
    #print("byte avec la nouvel valeur",beau_print_mask(byte_new))
    
    
    #value_temp = value
    #print("data to set: ", value, "\t", bin(value_temp))
    #value_temp >>= size
    #print("value for confirmation: ", value_temp, "\t bin: ",bin(value_temp))
    #if value_temp > 0:
    #    raise ValueError("the value have to mutch bit.")
    
    if size == 8:
        GData.data[n_byte] = value
        return 1
    else:
        GData.data[n_byte] = byte_new
    # 1) déterminer de combien déplacer la valeur
    
    # créer un masque qui fit avec la position de la valeur
    
    #n_masque = 0
    #for e in range(size):
    #    n_masque += 2**e
    #print("masque avant déplacement", bin(n_masque))
    #n_masque <<= p0 - size
    #print("masque après déplacement", bin(n_masque))
    ## inversion du masque
    #n_masque_ = format(n_masque, "b")
    #n_masque2 = ""
    #for e in n_masque_:
    #    if e == "0":
    #        n_masque2 += "1"
    #    else:
    #        n_masque2 += "0"
    #n_masque_ = n_masque_.replace("-", "")
    ## ajout des bits d'après
    #while(len(n_masque_) < 8):
    #    n_masque_ = "1" + n_masque_
    #n_masque = int(n_masque_)
    
    #value_ = prepa_val(value, p0, size)
    #print("masque (après déplacement)", beau_print_mask(n_masque))
    #print("value: ", value, "\t", beau_print_mask(value))
    #val_modif = prep_value(value, p0, size)
    ## supression temps de l'encienne val
    #print(f"data avant {beau_print_mask(value_)}\t{(GData.data[n_byte])}")
    #GData.data[n_byte] &= value
    #print(f"data sans {beau_print_mask(GData.data[n_byte])}")
    #print("new data: ", beau_print_mask(value))
    ## set the new data
    #GData.data[n_byte] = simul_OR(GData.data[n_byte], val_modif)
    #print(f"data après {beau_print_mask(GData.data[n_byte])}")
    ##TODO: AJOUTER UNE VÉRIFICATIONS DE SI LA VALEUR EST TROP GRANDE
    return 1

def set_value16(tag:str, value:int):
    index_a, index_b = get_index_ab_tag_16(tag)
    if (index_a == -1) and (index_b == -1):
        if PRINT_ERROR_MODE:
            print("the tags isen't find")
        return -1 # propagate the value.
    if index_b == - 1:
        # case of value of les than 16 bits send.
        # in C, conversion is needed
        return set_value(tag, value)
    # case 'index_a' impossible du to the logic of `get_index_ab_tag_16`
    # but juste in case of somme injection code.
    if index_a == -1:
        raise Exception("index 'a' dosen't fund.")
    n = 0
    # masque pour la séparation
    masque_8a = '0000000011111111'
    masque_8b = '1111111100000000'
    masque_8an = int(masque_8a, 2)
    masque_8bn = int(masque_8b, 2)
    
    # séparation de la data en 2 bytes
    data_a = masque_8an & value
    data_b = masque_8bn & value
    
    # moving the data_b 
    #print("data b (avant)", beau_print_mask(data_b))
    data_b >>= 8
    #print("data b(après)", beau_print_mask(data_b))
    # set the value
    GData.data[index_a] = data_a
    GData.data[index_b] = data_b

def get_index_ab_tag_16(tag:str)->tuple[int, int]:
    l0 = len(GData.GT)
    index_a = -1
    index_b = -1
    for e in range(l0):
        if tgi.containe_tag(GData.GT[e][0], tag):
            if tgi.get_size_tag(GData.GT[e][0], tag) != 16:
                # cas de valeur avec un nombre de bit inférieur à 16 demandé à cette fonciton.
                return (e, -1)
            else:
                if f";16a_{tag}" == GData.GT[e][0]:
                    if index_a != -1:
                        raise ValueError("plus d'un index est présent pour la partie `a` du tag")
                    index_a = e
                elif f";16b_{tag}" == GData.GT[e][0]:
                    if index_b != -1:
                        raise ValueError("plus d'un index est présent pour la partie `b` du tag")
                    index_b = e
                else:
                    # juste to be safe
                    raise Exception("[ERROR INCONNUE] un type inconnue de tags a été détecté")
        if (index_a != -1) and (index_b != -1):
            return index_a, index_b
    return -1, -1 # in the case of the tags not fund.
                    

def get_value16(tag:str):
    # TODO!: MAKE THIS ALSO WORK WITH MORE TINY VALUE (JUSTE EXPORT THEN IN 16 BYTES INT)
    index_a, index_b = get_index_ab_tag_16(tag)
    if (index_a == -1) and (index_b == -1):
        if PRINT_ERROR_MODE:
            print("the tags isen't find")
        return -1 # propagate the value.
    if index_b == - 1:
        # case of value of les than 16 bits send.
        # in C, conversion is needed
        return get_value(tag)
    # case 'index_a' impossible du to the logic of `get_index_ab_tag_16`
    # but juste in case of somme injection code.
    if index_a == -1:
        raise Exception("index 'a' dosen't fund.")
    
    # get the 2 bytes
    ba = GData.data[index_a]
    bb = GData.data[index_b]
    
    # make the receptacle (in 16 bits for c) 
    result = 0
    
    # moving 8 bite the 'a' value.
    #print("bb(avant): ", beau_print_mask(bb))
    bb <<= 8 
    #print("bb(après): ", beau_print_mask(bb))
    result = simul_OR(ba, result)
    result = simul_OR(bb, result)
    return result
     
#def get_i_container(tag:str) -> int:
#    """get the index of the container where the `tag` is present.
#
#    Args:
#        tag (str): the tag links with the value
#
#    Raises:
#    Exce ption: in case of call when the value linked with the tag is an 16bits value.
#    Rturns:
#        int:    -3: tags not fund
#            else: the index of the container with the tags inside.
#    """
#    n_byte = -1
#    for i in range(N_BYTES):
#        if tgi.containe_tag(GData.GT[i][0], tag):
#            n_byte = i
#            break
#    if n_byte == -1:
#        # tag non trouvé
#        return -3
#    size = tgi.get_size_tag(GData.GT[n_byte][0],tag) 
#    if size == 16:
#        raise Exception("BAD FUNCTION. use the 16 bits version.")
#    return n_byte
    
def get_value(tag:str):
    """get the value of the tags. Only with lees than 8 bits value.
    other wise, use `get_value16`. (it's a limitation of c)

    Args:
        tag (str): the tag of the value
    """
    # value: int8_t en c
    
    ##1) obtenir le numéro du bytes
    n_byte = get_n_bytes(tag)
    ## get the container and the byte for shorter line of code.
    gt = GData.GT[n_byte][0]
    
    # get the bytes where the data is stored
    data = GData.data[n_byte]
    
    # get the size of the data
    size = tgi.get_size_tag(gt, tag)
    
    if size == 8:
        return data
    
    # 1) déterminer de combien déplacer la valeur
    p0 = tgi.get_position_first_bits(gt, tag)
    # créer un masque qui fit avec la position de la valeur
    # the logic here is as folow: exemple: [<other data><'1'*size>]
    mask_data = make_binary_mask(p0, size)
    p0 = 8 - p0
    print("mask: ", beau_print_mask(mask_data))
        
    # applie the mask to the data
    
    data_temp = data & mask_data
    print("data temps: ", beau_print_mask(data_temp))
    # decaling the data of p0 - size to the right
    data_temp >>= p0 - size 
    print("data temps (après): ", beau_print_mask(data_temp))
    print("data_final: ", data_temp)
    return data_temp





def test_value16bit(val:int, tag="patapouf", verbose=False, silence_succes=True):
    r = set_value16(tag, value=val)        
    if verbose:
        print("set value result 16----------: ",r)
    r2 = get_value16("patapouf")
    if verbose:
        print_all_data_bin()
        print("get value 16")
        print("patapouf", r2)
    if r2 != val:
        raise Exception("in and out value differ")
    else:
        if not silence_succes:
            print(f"test successfull with [{val}] for 16 bits data")

def test_all_possible_value_16_bit(tag:str="patapouf", verbose=False, silence_succes_individuel=True):
    print("-------------------test val 16-------------")
    for e in range(65535):
        test_value16bit(e,tag, verbose,silence_succes_individuel)
    print("-----------------test 16 bit succes--------")

def get_nb_byte_to_send() -> int:
    """get the number of bytes to send (who many bytes have tags link withit)
    """
    r = 0
    for e in GData.GT:
        if e[0] != "":
            r += 1
    return r

def get_n_byte(n:int) -> int:
    return GData.data[n]

def receve(b0:int, b1:int, b2:int, b3:int, b4:int, b5:int, b6:int, b7:int) -> None:
    GData.data[0] = b0
    GData.data[1] = b1
    GData.data[2] = b2
    GData.data[3] = b3
    GData.data[4] = b4
    GData.data[5] = b5
    GData.data[6] = b6
    GData.data[7] = b7
    
def get_n_byte(n_bit:int) -> int:
    """get the nième bytes.

    Args:
        n_bit (int): the id of the bytes (from 0 to 7)

    Returns:
        int: the bytes in questions.
    """
    return GData.data[n_bit]


def send(l_bytes:list):
    """prepare the bytes for being send.

    Args:
        l_bytes (list): the container of the bytes linked to the STM32 CAN transiver.
    """
    while len(l_bytes) < len(GData.data):
        l_bytes.append(0)
    for e in enumerate(GData.data):
        l_bytes[e[0]] = e[1]


if __name__ == "__main__":
    # 1) create the tags
    set_tag("VN1",4)
    set_tag("MV2", 3)
    set_tag("FG5", 8)
    set_tag("DF2", 1)
    set_tag("patapouf", 16)
    
    
    #------------------ RÉCEPTION-------------
    mfs.set_tag("VN1",4)
    mfs.set_tag("MV2", 3)
    mfs.set_tag("FG5", 8)
    mfs.set_tag("DF2", 1)
    mfs.set_tag("patapouf", 16)
    #---------------- fin réglage - réception-------------    
    
    
    # normalement, il devras ressemblé à ceci: 
    # [";4_VN1;3_MV2;1_DF2","8_FG5", "8_DF2","16a_patapouf"]
    
    # set the value
    print("set value result: ",set_value("VN1", 2))
    print_all_data_bin()
    print("VN1",get_value("VN1"))
    # suposé:
    # 0010 000 0 (00100000) ; 00000000 ; 00000000 , 00000000
    print("set value result MV2: ",set_value("MV2", 2)) # erreur ici.
    print_all_data_bin()
    print("MV2",get_value("MV2"))
    # 0010 010 0 (00100100) ; 00000000 ; 00000000 , 00000000
    print("set value result FG5: ",set_value("FG5", 9))
    print_all_data_bin()
    print("FG5:",get_value("FG5"))
    # 0010 010 0 (00100100) ; 00001001 ; 00000000 , 00000000
    
    # 0010 010 0 (00100100) ; 00001001 ; 00000010 , 00011111
    print("set value result: ",set_value("DF2", 1))
    print_all_data_bin()
    print("DF2:",get_value("DF2"))
    # 0010 010 0 (00100100) ; 00000001 ; 00000010 , 00011111
    print("set value result patapouf: ",set_value16("patapouf", 46))
    print_all_data_bin()
    print("patapouf:",get_value16("patapouf"))
    
    # get value
    va = get_value("VN1")
    print("value va: ", va)
    
    make_binary_mask(0, 4)
    make_binary_mask(4, 3)
    print(prepa_val(7, 4, 3))
    
    
    print("-------------------- getvalue----------------------")
    print(get_value("MV2"))
    
    
    
    
    # ---------- essais de transmision (en simulation) ----------
    to_send_bufer = [0 for e in range(8)]
    print("bufers initial",to_send_bufer)
    byte_to_send = send(to_send_bufer)
    print("bufer à envoyer: ", to_send_bufer)
    
    # --------- transimisions CAN des bytes du bufer
    # ...
    #---------- réception
    # view before reception
    print("donné dans le can qui reçoi (avant réception)",mfs.GData2.data)
    mfs.receve(*to_send_bufer)
    # view after
    print("donné dans le can qui reçoi (après réception)",mfs.GData2.data)
    print("DF2:",mfs.get_value("DF2"))
    print("FG5:",mfs.get_value("FG5"))
    print("patapouf:",mfs.get_value16("patapouf"))
    
