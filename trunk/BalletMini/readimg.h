#ifndef _READ_IMG_H_
#define _READ_IMG_H_

IMGHDR *read_pngimg(const char *buf);

IMGHDR *read_jpgimg(const char *buf);

IMGHDR *ConvertRGBAToRGB8(const char *buf, unsigned int width, unsigned int height);

IMGHDR *CreateFrame(unsigned int width, unsigned int height, const char *color);

IMGHDR *CreateDelimiter(unsigned int width, unsigned int height, const char *color);

IMGHDR *CreateButton(unsigned int width, unsigned int height);

#endif /* _READ_IMG_H_ */
