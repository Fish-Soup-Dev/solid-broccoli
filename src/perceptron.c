#include <stdio.h>
#include <stdint.h>
#include <math.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image/stb_image_write.h"

unsigned char* GrayScale(unsigned char *img, int width, int height, int channels)
{
    size_t img_size = width * height * channels;
    int gray_channels = channels == 4 ? 2 : 1;
    size_t gray_img_size = width * height * gray_channels;

    unsigned char *gray_img = malloc(gray_img_size);

    if(gray_img == NULL)
    {
        printf("unable to allocate memory for the gray image.\n");
        exit(1);
    }

    for(unsigned char *p = img, *pg = gray_img; p != img + img_size; p += channels, pg += gray_channels)
    {
        *pg = (uint8_t)((*p + *(p + 1) + *(p + 2))/3.0);
        if(channels == 4)
        {
            *(pg + 1) = *(p + 3);
        }
    }

    return gray_img;
}

float map(int x, int in_min, int in_max, float out_min, float out_max)
{
    return ((float)x - (float)in_min) * (out_max - out_min) / ((float)in_max - (float)in_min) + out_min;
}

int main(int argc, char *argv[])
{

    if (argc < 3)
    {
        printf("error not enuf files\n");
        exit(1);
    }

    int width, height, channels;
    unsigned char *img = stbi_load(argv[1], &width, &height, &channels, 0);

    if(img == NULL)
    {
        printf("error in loading the image\n");
        exit(1);
    }

    int width2, height2, channels2;
    unsigned char *w = stbi_load(argv[2], &width2, &height2, &channels2, 0);

    if (w == NULL) {
        printf("error in loading the weights\n");
    }

    unsigned char *img2 = GrayScale(img, width, height, channels);

    unsigned char *w2 = GrayScale(w, width2, height2, channels2);

    unsigned int ans;

    for (unsigned int i = 0; i < 10 * 10; i++)
    {
        ans = ans + (img2[i] / 255) * (w2[i] / 255);
    }

    if (ans > 3000000)
    {
        printf("Square?\n");
        printf("was i right? ");
    }
    else
    {
        printf("Not square?\n");
        printf("was i right? ");
    }
    char t;
    scanf(" %c", &t);
    
    if (t == 'n')
    {
        for (unsigned int i = 0; i < 10 * 10; i++)
        {
            w2[i] =  map(img2[i] + w2[i], 0, 510, 0, 255);
        }
        stbi_write_jpg("w.jpg", width2, height2, 1, w2, 100);
    }
    
    return 0;
}