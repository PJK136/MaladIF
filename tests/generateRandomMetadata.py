#!/usr/bin/python3

import sys
import string
import random

N = int(sys.argv[1])
types = ["string", "boolean", "int", "double"]

print("AttributeName;AttributeType")
print("NoID;ID")

for i in range(1, N+1):
    print(random.choice(string.ascii_uppercase)+str(i), random.choice(types), sep=";")
