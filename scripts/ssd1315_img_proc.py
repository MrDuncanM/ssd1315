# This is a very simple image procesor for the SDD1315.
# You will need to use pip3 or venv to install PIL(Pillow) and possibly other prerequisites.
# The only arguments are file names and it will output a header file for each input file.
# Currently only makes 218 column x 6 page images...
# Also doesn't scale the source image down.
# Also doesn't try to deal with color, it's 1 bit per pixel input.

# P.S. Usage Example = ssd1315_img_proc.py dog.png

import os, sys
from PIL import Image

for imgFile in sys.argv[1:]:
	print("Processing: " + imgFile)

	inFileName, inFileExt = os.path.splitext(imgFile)
	outFileName = inFileName + ".h"
	outFile = open(outFileName, 'w', encoding="utf-8")

	img = Image.open(imgFile);

	outFile.write("/*\n")
	outFile.write(" * " + outFileName + "\n")
	# TODO: Time and date portion of header
	outFile.write(" */\n\n#include \"ssd1315/ssd1315_img.h\"\n\n")
	
	# Write the #define for the header
	structName = inFileName.upper()
	outFile.write("#ifndef " + structName + "_H_\n");
	outFile.write("#define " + structName + "_H_\n\n")

	# Create byte array header file from upper left 128x52 pixels.
	hexMap = [hex(64)]
	for y in range(6): # 6 bytes so 48 pixels
		for x in range(128):
			byte = 0
			for pixel in range(8):
				# bitmask each column of 8 pixels into a byte
				pixelPos = (x, (y * 8) + pixel)
				pixelVal = img.getpixel(pixelPos)
				if pixelVal == 1:
					byte |= 1 << pixel
			hexMap.append(hex(byte))
			
	# 768 bytes of pixels plus the start byte
	outFile.write("uint8_t " + inFileName.upper() + "_BUF[" + str(len(hexMap)) + "] = {")
	outFile.write(", ".join(map(str, hexMap)))
	outFile.write("};\n\n")
	
	outFile.write("SSD1315_IMG " + inFileName.upper() + " = {\n");
	outFile.write("\t128,\n");
	outFile.write("\t6,\n");
	outFile.write("\t" + inFileName.upper() + "_BUF\n");
	outFile.write("};\n\n")
	
	outFile.write("#endif")

	outFile.close()
