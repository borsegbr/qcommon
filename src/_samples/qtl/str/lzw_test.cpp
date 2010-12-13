#include <conio.h>
#include "lzw.h"

#if 0
int main (void)
{
    char *src = "TOBEORNOTTOBEORTOBEORNOT";

	qtl::lzw_codec<> lzw;
    
    lzw.set_symbol_count(32);
    lzw.reset();

    int lenSrc = strlen(src);
    unsigned short out[100];
    int j = 0;
    for (int i = 0; i < lenSrc; i++)
    {
        unsigned char c = (unsigned char)src[i];
        c -= 'A' + 1;
        if (lzw.encode_one(c, out[j]))
        {
            if (out[j] < 32)
            {
                printf("%c\n", out[j] + 'A' + 1);
            }
            else
            {
                printf("%d\n", out[j]);
            }
            j++;
        }
    }

    lzw.encode_flush(out[j]);
    printf("%d\n", out[j]);
    j++;

    printf("decoding...\n");
    lzw.reset();
    qtl::LzwSymbols<> decoded;
    for (int i = 0; i < j; i++)
    {
        lzw.decode_one(out[i], decoded);
        for (int k = 0; k < decoded.size(); k++)
        {
            printf("%c", decoded[k] + 'A' + 1);
        }
    }

    return 0;
}
#else

int encode (void)
{
    FILE *fIn = fopen("src.txt", "rb");
    FILE *fOut = fopen("enc.dat", "wb");

	qtl::lzw_core<>		lzw;
    qtl::lzw_encoder<>	enc;
    int ctrl = 0;

	unsigned short out[2];
	unsigned char emit[3];
    int i = 0;
	while (1)
    {
        unsigned char ch;
		int got = fread(&ch, 1, 1, fIn);
		if (got == 0)
		{
			break;
		}
        if (enc.encode_one(lzw, ch, out[i]))
        {
            i++;
			if (i == 2)
			{
				emit[0] = (unsigned char)(out[0] >> 4);
				emit[1] = (unsigned char)((out[0] << 4) | (out[1] >> 8));
				emit[2] = (unsigned char) (out[1] & 0xff);

				fwrite(emit, 1, 3, fOut);
				i = 0;
			}
        }
    }

	enc.encode_flush(lzw, out[i]);
	i++;
	if (i == 2)
	{
		emit[0] = (unsigned char)(out[0] >> 4);
		emit[1] = (unsigned char)((out[0] << 4) | (out[1] >> 8));
		emit[2] = (unsigned char) (out[1] & 0xff);

		fwrite(emit, 1, 3, fOut);
	}
	else /* i == 1 */
	{

		emit[0] = (unsigned char)(out[0] >> 4);
		emit[1] = (unsigned char)(out[0] << 4);
		fwrite(emit, 1, 2, fOut);
	}

    fclose(fIn);
    fclose(fOut);

    return 0;
}

int file_size (FILE *f)
{
	int rsrv = ftell(f);
	fseek(f, 0, SEEK_END);
	int result = ftell(f);
	fseek(f, rsrv, SEEK_SET);
	return result;
}

int decode (void)
{
    FILE *fIn = fopen("enc.dat", "rb");
    FILE *fOut = fopen("dst.txt", "wb");

	int fsize = file_size(fIn);

	unsigned char eat[3];
	unsigned short in[2];

	qtl::lzw_core<>		lzw;
	qtl::lzw_decoder<>	dec;
	qtl::LzwSymbols<>	decoded;
	while (1)
	{
		int got = fread(eat, 1, 3, fIn);
		if (got == 2)
		{
			in[0] = eat[0];
			in[0] <<= 4;
			in[0] |= eat[1] >> 4;
			dec.decode_one(lzw, in[0], decoded);
			for (int i = 0; i < decoded.size(); i++)
			{
				fwrite(&decoded[i], 1, 1, fOut);
			}
			break;
		}
		else if (got < 3)
		{
			break;
		}
		in[0] = eat[0];
		in[0] <<= 4;
		in[0] |= eat[1] >> 4;
		in[1] = eat[1] & 0xf;
		in[1] <<= 8;
		in[1] |= eat[2];
		dec.decode_one(lzw, in[0], decoded);
		for (int i = 0; i < decoded.size(); i++)
		{
			fwrite(&decoded[i], 1, 1, fOut);
		}
		dec.decode_one(lzw, in[1], decoded);
		for (int i = 0; i < decoded.size(); i++)
		{
			fwrite(&decoded[i], 1, 1, fOut);
		}
	}

    fclose(fIn);
    fclose(fOut);

    return 0;
}

int main ()
{
	encode();
	decode();
	return 0;
}

#endif

