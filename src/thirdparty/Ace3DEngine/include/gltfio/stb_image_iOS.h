//
// Created by czerzhang on 2020/1/2.
//

#ifndef TNT_STB_IMAGE_IOS_H
#define TNT_STB_IMAGE_IOS_H

typedef unsigned char stbi_uc;

extern unsigned char * stbi_load_from_memory(unsigned char const *buffer, int len);
extern int stbi_info_from_memory(unsigned char const *buffer, int len, int *x, int *y);
extern int stbi_info_from_file(char const *filename, int *x, int *y, unsigned char *buffer,
                               int *bufferSize);
extern void stbi_image_free(void *retval_from_stbi_load);

#endif  // TNT_STB_IMAGE_IOS_H
