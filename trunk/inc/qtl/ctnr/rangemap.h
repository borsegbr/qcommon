#ifndef _rangemap_H
#define _rangemap_H

#include "../all/predef.h"

#include <list>
#include <vector>

_QTL_BEGIN

template <class Ts, class Td = Ts>
struct RangeMapTraits
{
    typedef const Ts &  TsCref;
    typedef const Td &  TdCref;
    typedef Ts &        TsRef;
    typedef Td &        TdRef;

    template <class Rs, class Rd>
    struct TypeWrap
    {
        typedef std::list<Rs>   ListS;
        typedef std::list<Rd>   ListD;

        typedef ListS   &LsRef;
        typedef ListD   &LdRef;
        typedef typename ListS::iterator    LsItr;
        typedef typename ListD::iterator    LdItr;
        typedef const Rs &  RsCref;
        typedef const Rd &  RdCref;

        static LsItr RemovePair (LsRef ls, LdRef ld, LsItr is, LdItr id)
        {
            LsItr isNext = is;
            ++isNext;
            ls.erase(is);
            ld.erase(id);
            return isNext;
        }

        static void InsertPair (LsRef ls, LdRef ld, LsItr is, LdItr id,
            RsCref rs, RdCref rd)
        {
            LsItr iNewS = ls.insert(is, rs);
            LdItr iNewD = ld.insert(id, rd);
            iNewS->obj = iNewD;
            iNewD->obj = iNewS;
        }

        static LsItr GetBeginS (LsRef ls, TsCref sIni, TsCref tEnd)
        {
            return ls.begin();
        }

        static LdItr GetBeginD (LdRef ld, TdCref dIni, TdCref dEnd)
        {
            return ld.begin();
        }

        static LsItr GetEndS (LsRef ls, TsCref sIni, TsCref tEnd)
        {
            return ls.end();
        }

        static LdItr GetEndD (LdRef ld, TdCref dIni, TdCref dEnd)
        {
            return ld.end();
        }
    };

    typedef std::vector<Ts> OutVecS;
    typedef std::vector<Td> OutVecD;


    static bool Conform (TsCref sIni1, TsCref sEnd1, TdCref dIni1, TdCref dEnd1,
        bool rev1, TsCref sIni2, TsCref sEnd2, TdCref dIni2, TdCref dEnd2, bool rev2)
    {
        Ts ds1 = sEnd1 - sIni1;
        Td dd1 = dEnd1 - dIni1;

        Ts ds2 = sEnd2 - sIni2;
        Td dd2 = dEnd2 - dIni2;

        if (rev1 != rev2)
        {
            return false;
        }

        if (dd1*ds2!=dd2*ds1)
        {
            return false;
        }

        if (sIni1 > sEnd2 || sIni2 > sEnd1 || dIni1 > dEnd2 || dIni2 > dEnd1)
        {   // the new one and the old one have no intersection
            return false;
        }

        Ts ds21;
        Td dd21;
        if (rev1)
        {
            if (sIni2 > sIni1)
            {
                ds21 = sEnd2 - sIni1;
                dd21 = dEnd1 - dIni2;
            }
            else
            {
                ds21 = sEnd1 - sIni2;
                dd21 = dEnd2 - dIni1;
            }
        }
        else
        {
            if (sIni2 > sIni1)
            {
                ds21 = sEnd2 - sIni1;
                dd21 = dEnd2 - dIni1;
            }
            else
            {
                ds21 = sEnd1 - sIni2;
                dd21 = dEnd1 - dIni2;
            }
        }

        return (dd1*ds21==ds1*dd21);
    }

    static bool DoMapSToD (TsCref sIni, TsCref sEnd, TdCref dIni, TdCref dEnd,
        bool rev, TsCref s, TdRef d)
    {
        if (rev)
        {
            d = (s - sIni) * (dIni - dEnd) / (sEnd - sIni) + dEnd;
        }
        else
        {
            d = (s - sIni) * (dEnd - dIni) / (sEnd - sIni) + dIni;
        }
        return true;
    }

    static bool DoMapDToS (TdCref dIni, TdCref dEnd, TsCref sIni, TsCref sEnd,
        bool rev, TdCref d, TsRef s)
    {
        if (rev)
        {
            s = (d - dIni) * (sIni - sEnd) / (dEnd - dIni) + sEnd;
        }
        else
        {
            s = (d - dIni) * (sEnd - sIni) / (dEnd - dIni) + sIni;
        }
        return true;
    }

    static void OutVecSAdd (OutVecS &ovs, TsCref s)
    {
        ovs.push_back(s);
    }

    static void OutVecDAdd (OutVecD &ovd, TdCref d)
    {
        ovd.push_back(d);
    }
};

/* Any to any mapping */
template <class Ts, class Td = Ts, class Traits=RangeMapTraits<Ts, Td> >
struct SegRangeMap
{
    typedef const Ts &  TsCref;
    typedef const Td &  TdCref;
    typedef Ts &        TsRef;
    typedef Td &        TdRef;

    struct RangeS;
    struct RangeD;

    typedef typename Traits::template TypeWrap<RangeS, RangeD>  TypeWrap;
    typedef typename TypeWrap::ListS    ListS;
    typedef typename TypeWrap::ListD    ListD;

    typedef typename Traits::OutVecS    OutVecS;
    typedef typename Traits::OutVecD    OutVecD;

    typedef typename ListS::iterator    LsItr;
    typedef typename ListD::iterator    LdItr;

    struct RangeS
    {
        Ts      ini;
        Ts      end;
        bool    rev;
        LdItr   obj;

        RangeS (TsCref i, TsCref e, bool r = false) 
            : ini(i), end(e), rev(r), obj(0) {}
    };

    struct RangeD
    {
        Td      ini;
        Td      end;
        LsItr   obj;

        RangeD (TdCref i, TdCref e) : ini(i), end(e), obj(0) {}
    };

    template <class T>
    void Swap (T &a, T &b) { T t = a; a = b; b = t; }

	void Clear ()
	{
		listS.clear();
		listD.clear();
	}

    void AddMap (Ts sIni, Ts sEnd, Td dIni, Td dEnd)
    {
        LsItr   itr; 
        LsItr   itrIns = listS.end();
        bool    rev;

        if (sIni <= sEnd && dIni <= dEnd)
        {
            rev = false;
        }
        else if (sIni >= sEnd && dIni >= dEnd)
        {
            Swap(sIni, sEnd); Swap(dIni, dEnd);
            rev = false;
        }
        else
        {
            rev = true;
            if (sIni > sEnd) Swap(sIni, sEnd);
            if (dIni > dEnd) Swap(dIni, dEnd);
        }

        for (itr = TypeWrap::GetBeginS(listS, sIni, sEnd); itr != listS.end(); )
        {
            if (Traits::Conform(itr->ini, itr->end, itr->obj->ini, 
                itr->obj->end, itr->rev, sIni, sEnd, dIni, dEnd, rev))
            {
                if (sIni > itr->ini) sIni = itr->ini;
                if (sEnd < itr->end) sEnd = itr->end;
                if (dIni > itr->ini) dIni = itr->obj->ini;
                if (dEnd < itr->end) dEnd = itr->obj->end;

                itr = TypeWrap::RemovePair(listS, listD, itr, itr->obj);
            }
            else if (itr->ini > sIni || itr->ini == sIni && itr->end > sEnd)
            {
                itrIns = itr;
                ++itr;
            }
            else
            {
                ++itr;
            }
        }

        LdItr  itrD;
        for (itrD = TypeWrap::GetBeginD(listD, dIni, dEnd); itrD != listD.end(); ++itrD)
        {
            if (itrD->ini > dIni || itrD->ini == dIni && itrD->end > dEnd)
            {
                break;
            }
        }

        TypeWrap::InsertPair(listS, listD, itrIns, itrD, RangeS(sIni, sEnd, rev), RangeD(dIni, dEnd));

    }

    bool TrySToD (const RangeS &r, TsCref s, TdRef d)
    {
        if (s >= r.ini && s < r.end)
        {
            Traits::DoMapSToD(r.ini, r.end, r.obj->ini, 
                r.obj->end, r.rev, s, d);
            return true;
        }
        return false;
    }

    bool MapSToD (TsCref s, OutVecD &ds)
    {
        LsItr   itr;
        bool    found = false;
        ds.clear();
        for (itr = TypeWrap::GetBeginS(listS, s, s); itr != TypeWrap::GetEndS(listS, s, s); ++itr)
        {
            Td d;
            if (TrySToD(*itr, s, d))
            {
                Traits::OutVecDAdd(ds, d);
                found = true;
            }
            else if (s < itr->ini)
            {
                break;
            }
        }
        return found;
    }

    bool TryDToS (const RangeD &r, TdCref d, TsRef s)
    {
        if (d >= r.ini && d < r.end)
        {
            Traits::DoMapDToS(r.ini, r.end, r.obj->ini, 
                r.obj->end, r.obj->rev, d, s);
            return true;
        }
        return false;
    }

    bool MapDToS (TdCref d, OutVecS &ss)
    {
        LdItr   itr;
        bool    found = false;
        ss.clear();
        for (itr = TypeWrap::GetBeginD(listD, d, d); itr != TypeWrap::GetEndD(listD, d, d); ++itr)
        {
            Ts s;
            if (TryDToS(*itr, d, s))
            {
                Traits::OutVecSAdd(ss, s);
                found = true;
            }
            else if (d < itr->ini)
            {
                break;
            }
        }
        return found;
    }

protected:
    ListS   listS;
    ListD   listD;
};

_QTL_END

#if defined(TEST_rangemap) && TEST_rangemap

#include <cstdio>

int main (void)
{
    SegRangeMap<int>            m;
    SegRangeMap<int>::OutVecD   ov;
    SegRangeMap<int>::OutVecS   os;

    m.AddMap(1,3,5,7);
    m.AddMap(2,7,11,6);
    m.AddMap(6,9,10,13);

	printf("Test Src to Dest...\n");
    if(m.MapSToD(6, ov))
    {
        for (int i = 0; i < ov.size(); ++i)
        {
            printf("%d ", ov[i]);
        }
        printf("\n");
    }
    else
    {
        printf("Unmapped\n");
    }

	printf("Test Dest to Src...\n");
    if(m.MapDToS(10, os))
    {
        for (int i = 0; i < os.size(); ++i)
        {
            printf("%d ", os[i]);
        }
        printf("\n");
    }
    else
    {
        printf("Unmapped\n");
    }

    return 0;
}

#endif

#endif	/* ifndef _rangemap_H */

