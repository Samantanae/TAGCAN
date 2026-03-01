class Tag:
    l_tag = []
    def __init__(self) -> None:
        self.nom = ""
        self.n_bits = 0
        self.byte_ida = -1
        self.byte_idb = -1
        self.bit_pos = 0
        Tag.l_tag.append(self)
        