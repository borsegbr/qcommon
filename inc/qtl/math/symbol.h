/**
 * === QTL Common Files ===
 *  Unit Name:  symbol
 *  Author:     Lincoln Yu
 *  Purpose:    Mathematical calculation symbol
 * == History ==        
 *  2006-10-06: Created.
 * == Comments ==
 *
 */


#if !defined (_symbol_H)
#define _symbol_H	200610160200

#include "../all/predef.h"

#include <vector>
#include <string>
#include <map>
#include <hash_map>


_QTL_BEGIN


using namespace std;

class symbol
	{
public:
	virtual string get_tag ()
		{
		return "NO_TAG";
		}

	virtual string & conclude ()
		{
		return (_M_expr = make_expr());
		}

	virtual string make_expr ()
		{
		string _expr = get_tag();
		_expr += "(";
		int _num_args = _M_args.size();
		if (_num_args > 0)
			{
			_expr += _M_args[0]->make_expr();
			for (int i = 1; i < _num_args; ++i)
				{
				_expr += ",";
				_expr += _M_args[0]->make_expr();
				}
			}
		_expr += ")";
		return (_expr);
		}

	virtual symbol * create ()
		{
		return new symbol();
		}

private:
	vector<symbol *>	_M_args;
	string				_M_expr;

	};	// class symbol


class symbol_registry
	{
private:
	friend class symbol_parser;

	typedef map<string, symbol *>	_SymMap;

public:
	symbol_registry ()
		{
		}

	~symbol_registry ()
		{
		_SymMap::iterator _itr;

		for (_itr = _M_syms.begin(); _itr != _M_syms.end(); ++_itr)
			{
			delete (_itr->second);
			}
		}

	template <class _SymbolType> 
		void insert (string _tag)
		{
		_SymbolType _temp;
		_M_sym[_tag] = _temp.create();
		}

	void insert (string _tag, symbol *_target)
		{
		_M_syms[_tag] = _target;
		}

	symbol * create (string _tag)
		{
		return _M_syms[_tag]->create();
		}

private:
	_SymMap			_M_syms;
	};

class symbol_parser
	{
	string parse (symbol *_p_sym)
		{
		return "";
		}

private:
	symbol_registry		_M_sym_reg;		// registered symbols

	};


_QTL_END

#endif	// if !defined (_symbol_H)