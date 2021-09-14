# stm32f4_img2c
Generates C and Header files from an image for displaying it on DM-LCD35RT module using standar library. This is an alternative for Embedded Resource Editor GUI by ST, that doesn't have support for Linux.
## Bug Report
1. ImageMagick sometimes doesn't conver the RGB888 to RGB565, so, I suggest to use this web page https://online-converting.com/image/convert2bmp to conver any image into RGB565, then use stm32f4_img2c to generate the c code
## Table of contents
1. Getting Started
2. Compilation
3. Requirements
4. References

## 1. Getting Started
Given the fact it uses ImageMagick, the input image can be in any format that ImageMagick can handle, such as: PNG, JPG, BMP, etc. Be aware that the image size for this LCD is 320x240 max. stm32f4_img2c won't verify the size, because it might be helpful to generate BMP code for all image sizes.
1. Compile the sources
3. Give executable attributes:
```bash
chmod +x stm32f4_img2c
```
2. Generate the code from any image
```bash
stm32f4_img2c input_image.JPG output/directory
```
3. if you already have and image on BMP RGB565, call the bmp2c:
```bash
bin/stm32f4_bmp2c input_image.bmp input_image.bmp output/directory
```
Yes, the input image is twice

## 2. Compilation
1. Create a new folder
```bash
mkdir bin
```
2. Compile
```bash
make
```

## 3. Requirements
1. Make
2. GCC
3. ImageMagick

## 4. References
1. [Unofficial Documentation ](https://github.com/ckevar/stm32f4_img2c/wiki/IMAGE-ON-THE-LCD35RT,-USING-STM32F4-Discovery)
1. [BMP file format](https://en.wikipedia.org/wiki/BMP_file_format)
1. [Embedded Resource Editor GUI](https://www.element14.com/community/thread/26813/l/stm32f4-loading-image?displayFullThread=true)

