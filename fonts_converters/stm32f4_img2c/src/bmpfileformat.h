/*
 * File: bmpfileformat.h
 * Author: C. Alvarado
 * Date: Fri May 8, 8:39 PM
 * Reference: https://en.wikipedia.org/wiki/BMP_file_format
 */

#ifndef BMP_FILE_FORMAT_H
#define BMP_FILE_FORMAT_H 

#define BMP_FILE_HEADER_LEN 14
#define BMP_DIB_HEADER_LEN_OUT 40
#define BMP_DIB_HEADER_LENMAX 49
#define BMP_DIB_HEADER_SIZE 4

struct BMP_FILE_HEADER_STRUCT
{
	unsigned short file_identifier;	// 2 bytes
	unsigned file_size;				// 4 bytes
	unsigned file_app;				// 4 bytes
	unsigned offset;				// 4 bytes
} __attribute__((packed));

struct  BMP_DIB_HEADER_STRUCT
{
	unsigned width; 				// 4 bytes
	unsigned height;				// 4 bytes
	unsigned short color_panel;		// 2 bytes
	unsigned short bits_per_pixel;	// 2 bytes
	unsigned compression_method;	// 4 bytes
	unsigned raw_size;				// 4 bytes
	unsigned horizontal_resolution; // 4 bytes
	unsigned vertical_resolution;	// 4 bytes
	unsigned color_palette; 		// 4 bytes
	unsigned important_colors; 		// 4 bytes
	unsigned others[21];
}__attribute__((packed));

/*extern struct BMP_FILE_HEADER_STRUCT BMP_FILE;
extern struct BMP_DIB_HEADER_STRUCT BMP_DIB;
*/
#endif