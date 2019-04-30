#include <stdio.h>
#include <stdlib.h>
#include <cs50.h>

// function to check whether or not the given 512 byte block has jpg header
bool isJPEGHeader(unsigned char block[512]);

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: recover forensicimage.raw");
        return 1;
    }

    // open forensic image and verify that it exists
    FILE *rImg = fopen(argv[1], "r");

    if (!rImg)
    {
        fprintf(stderr, "File %s not found.", argv[1]);
        return 2;
    }

    // prerequisite variable declaration
    bool isEof = false;
    bool isJPEGOpen = false;
    int nImg = 0;
    FILE *jpegFile = NULL;

    while (true)
    {
        unsigned char b[512];
        // if returned block is less than standard 512 bytes size we are at eof
        if (fread(b, 1, 512, rImg) < 512)
        {
            printf("EOF!\n");
            break;
        }

        // if we encounter header
        if (isJPEGHeader(b))
        {
            // but if a file is already open
            if (isJPEGOpen)
            {
                // close current file
                fclose(jpegFile);
                isJPEGOpen = false; // line does nth
                nImg = nImg + 1;
            }

            // open new file
            char jpeg[8]; //name is num of char + \0
            sprintf(jpeg, "%03i.jpg", nImg);
            jpegFile = fopen(jpeg, "w");
            isJPEGOpen = true;
        }

        // if jpeg open write 512 bytes to jpeg
        if (isJPEGOpen)
        {
            fwrite(b, 1, 512, jpegFile);
        }
    }

    fclose(jpegFile);
    fclose(rImg);
    return 0;
}


bool isJPEGHeader(unsigned char block[512])
{
    if (block[0] == 0xff && block[1] == 0xd8 && block[2] == 0xff && (block[3] & 0xf0) == 0xe0)
    {
        return true;
    }
    return false;
}