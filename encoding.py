import math

def code(number):
    exp = int(math.floor(math.log2(number)))
    gamma = number - int(math.pow(2, exp))
    xd = (str(bin(gamma))[2:])
    xd = "0"*(exp - len(xd)) + xd
    coding = "0"*exp + "1" + xd
    return coding


encoded =[]

with open("test.txt", "r") as t:
    for number in t:
        encoded.append(code(int(number)))

all = ""
for elem in encoded:
    all += elem

while len(all) % 32 != 0:
    all = all + "0"

with open("generador.txt", "w") as o:
    i = 0
    while i < len(all)/32:
        o.write(all[32*(i): 32*(i+1)] + " ")
        print("list[" + str(i) + "] = 0b" + all[32*(i): 32*(i+1)] + ";")
        i = i+1

print(encoded[-1])
