#!/usr/bin/python
import sys
from random import randint

if (len(sys.argv) != 4):
	print "\nusage python matricesGenerator M K N\n"
	quit()

M = sys.argv[1]
K = sys.argv[2]
N = sys.argv[3]

in1 = open ("in1.txt", "w")
in1.write ("LINHAS = ")
in1.write (M)
in1.write ("\nCOLUNAS = ")
in1.write (K)
in1.write ("\n")

for i in range (0, int(M)):
	for j in range (0, int(K)):
		in1.write(str(randint(-100,100)))
		if (j < int(K)-1):
			in1.write(" ")
	if (i < int(M)-1):		
		in1.write("\n")

in2 = open ("in2.txt", "w")
in2.write ("LINHAS = ")
in2.write (K)
in2.write ("\nCOLUNAS = ")
in2.write (N)
in2.write ("\n")

for i in range (0, int(K)):
	for j in range (0, int(N)):
		in2.write(str(randint(-100,100)))
		if (j < int(N)-1):
			in2.write(" ")
	if (i < int(K)-1):		
		in2.write("\n")
