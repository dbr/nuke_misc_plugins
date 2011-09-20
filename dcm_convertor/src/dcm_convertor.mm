#include <iostream>

// Houdini headers
#include <IMG/IMG_DeepShadow.h>
#include <UT/UT_Exit.h>
#include <UT/UT_Options.h>
#include <UT/UT_WorkBuffer.h>

// PRMan
#include <dtex.h>


template <typename FTYPE> static void
printTuple(const FTYPE *pixel, int vsize)
{
    int i;
    printf("(%g", pixel[0]);
    for (i = 1; i < vsize; i++)
        printf(",%g", pixel[i]);
    printf(")");
}


static void
printPixel(IMG_DeepShadow &fp, int x, int y)
{
    int i, d, depth;
    const IMG_DeepShadowChannel *chp;
    IMG_DeepPixelReader pixel(fp);

    // Open the pixel
    if (!pixel.open(x, y))
    {
        printf("\tUnable to open pixel [%d,%d]!\n", x, y);
        return;
    }

    // Get the number of z-records for the pixel
    depth = pixel.getDepth();
    printf("Pixel[%d,%d] (samples: %d)\n", x, y, depth);

    // Iterate over all channels in the DCM
    for (i = 0; i < fp.getChannelCount(); i++)
    {
        chp = fp.getChannel(i);
        printf(" %5s = [", chp->getName());
        if (depth)
        {
            // Print first depth record
            printTuple<float>(pixel.getData(*chp, 0), chp->getTupleSize());
            // And the remaining depth records
            for (d = 1; d < depth; d++)
            {
                printf(", ");
                printTuple<float>(pixel.getData(*chp, d), chp->getTupleSize());
            }
        }
        printf("]\n");
    }
}


static void
dumpOptions(IMG_DeepShadow &fp)
{
    const UT_Options    *opt;
    UT_WorkBuffer        wbuf;
    if (opt = fp.getTBFOptions())
    {
        wbuf.strcpy("DSM Options: ");
        opt->appendPyDictionary(wbuf);
        printf("%s\n", wbuf.buffer());
    }
}

int
main(int argc, char *argv[])
{
    if(argc != 3)
    {
        printf("Usage: %s in.rat out.dtex\n", argv[0]);
    }

    IMG_DeepShadow fp;
    if(!fp.open(argv[1]))
    {
        std::cerr << "Usage!\n";
        return -1;
    }

    DtexCache *outCache;
    outCache = DtexCreateCache(128, NULL);
    DtexFile *outFile;

    if (DtexCreateFile(argv[2], outCache, &outFile) != DTEX_NOERR) {
        fprintf(stderr, "Unable to open output file %s.\n", argv[2]);
        return 1;
    }

    int xres, yres;
    fp.resolution(xres, yres);
    printf("%s [%d,%d] (%d channels)\n",
           argv[1], xres, yres, fp.getChannelCount());


    /* Create the appropriate output image now */
    float NP[16], Nl[16];

    DtexImage *outImage;
    int error;
    error = DtexAddImage(
        outFile,
        "/tmp/test.dtex",
        fp.getChannelCount(),
        xres,
        yres,
        1,
        1,
        NP,
        Nl,
        DTEX_COMPRESSION_NONE, //TODO: Better value
        DTEX_TYPE_FLOAT, // Determine from HDK API
        &outImage);

    if(error != DTEX_NOERR)
    {
		fprintf(stderr, "Unable to create output image, DTEX errno: %i\n", error);
		return 1;
    }


        // 
    dumpOptions(fp);

    for(int x = 0; x < xres; ++x)
    {
        for(int y = 0; y < yres; ++y)
        {
            /*
            printf("WHHAT\n");
            if (DtexGetPixel(outImage, x, y, outPixel) != DTEX_NOERR) {
                fprintf(stderr, "Unable to get output pixel at %d %d.\n", x, y);
                continue;
            }
            */

            printPixel(fp, x, y);
            return 2;
        }
    }
}
