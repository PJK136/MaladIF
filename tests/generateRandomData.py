#!/usr/bin/python3

import sys
import string
import random

booleans = [True, False]
dnames = open('diseases.txt').read().splitlines()
minInt = -1000
maxInt = 1000
dispersion = 5./100 #5%
sigmaInt = ((maxInt-minInt+1)*dispersion)/2/2 #95% des empreintes sont autour de `dispersion`% de la moyenne
minDouble = -10000
maxDouble = 10000
sigmaDouble = ((maxDouble-minDouble)*dispersion)/2/2 #95% des empreintes sont autour de `dispersion`% de la moyenne
minStr = 5
maxStr = 20

filename = sys.argv[1]
header = ""
types = []
with open(filename) as f:
    f.readline()
    for line in f:
        h, t = line.strip().split(';')
        header += h + ';'
        types.append(t)

N = int(sys.argv[2])
D = int(sys.argv[3])

if D-1 < 245:
    diseases = random.sample(dnames, D-1)
else:
    diseases = []
    for i in range(D-1):
        diseases.append("")
        for j in range(random.randint(5, 20)):
            diseases[-1] += random.choice(string.ascii_letters)

diseases.append("")
dAttributes = []
for d in range(D):
    dAttributes.append([])
    for j, t in enumerate(types):
        m = None
        if t == "ID":
            m = d
        elif t == "boolean":
            m = random.choice([True, False])
        elif t == "int":
            m = random.randint(minInt, maxInt)
        elif t == "double":
            m = random.uniform(minDouble, maxDouble)
        elif t == "string":
            m = ""
            for k in range(random.randint(minStr, maxStr)):
                m += random.choice(string.ascii_letters + string.digits)

        dAttributes[-1].append(m)

print(header+"Disease")

for i in range(1, N+1):
    d = random.randint(0, D-1)
    for j, t in enumerate(types):
        ret = ""
        if t == "ID":
            ret = i
        elif random.randint(0,3*len(types)) != 0: #1/3 des empreintes ont au moins un champ vide
            if t == "boolean":
                ret = True if random.gauss(int(dAttributes[d][j]),0.3) >= 0.5 else False
            elif t == "int":
                ret = int(random.gauss(dAttributes[d][j], sigmaInt))
            elif t == "double":
                ret = random.gauss(dAttributes[d][j], sigmaDouble)
            elif t == "string":
                k = random.randint(0, len(dAttributes[d][j])-1)
                ret = list(dAttributes[d][j])
                ret[k] = chr(ord(ret[k]) + (1 if random.random()<dispersion else 0))
                ret = "".join(ret)
        print(ret, end=';')
    print(diseases[d])

