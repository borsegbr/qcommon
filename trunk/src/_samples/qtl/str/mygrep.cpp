#include <qtl/str/kmp.h>


#define MAX_MATCH_SIZE	128
#define BUF_SIZE		1024



typedef qtl::kmp<unsigned char *>	Kmp;

struct GrepInfo
{
	FILE			*fIn;
	unsigned char	match[MAX_MATCH_SIZE];
	int				matchLen;
    unsigned char   *rematch;
	int				rep;
};

void ParseArgs (GrepInfo *info, int argc, char *argv[])
{
	info->fIn = NULL;
	info->rep = 0;
	info->matchLen = 0;
	for (int i = 1; i < argc; )
	{
		char *arg = argv[i];
		if (strcmp(arg, "-i") == 0)
		{
			i++;
			arg = argv[i];
			info->fIn = fopen(arg, "rb");
			i++;
		}
		else if (strcmp(arg, "-match") == 0)
		{
			int j = 0;
			i++;
			for (j = 0; j < MAX_MATCH_SIZE && i < argc; i++)
			{
				arg = argv[i];
				if (arg[0] != '-')
				{
                    sscanf(arg, "%x", &info->match[j++]);
				}
				else
				{
					break;
				}
			}
			info->matchLen = j;
			argv[j] = 0;
		}
		else if (strcmp(arg, "-rep") == 0)
		{
			i++;
			info->rep = atoi(argv[i]);
			i++;
		}
	}

}


int find (Kmp *k, GrepInfo *info)
{
	unsigned char buffer[BUF_SIZE];
	unsigned char *p, *pBufEnd;
	int rep = info->rep;
	int readSize;
	int offset = 0;

	k->reset();

	do
	{
		readSize = fread(buffer, 1, BUF_SIZE, info->fIn);
		p = buffer;
		pBufEnd = buffer + readSize;
		
		while (p < pBufEnd)
		{
			unsigned char *pp;
			k->get_p_ptrn(pp);
			if (k->find_onestep(p))
			{
				if (k->is_matched())
				{
					if (rep == 0)
					{
						offset++;
						offset -= info->matchLen;
						return offset;
					}
					else
					{
						rep--;
                        p++;
                        offset++;
                        k->set_p_ptrn(info->rematch);
					}
				}
				else
				{
					p++;
					offset++;
				}

			}
		}

	} while (readSize == BUF_SIZE);

	return (-1);
}

void next_rematch_offset (GrepInfo *info)
{
    int recur = info->matchLen;
    for (int i = 1; i < info->matchLen; i++)
    {
        bool matched = true;
        for (int j = 0; j < info->matchLen - i; j++)
        {
            if (info->match[i + j] != info->match[j])
            {
                matched = false;
                break;
            }
        }
        if (matched)
        {
            recur = i;
            break;;
        }
    }
    info->rematch = info->match + info->matchLen - recur;
    
}

int main (int argc, char* argv[])
{
	GrepInfo	info;
	ParseArgs(&info, argc, argv);
	
	if (!info.fIn)
	{ 
		printf("! Error opening file.\n");
		return 0;
	}
	if (info.matchLen == 0)
	{
		printf("! String to be matched not specified.\n");
		return 0;
	}

	Kmp k;
    next_rematch_offset(&info);
	k.set_ptrn(info.match, info.match + info.matchLen);
	int i = find(&k, &info);
	printf("found at %d(0x%08X)\n", i, i);

	return 0;
}
