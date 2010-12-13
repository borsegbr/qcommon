/**
 * === QuickTime Utility ===
 *  Unit Name:  DataTypes
 *  Author:     Lincoln Yu
 *  Purpose:	QuickTime data type enhancement
 * == History ==        
 *	2006-02-12:	Created.
 *	2006-02-20: Copy constructor for [:wide:] type added.
 *
 */

#ifndef _DataTypes_H
#define _DataTypes_H

#include "./QTC_Def.h"
#include "../../types/wide.h"


struct QT_Wide : public wide, public utils::WideBase
	{

	QT_Wide ()
		{
		hi = lo = 0;
		}

	QT_Wide (const wide &copy)
		{
		hi = copy.hi;
		lo = copy.lo;
		}
	template <class _Wide>
		QT_Wide(const _Wide &copy)
		{
		hi = copy.Hi();
		lo = copy.Lo();
		}
	template <class _Wide>
		QT_Wide &operator= (const _Wide &copy)
		{
		hi = copy.Hi();
		lo = copy.Lo();
		return (*this);
		}

	QT_Wide &operator= (const wide &copy)
		{
		hi = copy.hi;
		lo = copy.lo;
		return (*this);
		}

	QT_Wide &operator= (int copy)
		{
		utils::WideBase::operator=(copy);
		return (*this);
		}

	operator wide() const
		{
		wide temp;
		temp.hi = hi;
		temp.lo = lo;
		return temp;
		}

	virtual SInt32 &Hi()
		{
		return hi;
		}
	virtual UInt32 &Lo()
		{
		return lo;
		}
	virtual const SInt32 &Hi() const
		{
		return hi;
		}
	virtual const UInt32 &Lo() const
		{
		return lo;
		}

	template <class _Wide>
		QT_Wide operator+= (const _Wide &right)
		{
		utils::WideBase::operator+=(right);
		return (*this);
		}
	template <class _Wide>
		QT_Wide operator-= (const _Wide &right)
		{
		utils::WideBase::operator-=(right);
		return (*this);
		}
	template <class _Wide>
		QT_Wide operator+ (const _Wide &right) const
		{
		QT_Wide temp = *this;
		temp += right;
		return temp;
		}

	template <class _Wide>
		QT_Wide operator- (const _Wide &right) const
		{
		QT_Wide temp = *this;
		temp -= right;
		return temp;
		}

	};

template <class _Src, class _Dst>
_Dst ConvertWide (const _Src &right)
	{
	_Dst temp;
	temp.hi = right.hi;
	temp.lo = right.lo;
	return temp;
	}


#endif	// ifndef _DataTypes_H