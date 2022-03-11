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

    if (argc < 2)
    {
        printf("error not enuf files\n");
        exit(1);
    }

    if (strcmp(argv[1], "train") == 0)
    {
        int width, height, channels;
        char buf[18];

        for (unsigned int i = 1; i < atoi(argv[2]) + 1; i++)
        {
            printf("train %d", i);

            int width2, height2, channels2;
            unsigned char *w = stbi_load("w.jpg", &width2, &height2, &channels2, 0);

            if (w == NULL) {
                printf("error in loading the weights\n");
            }

            snprintf(buf, 18, "4/%d.jpg", i);
            unsigned char *img = stbi_load(buf, &width, &height, &channels, 0);

            if(img == NULL)
            {
                printf("error in loading the image\n");
                exit(1);
            }

            unsigned char *img2 = GrayScale(img, width, height, channels);

            unsigned char *w2 = GrayScale(w, width2, height2, channels2);

            float ans = 0.0;

            for (unsigned int i = 0; i < 30 * 30; i++)
            {
                ans = ans + (((float)img2[i] / 255.0) * ((float)w2[i] / 255.0));
            }    

            printf(" %f", ans);

            if (ans < 70.0)
            {
                for (unsigned int i = 0; i < 30 * 30; i++)
                {
                    w2[i] =  map(img2[i] - 200 + w2[i], 0, 510, 0, 255);
                }
                stbi_write_jpg("w.jpg", width2, height2, 1, w2, 100);
            }
            else if (ans > 90.0)
            {
                for (unsigned int i = 0; i < 30 * 30; i++)
                {
                    w2[i] =  map(img2[i] + w2[i], 0, 510, 0, 255);
                }
                stbi_write_jpg("w.jpg", width2, height2, 1, w2, 100);
            }

            printf(" Done\n");

        }
    }
    if (strcmp(argv[1], "clear") == 0)
    {
        int width2, height2, channels2;
        unsigned char *w = stbi_load("w.jpg", &width2, &height2, &channels2, 0);

        if (w == NULL) {
            printf("error in loading the weights\n");
        }

        unsigned char *w2 = GrayScale(w, width2, height2, channels2);

        for (unsigned int i = 0; i < 30 * 30; i++)
        {
            w2[i] = 0;
        }
        stbi_write_jpg("w.jpg", width2, height2, 1, w2, 100);
    }
    else
    {
        int width, height, channels;
        unsigned char *img = stbi_load(argv[1], &width, &height, &channels, 0);

        if(img == NULL)
        {
            printf("error in loading the image\n");
            exit(1);
        }

        int width2, height2, channels2;
        unsigned char *w = stbi_load("w.jpg", &width2, &height2, &channels2, 0);

        if (w == NULL) {
            printf("error in loading the weights\n");
        }

        unsigned char *img2 = GrayScale(img, width, height, channels);

        unsigned char *w2 = GrayScale(w, width2, height2, channels2);

        float ans = 0.0;

        for (unsigned int i = 0; i < 30 * 30; i++)
        {
            ans = ans + (((float)img2[i] / 255.0) * ((float)w2[i] / 255));
        }        

        if (ans > 70.0 && ans < 90.0)
        {
            printf("Its a 4 %f\n", ans);
        }
        else
        {
            printf("Not a 4 %f\n", ans);
        }
    }
    
    return 0;
}