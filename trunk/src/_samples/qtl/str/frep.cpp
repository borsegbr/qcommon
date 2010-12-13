#include <conio.h>
#include <cstdio>
#include <string>


#define DEBUG_REGEXP    0
#include <qtl/str/regexp_replacer.h>


#if DEBUG_REGEXP
#   define DEBUG_VIEW(re)  re.DebugView();
#else
#   define DEBUG_VIEW(re)
#endif

#ifdef MSVC
#   define getch    _getch
#   define gets     gets_s
#   define sprintf  sprintf_s
#endif

using namespace qtl;
using namespace std;


#define MAX_FLEN    (16*1024*1024)

typedef unsigned char Byte;

int file_init (FILE *f)
{
    fseek(f,0,SEEK_END);
    int len = ftell(f);
    fseek(f,0,SEEK_SET);
    return len;
}

int main (void)
{
    char inpbuf[1024];
    char filename[1024] = {0, };
    int  fbufsize = 0;
    char *fbuf = 0;
    bool quit = false;

    typedef RegExp<char*, char*>    RE;
    RE  re;

    enum ReplaceStyle
    {
        ReplaceDisabled,
        ReplaceQuery,
        ReplaceAll,
    } repStyle = ReplaceDisabled;

    typedef RegExpReplacer<RE>  Rep;
    Rep rep;
    int flen;

    while (!quit)
    {
        printf("-");
        int c = getch();
        printf("%c\n", c);
        switch (c)
        {
        case 'O': case 'o':
            {
                printf("Input file name>");
                gets(filename);
                FILE *fIn = fopen(filename, "r");     // binary mode?

                if (fIn == 0)
                {
                    printf("! Error opening file.\n");
                    break;
                }

                flen = file_init(fIn);
                if (flen > MAX_FLEN)
                {
                    printf("This file exceeds the size limit of 16MB\n");
                    fclose(fIn); fIn = 0;
                    flen = 0;
                    break;
                }
                if (flen > fbufsize)
                {
                    fbuf = (char*)realloc(fbuf, flen);
                    if (!fbuf)
                    {
                        printf("Memory allocation error\n");
                        fclose(fIn); fIn = 0;
                        flen = 0;
                        break;
                    }
                    fbufsize = flen;
                }
                flen = (int)fread(fbuf, 1, flen, fIn);

                fclose(fIn);
            }
            break;
        case 'Q': case 'q':
            quit = true;
            break;
        case 'F': case 'f':
        case 'R': case 'r':
            {
                char *p;
                FILE *fOut = 0;
                string repfn;

                if (flen == 0)
                {
                    printf("! No file assigned.\n");
                    break;
                }

                printf("Set regular expression>\n");
                gets(inpbuf);
                p = inpbuf;
                if (!re.SetPattern(p, inpbuf + strlen(inpbuf)))
                {
                    printf("! Pattern error at %d\n", p - inpbuf);
                    DEBUG_VIEW(re);
                    break;
                }
                DEBUG_VIEW(re);

                if (c == 'R' || c == 'r')
                {
                    printf("Replace all? (Y for yes)>");
                    if (getch() == 'Y')
                    {
                        repStyle = ReplaceAll;
                    }
                    else
                    {
                        repStyle = ReplaceQuery;
                    }
                    printf("\n");
                    printf("Replace pattern>\n");
                    gets(inpbuf);
                    p = inpbuf;
                    if (!rep.AssignPattern(re, p, inpbuf + strlen(inpbuf)))
                    {
                        printf("! Pattern error at %d\n", p - inpbuf);
                        break;
                    }
                    sprintf(inpbuf, "%s.rep", filename);
                    repfn = inpbuf;
                    fOut = fopen(inpbuf, "w");
                    if (!fOut)
                    {
                        printf("! Error opening replacement file\n");
                        break;
                    }
                }
                else
                {
                    repStyle = ReplaceDisabled;
                }

                p = fbuf;
                char *fend = fbuf + flen;
                char *m;
                int linenum = 1;
                int colnum = 1;
                for (; p != fend; ++p)
                {
                    char *oldp = p;
                    if (re.TryMatch(p, fend, m))
                    {
                        printf(": Match at (%d,%d) with length %d\n", linenum, colnum, m - p);
                        if (repStyle != ReplaceDisabled)
                        {
                            bool doRep;
                            if (repStyle == ReplaceQuery)
                            {
                                printf("Replace this occurrence? ('Y' for yes)>");
                                if (getch() == 'Y')
                                {
                                    doRep = true;
                                }
                            }
                            if (doRep || repStyle == ReplaceAll)
                            {
                                Rep::String repstr;
                                rep.GetRepString(repstr);
                                printf(": Replaced by \"%s\"\n", repstr.c_str());
                                fprintf(fOut, "%s", repstr.c_str());
                                p = m - 1;
                                continue;
                            }
                        }
                    }
                    else
                    {
                        p = oldp;
                        if (fOut)
                        {
                            fputc(*p, fOut);
                        }
                    }

                    if (*p == '\n')
                    {
                        linenum++;
                        colnum = 1;
                    }
                    else if (*p == 0x0a)
                    {
                        if (p != fbuf, p[-1] == 0x0d)
                        {
                            linenum++;
                            colnum = 1;
                        }
                    }
                    else
                    {
                        colnum++;
                    }
                }
                if (fOut)
                {
                    printf(": Replaced text has been saved to file %s.\n", repfn.c_str());
                    fclose(fOut);
                }
            }
            break;
        }
    }
}

