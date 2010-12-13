/*
 * === Script ===
 *  Unit Name:  parse_cfg
 *  Author:     quanben
 *  Purpose:    Simple configuration file parser
 *              Supporting elementary types such as
 *              string, integer, double etc.
 * == History ==
 *  2008-11-10: Created.
 *
 */


#ifndef _parse_config_H
#define _parse_config_H

#include <string>
#include <vector>


#include "../all/predef.h"

_QUL_BEGIN


struct CfgMngrSampleStream
{
    FILE *fp;

    CfgMngrSampleStream() : fp(NULL) {}
    CfgMngrSampleStream(FILE *f) : fp(f) {}

    bool getline(std::string &line)
    {
        int i;
        char buf[256];
        bool nonempty;

        do
        {
            char *pGets = fgets(buf, 256, fp);
            if (pGets == NULL)
            {
                return false;
            }

            line = buf;

            int n = line.size();
            if (n == 0)
            {
                return false;
            }

            nonempty = false;
            for (i = 0; i < n; i++)
            {
                if (line[i] != ' ' && line[i] != '\t' 
                    && line[i] != 0xa && line[i] != 0xd)
                {
                    nonempty = true;
                    break;
                }
            }
        } while (!nonempty);

        for (i = line.size()-1; i >= 0; i--)
        {
            if (line[i] != 0xa && line[i] != 0xd)
            {
                i++;
                break;
            }
        }
        line = line.substr(0, i);
        return true;
    }

    void putline(const std::string &line)
    {
        fprintf(fp, "%s\n", line.c_str());
    }
};

template <class IStream = CfgMngrSampleStream, 
          class OStream = CfgMngrSampleStream>
class CfgMngr
{
public:
    typedef std::string String;

    enum Type
    {
        k_Real,
        k_Int,
        k_String
    };

    struct Field
    {
        std::string name;
        Type        type;
        double      rVal;
        int         iVal;
        String      sVal;
    };

protected:
    bool IsBlank(char c);
    bool ParseAs(Type type, const String &fn, const String &s, 
        Field &f, bool bTryType);

public:
    bool Load(IStream in);
    void Save(OStream out) const;
    Field * Retrieve(const String &fn);
    Field * Add(const String &fn);
    bool Delete(const String &fn);

protected:
    std::vector<Field>  m_Fields;
};

template <class IStream, class OStream>
bool CfgMngr<IStream, OStream>::IsBlank(char c)
{
    return (c == ' ' || c == '\t');
}

template <class IStream, class OStream>
bool CfgMngr<IStream, OStream>::ParseAs(Type type, const String &fn, 
    const String &s, Field &f, bool bTryType)
{
    int i;
    int n = s.length();
    f.name = fn;

    if (type == k_Int)
    {
        for (i = 0; i < n && IsBlank(s[i]); i++);

        if (i == n || !(s[i] >= '0' && s[i] <= '9'))
        {
            if (!bTryType)
            {
                return false;
            }
            else
            {
                type = k_String;
            }
        }
    }

    if (type == k_Int)
    {
        int val = 0;
        const int k_Range = 0x7fffffff / 10;
        bool bExceeding = false;
        for ( ; i < n; i++)
        {
            if (s[i] >= '0' && s[i] <= '9')
            {
                if (val > k_Range)
                {
                    bExceeding = true;
                    break;
                }
                val *= 10;
                val += s[i] - '0';
            }
            else
            {
                for ( ; i < n && IsBlank(s[i]); i++);
                break;
            }
        }
        
        if (i != n || bExceeding)
        {
            if (!bTryType)
            {
                return false;
            }
            else if((i < n && s[i] == '.') || val > k_Range)
            {
                type = k_Real;
            }
            else
            {
                type = k_String;
            }
        }
        else
        {
            f.type = k_Int;
            f.iVal = val;

            return true;
        }
    }

    if (type == k_Real)
    {
        // CAUTION:
        // TODO:
        // no range checking
        for (i = 0; i < n && IsBlank(s[i]); i++);

        double val = 0;
        int nDec = -1;
        
        for ( ; i < n; i++)
        {
            if (s[i] == '.')
            {
                if (nDec >= 0)
                {
                    break;
                }
                nDec = 0;
            }
            else if (s[i] >= '0' && s[i] <= '9')
            {
                if (nDec >= 0)
                {
                    nDec++;
                }
                val *= 10;
                val += s[i] - '0';
            }
            else
            {
                break;
            }
        }
        for ( ; nDec > 0; nDec--)
        {
            val /= 10;
        }
        f.rVal = val;
        f.type = k_Real;

        return true;
    }

    if (type == k_String)
    {
        f.type = k_String;
        if (n == 0)
        {
            f.sVal = "";
            return true;
        }
        // read from the second character if the first is blank
        if (IsBlank(s[0]))
        {
            f.sVal = s.substr(1);
        }
        else
        {
            f.sVal = s;
        }

        return true;
    }
}

template <class IStream, class OStream>
bool CfgMngr<IStream, OStream>::Load(IStream in)
{
    String str;
    String strFieldName;
    m_Fields.clear();
    while (in.getline(str))
    {
        int i, iStart;
        int n = str.size();
        Type type = k_Int;

        // skip blanks
        for (i = 0; i < n && IsBlank(str[i]); i++);
        if (i == n) 
        {
            return false;
        }

        if (str[i] == '/')
        {
            continue;
        }

        iStart = i;
        for ( ; i < n; i++)
        {
            if (str[i] == ':' || str[i] == '=' || IsBlank(str[i]))
            {
                break;
            }
        }

        strFieldName = str.substr(iStart, i - iStart);

        // skip blanks
        for ( ; i < n && IsBlank(str[i]); i++);

        bool bTryType = false;
        if (str[i] == ':')
        {
            i++;    // pass the ':'
            // skip blanks
            for ( ; i < n && IsBlank(str[i]); i++);
            if (i == n) 
            {
                return false;
            }

            iStart = i;
            for ( ; i < n && !IsBlank(str[i]) && str[i] != '='; i++);

            String strTypeName = str.substr(iStart, i - iStart);

            if (strTypeName == "Int")
            {
                type = k_Int;
            }
            else if (strTypeName == "Real")
            {
                type = k_Real;
            }
            else if (strTypeName == "String")
            {
                type = k_String;
            }
            else
            {   // invalid type, take as string
                type = k_String;
            }

            for ( ; i < n && str[i] != '='; i++);
            if (i == n)
            {
                return false;
            }
        }
        else if (str[i] == '=')
        {
            bTryType = true;
            type = k_Int;   // try integer first
        }
        else
        {
            return false;
        }

        i++;    // pass the '='
                // all characters after '=' are counted in the parsing
        // skip blanks
        Field f;
        String s = str.substr(i, n);
        if (!ParseAs(type, strFieldName, s, f, bTryType))
        {
            return false;
        }

        m_Fields.push_back(f);
    }

}

template <class IStream, class OStream>
void CfgMngr<IStream, OStream>::Save(OStream out) const
{
    int n = m_Fields.size();
    int nMaxNameLen = 0;
    for (int i = 0; i < n; i++)
    {
        const Field &f = m_Fields[i];
        if (f.name.size() > nMaxNameLen)
        {
            nMaxNameLen = f.name.size();
        }
    }

    nMaxNameLen += 1;
    nMaxNameLen += 3;
    nMaxNameLen /= 4;
    nMaxNameLen *= 4;

    for (int i = 0; i < n; i++)
    {
        String str;
        const Field &f = m_Fields[i];
        str = m_Fields[i].name;
        int nStr = str.size();
        while (nStr < nMaxNameLen)
        {
            str += ' '; nStr++;
        }

        str += "= ";

        if (f.type == k_Int)
        {
            char buf[32];
            sprintf(buf, "%d", f.iVal);
            str += buf;
        }
        else if (f.type == k_Real)
        {
            char buf[64];
            sprintf(buf, "%f", f.rVal);
            str += buf;
        }
        else if (f.type == k_String)
        {
            str += f.sVal;
        }

        out.putline(str);
    }
}

template <class IStream, class OStream>
typename CfgMngr<IStream, OStream>::Field * 
    CfgMngr<IStream, OStream>::Retrieve(const String &fn)
{

    int n = m_Fields.size();
    for (int i = 0; i < n; i++)
    {
        if (m_Fields[i].name == fn)
        {
            return &m_Fields[i];
        }
    }
    return NULL;
}

template <class IStream, class OStream>
typename CfgMngr<IStream, OStream>::Field * 
    CfgMngr<IStream, OStream>::Add(const String &fn)
{
    Field *pF = Retrieve(fn);
    if (pF)
    {
        return pF;
    }
    Field f;
    f.name = fn;
    m_Fields.push_back(f);
    return &m_Fields[m_Fields.size()-1];
}

template <class IStream, class OStream>
bool CfgMngr<IStream, OStream>::Delete(const String &fn)
{
    int i;
    int n = m_Fields.size();
    for (i = 0; i < n; i++)
    {
        if (m_Fields[i].name == fn)
        {
            m_Fields.erase(m_Fields.begin() + i);
            return true;
        }
    }
    return false;
}


_QUL_END


#endif  /* ifndef _parse_config_H */
