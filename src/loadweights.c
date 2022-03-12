#include "include/loadweights.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint32_t loadVals(float *p, uint32_t *n, char *filename)
{
    FILE *WFile = fopen(filename, "r");

	char c;
    char C_WLen[5] = {0};
    uint32_t i = 0;

	while((c = fgetc(WFile)) != '\n')
    {
        if (c == EOF)
        {
            printf("weights file is empty!");
            exit(1);
        }

        C_WLen[i] = c;
        i++;
    }

    uint32_t I_WLen = atoi(C_WLen);
    *n = I_WLen;

    for (uint32_t i = 0; i < I_WLen; i++)
    {
        uint32_t j = 0;
        char temp[18] = {0};

        while((c = fgetc(WFile)) != '\n')
        {
            if (j > 18)
            {
                printf("Float in weights file is to long at line (%i)\n", i + 2);
                exit(1);
            }

            if (c == EOF)
            {
                break;
            }

            temp[j] = c;
            j++;
        }

        p[i] = atof(temp);
    }
		
	fclose(WFile);

	return 0;
}

int resetVals(char *filename)
{
    FILE *WFile = fopen(filename, "r");

    char c;
    char C_WLen[5] = {0};
    uint32_t i = 0;

	while((c = fgetc(WFile)) != '\n')
    {
        if (c == EOF)
        {
            printf("weights file has no set length");
            exit(1);
        }

        C_WLen[i] = c;
        i++;
    }

    uint32_t I_WLen = atoi(C_WLen);

    if (I_WLen == 0)
    {
        printf("weights file length is 0");
        exit(1);
    }

    freopen(filename, "w+", WFile);

    fprintf(WFile, "%i\n", I_WLen);

    for (uint32_t i = 0; i < I_WLen; i++)
    {
        fprintf(WFile, "%f\n", 0.000000);
    }

    fclose(WFile);

    return 0;
}

int saveVals(float *p, uint32_t n, char *filename)
{
    FILE *WFile = fopen(filename, "w+");

    fprintf(WFile, "%i\n", n);

    for (uint32_t i = 0; i < n; i++)
    {
        fprintf(WFile, "%f\n", p[i]);
    }

    fclose(WFile);

    return 0;
}