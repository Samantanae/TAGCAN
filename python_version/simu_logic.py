def simul_OR(val1, val2) -> int:
    if isinstance(val1, int):
        val1 = format(val1, 'b')
    if isinstance(val2, int):
        val2 = format(val2, 'b')
    if len(val2) > len(val1):
        val1, val2 = val2, val1
    while len(val1) > len(val2):
        val2 = "0" + val2
    #print(f"val1:{val1} ; val2:{val2}")
    r = ""
    lv = len(val1)
    for i in range(lv):
        if (val1[i] == "1") or (val2[i] == "1"):
            r += "1"
        else:
            r += "0"
    return int(r, 2)

def simu_NOT(val) -> int:
    if isinstance(val, int):
        val = bin(val)[2:]
    r = ""
    for e in val:
        if e == "0":
            r += "1"
        elif e == "1":
            r += "0"
        else:
            raise ValueError("not a binairy val")
    while len(r) < 8:
        r = "1" + r
    return int(r, 2)