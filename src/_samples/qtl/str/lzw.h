/**
 * === String Library ===
 *  Unit Name:  lzw
 *  Author:     Lincoln Yu
 *  Purpose:    LZW compression algorithm
 *  Reference:  http://en.wikipedia.org/wiki/LZW 
 * == History ==        
 *  2007-11-14: Created and debugged.
 *				
 * == Comments ==
 *
*/

#ifndef _lzw_H
#define _lzw_H

#include <vector>
#include <map>


namespace qtl {


template <class _Symbol = unsigned char>
    struct LzwSymbols
{
    std::vector<_Symbol>    data;

    _Symbol & operator[] (int index) { return data[index]; }
    const _Symbol & operator[] (int index) const
    {
        return data[index];
    }

    int size () { return data.size(); }

    bool operator< (const LzwSymbols &other) const
    {
        int thisSize = data.size(), thatSize = other.data.size();
        int minLen = (thisSize < thatSize)? thisSize : thatSize;
        for (int i = 0; i < minLen; i++)
        {
            if (data[i] < other.data[i]) { return true; }
            else if (data[i] > other.data[i]) { return false; }
        }
        return (thisSize < thatSize);
    }

    void clear ()
    {
        data.clear();
    }

    void pop_back ()
    {
        data.pop_back();
    }

    LzwSymbols & operator+= (_Symbol symbol)
    {
        data.push_back(symbol);
        return (*this);
    }

    LzwSymbols & operator= (_Symbol symbol)
    {
        data.resize(1);
        data[0] = symbol;
        return (*this);
    }

    void print () const
    {   // for debug purpose only
        for (int i = 0; i < data.size(); i++) { printf("%c", data[i]); }
    }
};

template <class _Symbol = unsigned char, class _DictEntry = unsigned short,
        class _Symbols = LzwSymbols<_Symbol> >
	struct lzw_core
{
	lzw_core (int symbolCount  =256, int dictSize = 4096)
        : m_SymbolCount(symbolCount), m_MaxDictSize(dictSize)
	{
        m_CurDictIdx = m_SymbolCount;
	}

    void		set_symbol_count (_DictEntry count) { m_SymbolCount = count; }
    _DictEntry  get_symbol_count () const			{ return m_SymbolCount; }
    void		set_max_dict_size (_DictEntry size) { m_MaxDictSize = size; }
    _DictEntry  get_max_dict_size () const			{ return m_MaxDictSize; }

    void reset ()
    {
        m_W.clear();
        m_CurDictIdx = m_SymbolCount;
    }

    _Symbols        m_W;
    _DictEntry      m_MaxDictSize;
    _DictEntry		m_SymbolCount;
    _DictEntry      m_CurDictIdx;
};	/* class lzw_core */


template <class _Symbol = unsigned char, class _DictEntry = unsigned short,
        class _Symbols = LzwSymbols<_Symbol>, 
        class _MapIdxToEntry = std::vector<_Symbols> >
    class lzw_decoder
{
protected:
	typedef lzw_core<_Symbol, _DictEntry, _Symbols>		_LzwCore;

public:
    void reset ()
    {
        m_MapIdxToEntry.clear();
    }

    void decode_one (_LzwCore &core, const _DictEntry &input, _Symbols &symbols)
    {
        /* See Annotation 1 */
        if (input < core.m_SymbolCount) 
        { 
            _Symbol c = (_Symbol)input;
            symbols = c;
        }
        else 
		{
			_DictEntry	entry = input - core.m_SymbolCount;
			if (entry < m_MapIdxToEntry.size())
			{
				symbols = m_MapIdxToEntry[entry];
			}
			else
			{	// not found in the dictionary
				symbols = core.m_W;
				symbols += core.m_W[0];
			}
		}

        _Symbols &wc = (core.m_W += symbols[0]);	/* w + entry[0] */

        if (wc.size() > 1)
        {
			if (core.m_CurDictIdx >= m_MapIdxToEntry.size())
			{
				m_MapIdxToEntry.push_back(wc);
			}
			else
			{
				m_MapIdxToEntry[core.m_CurDictIdx] = wc;
			}

			core.m_CurDictIdx++;
			// exceeding checking
            if (core.m_CurDictIdx >= core.m_MaxDictSize)
            {
                core.m_CurDictIdx = (core.m_MaxDictSize + core.m_SymbolCount) / 2;   // always overwriting from this place
            }
        }

        core.m_W = symbols;
    }

protected:
    _MapIdxToEntry  m_MapIdxToEntry;

};  /* lzw_decoder */


template <class _Symbol = unsigned char, class _DictEntry = unsigned short,
        class _Symbols = LzwSymbols<_Symbol>, 
        class _MapEntryToIdx = std::map<_Symbols, _DictEntry> >
    class lzw_encoder
{
protected:
	typedef lzw_core<_Symbol, _DictEntry, _Symbols>		_LzwCore;

public:
    void reset ()
    {
        m_MapEntryToIdx.clear();
	}

    bool encode_one (_LzwCore &core, const _Symbol &c, _DictEntry &output)
    {
        _Symbols &wc = (core.m_W += c);
        if (wc.size() > 1 && m_MapEntryToIdx.find(wc) == m_MapEntryToIdx.end())
        {   // nonchar word not found in dictionary
            m_MapEntryToIdx[wc] = core.m_CurDictIdx++;

            _Symbols &w = core.m_W; core.m_W.pop_back();

            /*
             * Annotation 1: 
             * we see clearly the necessity and the possiblity provided here 
             * of implementing an explicit conversion method with necessary 
             * encapsulation of the symbol object for symbol set which does 
             * not use a common mapping scheme from itself to dictionary 
             * entries and the outer program does not perform the transformation.
             */
            if (w.size() == 1) { output = (_DictEntry)w[0]; }
            else { output = m_MapEntryToIdx[w]; }

            // exceeding checking
            if (core.m_CurDictIdx >= core.m_MaxDictSize)
            {
                core.m_CurDictIdx = (core.m_MaxDictSize + core.m_SymbolCount) / 2;   // always overwriting from this place
            }

            core.m_W = c;
            return true;
        }
        else
        {
            return false;
        }
    }

    void encode_flush (_LzwCore &core, _DictEntry &output)
    {
        output = m_MapEntryToIdx[core.m_W];
    }

protected:
    _MapEntryToIdx  m_MapEntryToIdx;

};  /* class lzw_encoder */


}	/* namespace qtl */

#endif  /* ifndef _lzw_H */

