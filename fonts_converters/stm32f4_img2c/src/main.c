/*
 * File: main.c
 * Author: C. Alvarado
 * Date: Fri May 8, 8:39 PM
 */
#include "headercomment.h"
#include "bmpfileformat.h"
#include <stdio.h>
#include <string.h>

struct BMP_FILE_HEADER_STRUCT 	BMP_FILE;
struct BMP_DIB_HEADER_STRUCT 	BMP_DIB;

void extract_img_name(char const *img_name, char *name) {
	size_t count = strlen(img_name);
	int offset, len;
	offset = len = 0;

	for (int i = count; i >= 0; --i) {
		if (img_name[i] == '.')
			len = i;

		if(img_name[i] == '/') {
			offset = i + 1;
			break;
		}
	}
	if(*(img_name + offset) >= '0' && *(img_name + offset) <= '9') { // number
		memcpy(name + 1, img_name + offset, len - offset + 1);
		name[0] = '_';
		name[len - offset + 1] = '\0';
	} else {
		memcpy(name, img_name + offset, len - offset);
		name[len - offset] = '\0';
	}
}

void generate_header_file(const char *h_name, const char *img_name, unsigned size) {
	FILE *generated_file = fopen(h_name,"w");
	fprintf(generated_file, "%s\n", head_comments);
	fprintf(generated_file, "%s _%s_H_\n", "#ifndef", img_name);
	fprintf(generated_file, "%s _%s_H_\n\n", "#define", img_name);
	fprintf(generated_file, "extern const unsigned char %s[%u];\n", img_name, size);
	fprintf(generated_file, "\n%s\n", "#endif");
	fprintf(generated_file, "%s\n", tail_comments);
	fclose(generated_file);
}

void fprintf_bin_dimilitedBycommas(FILE *file, const void *data, unsigned length) {
	static unsigned char word = 0;
	unsigned char *data2 = (unsigned char *)data; 

	for (int i = 0; i < length; ++i) {
		if (word == 16) {
			fprintf(file, "\n");
			word = 0;
		}
		fprintf(file, "0x%02X,", data2[i]);
		word++;
	}
}


unsigned generated_c_file(const char *c_name, const char *img_name, const char *in_img) {
	FILE *generated_file = fopen(c_name,"w");
	FILE *in_bin = fopen(in_img, "rb");
	unsigned BMP_DIB_REAL_LEN = 0;
	unsigned char avoidPadding = 0;
	unsigned currentReadData = 0;
	size_t total_data_read;
	size_t buffer_len;
	unsigned short buffer[512];
	// File header
	fread(&BMP_FILE, 1, BMP_FILE_HEADER_LEN, in_bin); // bit map file header

	// DIP header length
	fread(&BMP_DIB_REAL_LEN, 1, BMP_DIB_HEADER_SIZE, in_bin); // bit map file header

	// Read DIP header
	fread(&BMP_DIB, 1, BMP_DIB_REAL_LEN - BMP_DIB_HEADER_SIZE, in_bin); // bit map file header

	if(BMP_DIB_REAL_LEN + BMP_DIB_HEADER_SIZE + BMP_FILE_HEADER_LEN > BMP_FILE.offset) {
		// TODO: read dummy headers
	}

	// Re-writing the header to suit stm32f4 requirements
	BMP_FILE.file_size = BMP_DIB.width * BMP_DIB.height * 2 + BMP_FILE_HEADER_LEN + BMP_DIB_HEADER_LEN_OUT;
	BMP_FILE.offset = BMP_FILE_HEADER_LEN + BMP_DIB_HEADER_LEN_OUT;
	BMP_DIB_REAL_LEN = BMP_DIB_HEADER_LEN_OUT;
	BMP_DIB.horizontal_resolution = 0;
	BMP_DIB.vertical_resolution = 0;
	BMP_DIB.color_palette = 0;
	BMP_DIB.important_colors = 0;
	// Some files have padding bytes, Stm32f4 doesnt need them
	if (BMP_DIB.width * BMP_DIB.height * 2 != BMP_DIB.raw_size) {
		avoidPadding = 1;
		BMP_DIB.raw_size = BMP_DIB.width * BMP_DIB.height * 2;
	}

	fprintf(generated_file, "%s\n", head_comments);
	fprintf(generated_file, "const unsigned char %s[%u] = {\n", img_name, BMP_FILE.file_size);

	// fprintf_bin_dimilitedBycommas(generated_file, &BMP_FILE, BMP_FILE_HEADER_LEN);
	// fprintf_bin_dimilitedBycommas(generated_file, &BMP_DIB_REAL_LEN, BMP_DIB_HEADER_SIZE);
	// fprintf_bin_dimilitedBycommas(generated_file, &BMP_DIB, BMP_DIB_HEADER_LEN_OUT - BMP_DIB_HEADER_SIZE);

	if (!avoidPadding) {
		while(currentReadData < BMP_DIB.raw_size) {
			size_t readData = fread(buffer, 1, 1024, in_bin); // bit map file header
			currentReadData += readData;
			fprintf_bin_dimilitedBycommas(generated_file, buffer, readData);
		}
	} else if ((BMP_DIB.width + 1)*2 < 1024) {
		while(currentReadData < BMP_DIB.raw_size) {
			size_t readData = fread(buffer, 1, (BMP_DIB.width + 1)*2, in_bin); // bit map file header
			
			if (readData == (BMP_DIB.width + 1)*2) 
				readData -= 2;

			currentReadData += readData;
			fprintf_bin_dimilitedBycommas(generated_file, buffer, readData);
		}
	} else {
		fprintf(stderr, "[ERROR: ] Image is too large, max width is 511px\n");
		fprintf(generated_file, "\n#error image incomplete due to large width\n");
	}
	fprintf(generated_file, "};\n");
	fprintf(generated_file, "\n%s\n", tail_comments);
	fclose(generated_file);
	fclose(in_bin);
	return BMP_FILE.file_size;
}

int main(int argc, char const *argv[])
{
	char img_name[100];
	char h_name[100];
	char c_name[100];

	extract_img_name(argv[1], img_name);
	sprintf(h_name, "%s/%s%s", argv[3], img_name, ".h");
	sprintf(c_name, "%s/%s%s", argv[3], img_name, ".c");

	unsigned size = generated_c_file(c_name, img_name, argv[2]);
	generate_header_file(h_name, img_name, size);

	return 0;
}
