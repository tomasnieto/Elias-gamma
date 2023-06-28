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

while len(all) % 16 != 0:
    all = all + "0"

i = 0
while i < len(all)/16:
    print("list[" + str(i) + "] = 0b" + all[16*(i): 16*(i+1)] + ";")
    i = i+1

print(encoded[-1])
