#define DEBUG_REGEXP    1
#include <qtl/str/regexp_replacer.h>

using namespace qtl;

int testcase_001(char *pattern = NULL, char *text = NULL)
{
    typedef RegExp<char*, char*>    RE;
    RE  re;

    if (pattern == NULL)
    {
        pattern = "a(?<a>b(?<a>k)*b\\k<a>)*c\\k<a>d";   /* a(?<a>b(?<a>k)*b\k<a>)*s\k<a>d */
    }
    
    char *p = pattern;
    bool bSp = re.SetPattern(p, pattern + strlen(pattern));

    if (!bSp)
    {
        printf("! error setting pattern at %d.\n", p - pattern);
        return 0;
    }

    re.DebugView();
    printf("\n");

    if (text == NULL)
    {
        text = "abkbkbkkbkcbkkbkdt";
    }

    bool bMatched = re.TryMatch(text, text + strlen(pattern), p);
    if (!bMatched)
    {
        printf(": Unmatched.\n");
        return 0;
    }

    printf(": Match ends at %d.\n", p - text);

    char *tbegin, *tend;
    char *pp = "<a>";
    if (re.GetShortcut(tbegin, tend, pp, pp + strlen(pp)))
    {
        printf(": <a> = ");
        for (char *tt = tbegin; tt < tend; tt++)
        {
            printf("%c", *tt);
        }
        printf("\n");
    }

    return 0;
}

/* searching for the specified pattern within the text */
int testcase_002()
{
    typedef RegExp<char*, char*>    RE;
    RE  re;
    char *pattern = "(?<1>.)(?<2>\\k<1>)";
    char *p = pattern;
    bool bSp = re.SetPattern(p, pattern + strlen(pattern));

    if (!bSp)
    {
        printf("! error setting pattern at %d.\n", p - pattern);
        return 0;
    }

    re.DebugView();
    printf("\n");

    char *text = "11kljl ll\n\nfkgjdlsf sfsdll\nfdf";
    char *pt = text;

    for ( ; pt < text + strlen(text); ++pt)
    {
        bool bMatched = re.TryMatch(pt, text + strlen(pattern), p);
        if (!bMatched)
        {
            continue;
        }

        printf(": Match ends at %d.\n", p - text);

        char *tbegin, *tend;
        char *pp = "<1>";
        if (re.GetShortcut(tbegin, tend, pp, pp + strlen(pp)))
        {
            printf(": <1> = ");
            for (char *tt = tbegin; tt < tend; tt++)
            {
                printf("%02x ", *tt);
            }
            printf("\n");
        }
        pp = "<2>";
        if (re.GetShortcut(tbegin, tend, pp, pp + strlen(pp)))
        {
            printf(": <2> = ");
            for (char *tt = tbegin; tt < tend; tt++)
            {
                printf("%02x ", *tt);
            }
            printf("\n");
        }
    }

    return 0;
}

int main()
{
    testcase_001("a(b*|d*|c*)*c", "abbdcbddccdc");
    //testcase_001();
    //testcase_002();

    return 0;
}
