
/********************************************************************
*
* Writing a DAT archive file
*
* This example creates a new DAT archive containing two files
* The first file is compressed and the second is uncompressed
*
********************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include "../../../include/FuryUtils.h"

void writing_a_dat_file() {
	
	FILE *inputFile1, *inputFile2, *outputFile;
	unsigned int inputSize1, inputSize2, outputSize;
	unsigned char *inputBuffer1, *inputBuffer2, *outputBuffer;
	dat_p dat;

	/* Load files to be archived into memory */
	inputFile1 = fopen("../../../testassets/pal8.bmp", "rb");
	fseek(inputFile1, 0, SEEK_END);
	inputSize1 = ftell(inputFile1);
	fseek(inputFile1, 0, SEEK_SET);
	inputBuffer1 = (unsigned char *)malloc(inputSize1);
	fread(inputBuffer1, sizeof(unsigned char), inputSize1, inputFile1);
	fclose(inputFile1);

	inputFile2 = fopen("../../../testassets/tooshort.bmp", "rb");
	fseek(inputFile2, 0, SEEK_END);
	inputSize2 = ftell(inputFile2);
	fseek(inputFile2, 0, SEEK_CUR);
	inputBuffer2 = (unsigned char *)malloc(inputSize2);
	fread(inputBuffer2, sizeof(unsigned char), inputSize2, inputFile2);
	fclose(inputFile2);

	/* Create a new empty DAT file */
	dat = Dat_createNew();

	/* Add the files to the DAT file */
	Dat_add(dat, "PAL8.BMP", inputBuffer1, inputSize1, 1);
	Dat_add(dat, "Short.bmp", inputBuffer2, inputSize2, 0);

	/* This memory can be released now */
	free(inputBuffer2);
	free(inputBuffer1);

	/* Get the archived file into memory */
	outputSize = Dat_size(dat);
	outputBuffer = (unsigned char *)malloc(outputSize);
	Dat_buffer(dat, outputBuffer, outputSize);

	/* Destroy the DAT file */
	Dat_destroy(dat);

	/* Write the archive to disk */
	outputFile = fopen("writing_a_dat_file.EXAMPLE", "wb");
	fwrite(outputBuffer, sizeof(unsigned char), outputSize, outputFile);
	fclose(outputFile);
	free(outputBuffer);
}