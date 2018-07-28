#!/usr/bin/python
# (c) 2018 Affonso Amendola
# Released under GPLv3 or later
#
# ARGUMENTS:
# python ppm_parser.py [filein.ppm] [start] [end] [fileout.h]
#
#
#  filein:  expected to be a .ppm file, without comments
#   start:  start index of allocated pallette space
#     end:  end index of allocated pallette space
# fileout:  output filename without extension
#           will generate a C header file with graphics and tile data

import sys

def read_uint8(currentLine, posInLine, separator, returnEndPosition):
	out = 0
	done = False
	middleOfNumber = False
	j = posInLine

	while(not(done)):
		currentChar = currentLine[j]
		if((currentChar != '0' or middleOfNumber == True) and currentChar != separator):
			out = out*10 + (int(currentChar))
			middleOfNumber = True	
		elif(currentChar == '0' or currentChar == separator):
			done = True
		j = j+1

	if(returnEndPosition == True):
		return out, j
	else:
		return out

filename = sys.argv[1]
startIndex = int(sys.argv[2])
endIndex = int(sys.argv[3])
fileout = sys.argv[4]

f = open(filename,"r")
fout = open(fileout + ".h", "w")

filetype = f.readline()[1]

if(filetype != '3'):
	print("UNKNOWN FORMAT")
	exit()

currentLine = f.readline()

sizeX = 0
sizeY = 0
i = 0

middleOfNumber = False
done = False

while(not(done)):
	currentChar = currentLine[i]

	if((currentChar != '0' or middleOfNumber == True) and currentChar != ' '):
		sizeX = sizeX*10 + (int(currentChar))
		middleOfNumber = True	
	elif(currentChar == ' '):
		done = True
	i = i+1

middleOfNumber = False
done = False
while(not(done)):
	currentChar = currentLine[i]

	if((currentChar != '0' or middleOfNumber == True) and currentChar != '\n'):
		sizeY = sizeY*10 + (int(currentChar))
		middleOfNumber = True	
	elif(currentChar == '\n'):
		done = True
	i = i+1

f.readline()

r = 0
g = 0
b = 0

i = 0
j = 0

middleOfNumber = False
done = False

currentIndex = startIndex;

pallette = []
indexes = []
gfx=[]
for i in range(sizeX*sizeY):

	currentLine = f.readline()
	r = read_uint8(currentLine, 0, '\n', False)

	currentLine = f.readline()
	g = read_uint8(currentLine, 0, '\n', False)

	currentLine = f.readline()
	b = read_uint8(currentLine, 0, '\n', False)
	
	color  =  ((b >> 5) & 0x7) << 9
	color |= (((g >> 5) & 0x7) << 5)
	color |= (((r >> 5) & 0x7) << 1)

	if(not(color in pallette)):
		pallette.append(color)
		indexes.append(currentIndex)
		currentIndex += 1
		if(currentIndex > endIndex):
			print("TO MANY COLORS TO FIT THAT ALLOCATED PALLETTE SPACE")
			exit()
	gfx.append(indexes[pallette.index(color)])

c = len(pallette)
while c < 16:
	pallette.append(0)
	c += 1

fout.write(
'''const u16 palette[16] =
{\n\t''' + ",\n\t".join(map("0x{:04X}".format, pallette)) + "\n};\n\n")

tiles = [
  ["player",       0, 0],
  ["arrow",        1, 0],
  ["floor_closed", 2, 0],

  ["border_top_left",  0, 1],
  ["border_top",       1, 1],
  ["border_top_right", 2, 1],

  ["border_left",  0, 2],
  ["floor_open",   1, 2],
  ["border_right", 2, 2],

  ["border_bottom_left",  0, 3],
  ["border_bottom",       1, 3],
  ["border_bottom_right", 2, 3]
]

for tile_name, i, j in tiles:
	tile_data = []
	for y in range(8):
		value = 0
		for x in range(8):
			value = (value << 4) | gfx[24*(j*8 + y) + (i*8 + x)]
		tile_data.append(value)

	fout.write("const u16 tile_" + tile_name + "[8] =\n{\n\t" + \
	           ",\n\t".join(map("0x{:08X}".format, tile_data)) + "\n};\n\n")
