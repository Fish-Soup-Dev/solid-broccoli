#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "include/loadweights.h"
#include "include/tools.h"

#define STB_IMAGE_IMPLEMENTATION
#include "include/stb_image/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "include/stb_image/stb_image_write.h"

uint8_t* GrayScale(uint8_t *img, int width, int height, int channels)
{
    size_t img_size = width * height * channels;
    int gray_channels = channels == 4 ? 2 : 1;
    size_t gray_img_size = width * height * gray_channels;

    uint8_t *gray_img = malloc(gray_img_size);

    if(gray_img == NULL)
    {
        printf("unable to allocate memory for the gray image.\n");
        exit(1);
    }

    for(uint8_t *p = img, *pg = gray_img; p != img + img_size; p += channels, pg += gray_channels)
    {
        *pg = (uint8_t)((*p + *(p + 1) + *(p + 2))/3.0);
        if(channels == 4)
        {
            *(pg + 1) = *(p + 3);
        }
    }

    return gray_img;
}

int main(int argc, char *argv[])
{
    float WeightList[10001] = {0};
    uint32_t NumOfVals = 0;

    loadVals(WeightList, &NumOfVals, "w.txt");

    if (strcmp(argv[1], "clear") == 0)
    {
        resetVals("w.txt");
        printf("Values reset\n");

        loadVals(WeightList, &NumOfVals, "w.txt");

        int width2, height2, channels2;
        uint8_t *OutW = stbi_load("w.jpg", &width2, &height2, &channels2, 0);
        
        uint32_t index = 0;

        for (uint32_t i = 0; i < NumOfVals; i++)
        {
            if (WeightList[i] > 0.0)
            {
                OutW[index++] = (uint8_t)map(WeightList[i], 0.0, 200.0, 0, 255);
                OutW[index++] = (uint8_t)0;
                OutW[index++] = (uint8_t)0;
            }
            else if (WeightList[i] < 0.0)
            {
                OutW[index++] = (uint8_t)0;
                OutW[index++] = (uint8_t)map(WeightList[i], 0.0, 200.0, 0, 255);
                OutW[index++] = (uint8_t)0;
            }
            else if (WeightList[i] == 0.0)
            {
                OutW[index++] = (uint8_t)0;
                OutW[index++] = (uint8_t)0;
                OutW[index++] = (uint8_t)0;
            }
        }

        stbi_write_jpg("w.jpg", 30, 30, 3, OutW, 100);
        printf("Done makeing image\n");
    }
    else if (strcmp(argv[1], "learn") == 0)
    {
        int width, height, channels;
        char buf[18];

        for (uint32_t j = 0; j < atoi(argv[3]); j++)
        {
            printf("Startting pass %i of %i\n", j, atoi(argv[3]));

            for (uint32_t i = 1; i < atoi(argv[2]) + 1; i++)
            {
                printf("train %d", i);

                snprintf(buf, 18, "4/%d.jpg", i);
                uint8_t *img = stbi_load(buf, &width, &height, &channels, 0);

                if(img == NULL)
                {
                    printf("error in loading the image\n");
                    exit(1);
                }

                uint8_t *grayimg = GrayScale(img, width, height, channels);

                float ans = 0.0;

                for (uint32_t i = 0; i < width * height; i++)
                {
                    ans = ans + (((float)grayimg[i] / 255.0) * ((float)WeightList[i] + 1 / 255.0));
                }

                printf(" %f", ans);

                if (ans < 8600.0)
                {
                    for (uint32_t i = 0; i < NumOfVals; i++)
                    {
                        WeightList[i] = WeightList[i] + (float)grayimg[i] / 255;
                    }

                    saveVals(WeightList, NumOfVals, "w.txt");
                }
                if (ans > 9000.0)
                {
                    for (uint32_t i = 0; i < NumOfVals; i++)
                    {
                        WeightList[i] = WeightList[i] - (float)grayimg[i] / 255;
                    }

                    saveVals(WeightList, NumOfVals, "w.txt");
                }
                if (ans < 9000.0 && ans > 8600.0)
                {
                    printf(" (!)");
                }
                else
                {
                    printf(" ( )");
                }

                printf(" Done\n");
            }
        }
        int width2, height2, channels2;
        uint8_t *OutW = stbi_load("w.jpg", &width2, &height2, &channels2, 0);
        
        uint32_t index = 0;

        for (uint32_t i = 0; i < NumOfVals; i++)
        {
            if (WeightList[i] > 0.0)
            {
                OutW[index++] = (uint8_t)map(WeightList[i], 0.0, 200.0, 0, 255);
                OutW[index++] = (uint8_t)0;
                OutW[index++] = (uint8_t)0;
            }
            else if (WeightList[i] < 0.0)
            {
                OutW[index++] = (uint8_t)0;
                OutW[index++] = (uint8_t)0;
                OutW[index++] = (uint8_t)map(WeightList[i] * -1, 0.0, 200.0, 0, 255);
            }
            else if (WeightList[i] == 0.0)
            {
                OutW[index++] = (uint8_t)0;
                OutW[index++] = (uint8_t)0;
                OutW[index++] = (uint8_t)0;
            }
        }

        stbi_write_jpg("w.jpg", 30, 30, 3, OutW, 100);
        printf("Done makeing image\n");
}
    else if (strcmp(argv[1], "run") == 0)
    {
        int width, height, channels;
        uint8_t *img = stbi_load(argv[2], &width, &height, &channels, 0);

        if(img == NULL)
        {
            printf("error in loading the image\n");
            exit(1);
        }

        uint8_t *grayimg = GrayScale(img, width, height, channels);

        float ans = 0.0;

        for (uint32_t i = 0; i < width * height; i++)
        {
            ans = ans + (((float)grayimg[i] / 255.0) * ((float)WeightList[i] + 1 / 255.0));
        }

        if (ans > 8600.0 && ans < 9000.0)
        {
            printf("This is a 4\n");
        }
        else
        {
            printf("This is NOT a 4\n");
        }

        printf("%f\n", ans);
        printf("Done\n");
    }
	return 0;
}