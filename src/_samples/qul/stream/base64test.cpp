#include <stdio.h>
#include <string.h>

#include <qul/stream/base64.h>


using namespace qul;

typedef struct
{
    FILE    *fIn;
    FILE    *fOut;
    enum
    {
        k_Encode,
        k_Decode
    }       workingMode;
} Base64Info;


int GetFileSize (FILE *f)
{
    int size = 0;
    int pos = ftell(f);
    fseek(f, 0, SEEK_END);
    size = ftell(f);
    fseek(f, pos, SEEK_SET);
    return size;
}

int ParseArgs (Base64Info *info, int argc, char *argv[])
{
    int result = 0;
    int i;
    info->workingMode = Base64Info::k_Encode;
    info->fIn = NULL;
    info->fOut = NULL;
    for (i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-i") == 0)
        {
            i++;
            info->fIn = fopen(argv[i], "rb");
        }
        else if (strcmp(argv[i], "-o") == 0)
        {
            i++;
            info->fOut = fopen(argv[i], "w");
        }
        else if (strcmp(argv[i], "-d") == 0)
        {
            info->workingMode = Base64Info::k_Decode;
        }
        else if (strcmp(argv[i], "-e") == 0)
        {
            info->workingMode = Base64Info::k_Encode;
        }
    }
    if (info->fIn == NULL)
    {
        printf("Error, input file not specified or failed to be opened!\n");
        result = 1;
    }
    if (info->fIn == NULL)
    {
        printf("Error, output file not specified or failed to be opened!\n");
        result = 1;
    }
    return result;
}


int Base64Encode (Base64Info *info)
{
    Base64Encoder   encoder;
    int             charCount = GetFileSize(info->fIn);
    int             b64Count = encoder.GetEncodedLen(charCount);
    unsigned char   *bufIn = new unsigned char[charCount];
    char            *bufOut = new char[b64Count];

    fseek(info->fIn, 0, SEEK_SET);
    fread(bufIn, 1, charCount, info->fIn);
    encoder.Encode(bufOut, bufIn, charCount);
    fwrite(bufOut, 1, b64Count, info->fOut);

    printf("%d character(s) encoded into %d base64 chars\n", charCount, b64Count);

    delete[] bufIn;
    delete[] bufOut;
}

int Base64Decode (Base64Info *info)
{
    Base64Decoder   decoder;
    int             b64Count = GetFileSize(info->fIn);
    if (b64Count % 4)
    {
        return -1;
    }
    char            *bufIn = new char[b64Count];
    int decBufLen = decoder.GetBufferLen(b64Count);
    if (decBufLen < 0)
    {
        return -1;
    }
    unsigned char   *bufOut = new unsigned char[decBufLen];
    
    fseek(info->fIn, 0, SEEK_SET);
    fread(bufIn, 1, b64Count, info->fIn);
    int charCount = decoder.Decode(bufOut, bufIn, b64Count);
    fwrite(bufOut, 1, charCount, info->fOut);

    printf("%d character(s) decoded\n", charCount);

    delete[] bufIn;
    delete[] bufOut;
}

int main (int argc, char *argv[])
{
    Base64Info  info;
    if (ParseArgs(&info, argc, argv))
    {
        return 0;
    }

    switch (info.workingMode)
    {
    case Base64Info::k_Encode:
        Base64Encode(&info);
        break;
    case Base64Info::k_Decode:
        Base64Decode(&info);
        break;
    }

    return 0;
}
