"""
    this file is juste the proof of concept for the gestion of the tags.    
    it's contain the logic for modif of the gt (gestionnaire de tag).
    The 'main_func.py' containe the actual container of data.
    The 'main_func_simu.py' is used to simulate the seconde STM32 (the recever)
    It's suposed to be executed from 'main_func.py'
    
    NB: list[str] apparais si la chaine de caractère serras ultimement modifier.
    NB2: tout les GT ont la même tailles (en C),
    affin d'évité des problèmes d'insertion dans une chaine de caractère qui manque d'espace.
    
    
"""



# when the param is `gt:list[str]` it's possibly modif the value of the gt
# when the param is `gt`, it's only read witout writing on it.

    
#
def get_nb_tag(gt:str):
    """VERSION C: get_nb_tag(char* tg1)
    obtien le nombre de tags présent dans le gestionnaire de tags.
    Args:
        gt (str): _description_

    Returns:
        _type_: _description_
    """
    r = gt.count(";")
    if r == 0:
        return -1 # juste pour correspondre au C
    return r


def have_space_for_value(gt:str, size:int) -> bool:
    """verif if the gt can containe the value. in the case of 16 bits, you need 2 gt.

    Args:
        gt (str): the gestionnaire of tags
        size (int): the size of the value (1, 3, 4, 8 or 16)

    Raises:
        Exception: in the case of size not in the permited possible value

    Returns:
        bool: if the gt can actualy contain the value or not. 

    """
    if len(gt) == 0:
        return True
    elif size == 4:
        if gt.endswith(";4_") or (gt.count(";4_;") > 0):
            return True
        return False
    elif size == 3:
        if gt.endswith(";3_") or (gt.count(";3_;") > 0):
            return True
        elif gt.endswith(";4_") or (gt.count(";4_;") > 0):
            return True
        return False
    elif size == 1:
        if gt.endswith(";1_") or (gt.count(";1_;") > 0):
            return True
        elif gt.endswith(";4_") or (gt.count(";4_;") > 0):
            return True
        return False
    elif size == 8:
        if len(gt) > 0:
            return False
        else:
            return True
    raise Exception("size invalide")


# verif_gt_contain_tag
def _verif_contain(e:str, tag_name:str, r:str) -> bool:
    """C'est une simple sous-fonction de vérification.
    En C, elle retourne 1 pour True, 0 pour False et un négatif en cas d'erreur.
    le négatif est le code d'erreur.

    Args:
        e (str): _description_
        tag_name (str): _description_
        r (str): _description_

    Returns:
        bool: _description_
    """
    return (e.startswith(r) and e == r + tag_name)

# size_tag              encore mieu dans le sense qu'il prend direct un gestionnaire de tags
def get_size_one_tag(tag:str):
    """Obtien la taille alouer au tag passé en argument. (permet la convertion simple de tag vers int)

    Args:
        tag (str): un tag, sans ces séparateur.

    Returns:
        int: la taille de la donnée rataché à celui-ci.
    """
    if tag.startswith("1_"):
        return 1
    elif tag.startswith("3_"):
        return 3
    elif tag.startswith("4_"):
        return 4
    elif tag.startswith("8_"):
        return 8
    elif tag.startswith("16a_") or tag.startswith("16b_"):
        return 16
    else:
        return -1

def set_tag_to_spe_val(gt:list[str],val):
    """permet de set facillement les tags de 8 bites et de 16 bites.

    Args:
        gt (list[str]): _description_
        val (_type_): _description_
    """
    gt[0] = val
    
# size_tag
def get_size_tag(gt:str, tag_name:str)->int:
    """version évoluer du précédent chercheur de taille de tags.
    il permet de récupérer la taille dédier au tag sans même avoir à tout le temps spécifier la taille de celui-ci.
    

    Args:
        gt (str): gestionnaire de tags
        tag_name (str): le nom du tag (juste le nom. pas de séparateur, pas de 'x_' avant.)

    Returns:
        int: la taille qui y est rataché.
    """
    tlp = gt.split(";")[1:]
    for e in tlp:
        if _verif_contain(e, tag_name, "1_"):
            return 1
        elif _verif_contain(e, tag_name, "3_"):
            return 3
        elif _verif_contain(e, tag_name, "4_"):
            return 4
        elif _verif_contain(e, tag_name, "8_"):
            return 8
        elif _verif_contain(e, tag_name, "16a_") or _verif_contain(e, tag_name, "16b_"):
            return 16
    return -1 


def split_4to31(gt:list[str]):# TODO: IMPLEMENT THIS.
    if _endwith_size_tag_noname(gt[0], 4):
        gt[0].replace(";4_", f";3_;1_")
    elif _tag_noname_inside(gt[0], 4):
        gt[0].replace(";4_;", f";3_;1_;", 1)
    else:
        raise ValueError("dosen't have 4 bits space left")

def subtags_end(gt:list[str], size:int, tag_name):
    """use to replace an unamed tags when it's at the end of the gt. other wise, it's could also replace other tag and produce somme bugs.
    also work for inside tags.
    Args:
        gt (list[str]): _description_
        size (int): _description_
        tag_name (str): _description_
    """
    second_size = 1
    if size == 3:
        second_size = 1
    elif size == 1:
        second_size = 3
    elif size == 4:
        second_size = 4
    if len(str(size)) == 1:
        if gt[0].endswith(";4_") and size != 4:
            gt[0] = gt[0][:-2] + f"{size}_{tag_name};{second_size}_"
        elif (f";4_;" in gt[0]) and size < 4:            
            gt[0].replace(f";4_;", f";{size}_{tag_name};{second_size}_;")


def put_end_subtags(gt:list[str], size:int, tag_name:str):
    """use to replace an unamed tags when it's at the end of the gt. other wise, it's could also replace other tag and produce somme bugs.
    also work for inside tags.
    Args:
        gt (list[str]): _description_
        size (int): _description_
        tag_name (str): _description_
    """
    
    if len(str(size)) == 1:
                
        if f";{size}_;" in gt[0]:
            gt[0].replace(f";{size}_;", f";{size}_{tag_name};")
        elif gt[0].endswith(f";{size}_"):
            gt[0] = gt[0][:-2] + f"{size}_{tag_name}"
        # verif if the size is a subdivision
        
        
    elif len(str(size)) == 2:
        # cas de valeurs à plus de 9 bits.
        raise ValueError("this function is not implement for now and is possibly an value error.")
    
# verif_gt_contain_tag
def containe_tag(gt:str, tag_name:str)->bool:
    """vérifie si le gt contien le tag.

    Args:
        gt (str): gestionnaire de tag
        tag_name (str): nom du tag

    Returns:
        bool: vrai s'il contien. faux sinon.
    """
    r = get_size_tag(gt, tag_name)
    if r == -1:
        return False
    else:
        return True
        
def _endwith_size_tag_noname(gt:str, size:int) -> bool:
    """verifie if the gt end with a free space of the size requested.
        no modif make to 'gt'
    Args:
        gt (str): tag 
        size (int): the size (in bits) for the value
    """
    if size == 16:
        return False #raise ValueError("a bytes can't contain more than 8 bits.")
    return gt.endswith(f";{size}")

def _tag_noname_inside(gt:str, size: int):
    if size == 16:
        return False
    return gt.count(f";{size}_;") > 0








def add_tag(gt:list[str],tag_name:str, size:int):    # list[str] is to permit modif to the string to desen't need to necerely return.
    """add a tag to a gestionnaire of tags
    Celui-ci ajoute un tag uniquement à l'intérieur du gt d'un bytes. pas des bytes.
    (utilisé à l'interne.)

    Args:
        gt (list[str]): _description_
        tag_name (str): _description_
        size (int): _description_

    Raises:
        Exception: _description_
        Exception: _description_
        Exception: _description_
    """
    if len(gt) == 0:
        gt.append("") # juste pour corespondre au c. (pas implémenté dedans)
        
    if size == 8:
        if gt[0].count(";") != 0:
            raise Exception("bytes can't containe more tan 8 bytes")
        gt[0] = f";8_{tag_name}"
        
    if gt[0].count(";") == 0:
        if size == 4:
            gt[0] = f";4_{tag_name};4_"
        elif size == 3:
            gt[0] = f";3_{tag_name};1_;4_"
        elif size == 1:
            gt[0] = f";1_{tag_name};3_;4_"
    else:
        if size == 4:
            if gt[0].count(";4_;") > 0:
                gt[0].replace(";4_;", f";4_{tag_name};", 1)
            elif gt[0].endswith(";4_") > 0:
                put_end_subtags(gt, size, tag_name)
            else:
                raise Exception("no more space in this gt for 4 bites value")
        elif size == 3:
            # case when the exact space needed is free
            if gt[0].count(";3_;") > 0:   # case of space uname not at the end of the gt.
                gt[0].replace(";3_;",f";3_{tag_name};",1)
            elif gt[0].endswith(";3_"):
                put_end_subtags(gt, size, tag_name)                
            # case of space of 4 bits dispo.
            elif gt[0].count(";4_;") > 0:
                gt[0].replace(";4_;", f";3_{tag_name};1_;", 1)
            elif gt[0].endswith(";4_"):
                subtags_end(gt, 3, tag_name)
                print(gt)
            else:
                raise Exception("no more space in this gt for 4 bites value")
        elif size == 1:
            
            if gt[0].count(";1_;") >= 1:
                gt[0].replace(";1_;", f";1_{tag_name};", 1)
            elif gt[0].endswith(";1_"):
                put_end_subtags(gt, size, tag_name)
            elif gt[0].count(";4_;") > 0:
                subtags_end(gt, 1, tag_name)

# get_position_first_bit_tag
def get_position_first_bits(gt:str, tag:str):
    """obtien la possition du premier bits du tags.

    Args:
        gt (str): _description_
        tag (str): _description_

    Returns:
        _type_: _description_
    """
    r = get_size_tag(gt, tag)
    if r == -1:
        return -1
    tag_ = tag # just for make no modif to the tag name (only needed in c)
    n = 0
    posi_bit = 0
    temp = gt.split(";")[1:]
    if (r == 16) or (r == 8):
        return 0
    for e in temp:
        if e  == f"{r}_{tag}":
            return posi_bit
        posi_bit += get_size_one_tag(e)
    return -1



a = 4 & 8
print(a)


a = b'0B10101110'
bin(121)


    
