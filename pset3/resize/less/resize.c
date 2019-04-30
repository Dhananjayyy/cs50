// Copies a BMP file

#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: resize multiplier infile outfile\n");
        return 1;
    }

    // ensure that proper scale multiplier is given
    int scaleBy = atoi(argv[1]);

    if (scaleBy < 1 || scaleBy > 100)
    {
        fprintf(stderr, "Scaling factor between 0 and 100");
    }

    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    // determine padding of input and output
    int inPadding  = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int outPadding = (4 - (scaleBy * bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // obtain metadata of inptr for future computations
    int inWidth    = bi.biWidth;
    int inHeight   = abs(bi.biHeight);

    // Update metadata for outptr
    bi.biWidth *= scaleBy;
    bi.biHeight *= scaleBy;
    bi.biSizeImage = ((sizeof(RGBTRIPLE) * bi.biWidth) + outPadding) * abs(bi.biHeight);
    bf.bfSize = bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);


    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);


    // iterate over infile's scanlines
    for (int i = 0; i < inHeight; i++)
    {

        // copy every line of inptr multiplier - 1 times to outptr
        for (int h = 0; h < (scaleBy - 1); h++)
        {
            // iterate over pixels in scanline
            for (int j = 0; j < inWidth; j++)
            {
                // temporary storage
                RGBTRIPLE triple;

                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

                // copy same pixel multiplier times into outptr
                for (int k = 0; k < scaleBy; k++)
                {
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }
            }

            // then add padding to outptr
            for (int k = 0; k < outPadding; k++)
            {
                fputc(0x00, outptr);
            }

            // move to the start of the same line in inptr
            fseek(inptr, - (inWidth * sizeof(RGBTRIPLE)), SEEK_CUR);
        }


        // last time same line supposed to be copied to outptr from inptr
        for (int j = 0; j < inWidth; j++)
        {
            // temporary storage
            RGBTRIPLE triple;

            // read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

            // copy same pixel multiplier times into outptr
            for (int k = 0; k < scaleBy; k++)
            {
                fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
            }
        }

        // outptr padding
        for (int k = 0; k < outPadding; k++)
        {
            fputc(0x00, outptr);
        }

        // skip over padding of inptr, if any
        fseek(inptr, inPadding, SEEK_CUR);
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}