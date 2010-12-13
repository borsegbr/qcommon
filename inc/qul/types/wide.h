/**
 * === QUL Types ===
 *  Unit Name:  Wide
 *  Author:     Lincoln Yu
 *  Purpose:	Structure holding signed 64 
 * == History ==        
 *	2006-02-16:	Created.
 *
 */

#ifndef _wide_H
#define _wide_H

#include "../all/predef.h"
#include "../all/types.h"


_QUL_BEGIN

struct WideBase
	{

	virtual SInt32 &Hi() = 0;
	virtual UInt32 &Lo() = 0;
	virtual const SInt32 &Hi() const = 0;
	virtual const UInt32 &Lo() const = 0;

	void neg ()
		{
		Lo() = ~Lo()+1;			// -Lo()
		Hi() = ~Hi();
		if (Lo() == 0)
			Hi()++;
		}

	void operator= (UInt32 val)
		{
		Lo() = val, Hi() = 0;
		}
	operator UInt32 () const
		{
		if (Hi() < 0) return 0;
		if (Hi() > 0) return (UInt32)(-1);
		return Lo();
		}
	void operator+= (UInt32 right)
		{
		if (Lo() > (UInt32)(-1) - right)
			Hi()++;
		Lo() += right;
		}
	void operator-= (UInt32 right)
		{
		if (Lo() < right)
			Hi()--;
		Lo() -= right;
		}
	bool operator< (UInt32 right) const
		{
		return (Hi() < 0 || Hi() == 0 && Lo() < right);
		}
	bool operator> (UInt32 right) const
		{
		return (Hi() > 0 || Hi() == 0 && Lo() > right);
		}
	bool operator<= (UInt32 right) const
		{
		return (Hi() < 0 || Hi() == 0 && Lo() <= right);
		}
	bool operator>= (UInt32 right) const
		{
		return (Hi() > 0 || Hi() == 0 && Lo() >= right);
		}
	bool operator== (UInt32 right) const
		{
		return (Hi() == 0 && Lo() == right);
		}


	void operator= (SInt32 val)
		{
		Hi() = (val >= 0)? 0:-1;
		Lo() = (UInt32)val;
		}
	operator SInt32 () const
		{
		if (Hi() < -1 || Hi() == -1 && Lo() < (1<<31))
			return (SInt32)(1<<31);
		if (Hi() > 0 || Hi() == 0 && Lo() > ~(1<<31))
			return (SInt32)~(1<<31);
		return (SInt32)Lo();
		}	
	void operator+= (SInt32 right)
		{
		if (right > 0)
			{
			if (Lo() > (UInt32)(-1) - (UInt32)right)
				Hi()++;
			}
		else
			{
			if (Lo() < (UInt32)(-right))
				Hi()--;
			}
		Lo() += right;
		}
	void operator-= (SInt32 right)
		{
		if (right < 0)
			{
			if (Lo() > (UInt32)(-1) + (UInt32)right)
				Hi()++;
			}
		else
			{
			if (Lo() < (UInt32)right)
				Hi()--;
			}
		Lo() -= right;
		}
	bool operator< (SInt32 right) const
		{
		return (right < 0)?
			(Hi() < -1 || Hi() == -1 && Lo() < (UInt32)right) : 
			(Hi() < 0 || Hi() == 0 && Lo() < (UInt32)right);
		}
	bool operator> (SInt32 right) const
		{
		return (right < 0)?
			(Hi() > -1 || Hi() == -1 && Lo() > (UInt32)right) :
			(Hi() > 0 || Hi() == 0 && Lo() > (UInt32)right);
		}
	bool operator<= (SInt32 right) const
		{
		return (right < 0)?
			(Hi() < -1 || Hi() == -1 && Lo() <= (UInt32)right) : 
			(Hi() < 0 || Hi() == 0 && Lo() <= (UInt32)right);
		}
	bool operator>= (SInt32 right) const
		{
		return (right < 0)?
			(Hi() > -1 || Hi() == -1 && Lo() >= (UInt32)right) :
			(Hi() > 0 || Hi() == 0 && Lo() >= (UInt32)right);
		}
	bool operator== (SInt32 right) const
		{
		return (Hi() == ((right<0)?-1:0) && Lo() == right);
		}

	void operator= (int val)
		{
		return (*this = (SInt32)val);
		}
	operator int () const
		{
		return (SInt32)(*this);
		}
	void operator+= (int right)
		{
		return (*this += (SInt32)right);
		}
	void operator-= (int right)
		{
		return (*this -= (SInt32)right);
		}
	bool operator< (int right) const
		{
		return (*this < (SInt32)right);
		}
	bool operator> (int right) const
		{
		return (*this > (SInt32)right);
		}
	bool operator<= (int right) const
		{
		return (*this <= (SInt32)right);
		}
	bool operator>= (int right) const
		{
		return (*this >= (SInt32)right);
		}
	bool operator== (int right) const
		{
		return (*this == (SInt32)right);
		}

	template <class _Wide>
		void operator= (const _Wide &val)
		{
		Hi() = val.Hi();
		Lo() = val.Lo();
		}
	template <class _Wide>
		void operator+= (const _Wide &right)
		{
		if (Lo() > (UInt32)(-1) - right.Lo())
			Hi()++;
		Lo() += right.Lo();
		Hi() += right.Hi();
		}
	template <class _Wide>
		void operator-= (const _Wide &right)
		{
		if (Lo() < right.Lo())
			Hi()--;
		Lo() -= right.Lo();
		Hi() -= right.Hi();
		}
	template <class _Wide>
		bool operator< (const _Wide &right) const
		{
		return (Hi() < right.Hi() || Hi() == right.Hi() && Lo() < right.Lo());
		}
	template <class _Wide>
		bool operator> (const _Wide &right) const
		{
		return (Hi() > right.Hi() || Hi() == right.Hi() && Lo() > right.Lo());
		}
	template <class _Wide>
		bool operator<= (const _Wide &right) const
		{
		return (Hi() < right.Hi() || Hi() == right.Hi() && Lo() <= right.Lo());
		}
	template <class _Wide>
		bool operator>= (const _Wide &right) const
		{
		return (Hi() > right.Hi() || Hi() == right.Hi() && Lo() >= right.Lo());
		}
	template <class _Wide>
		bool operator== (const _Wide &right) const
		{
		return (Hi() == right.Hi() && Lo() == right.Lo());
		}

	};	// class WideBase


struct Wide : public WideBase
	{

	Wide() : hi(0),lo(0)
		{
		}
	Wide(SInt32 val)
		{
		(*this) = val;
		}
	Wide(int val)
		{
		(*this) = val;
		}
	Wide(UInt32 val)
		{
		(*this) = val;
		}
	template <class _Wide>
		Wide(const _Wide &val)
		{
		(*this) = val;
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

	Wide operator- () const
		{
		Wide temp = *this;
		temp.neg();
		return temp;
		}

	Wide operator= (SInt32 right)
		{
		WideBase::operator=(right);
		return (*this);
		}
	Wide operator+= (SInt32 right)
		{
		WideBase::operator+=(right);
		return (*this);
		}
	Wide operator-= (SInt32 right)
		{
		WideBase::operator-=(right);
		return (*this);
		}
	Wide operator+ (SInt32 right) const
		{
		Wide temp = *this;
		temp += right;
		return temp;
		}
	Wide operator- (SInt32 right) const
		{
		Wide temp = *this;
		temp -= right;
		return temp;
		}

	Wide operator= (UInt32 right)
		{
		WideBase::operator=(right);
		return (*this);
		}
	Wide operator+= (UInt32 right)
		{
		WideBase::operator+=(right);
		return (*this);
		}
	Wide operator-= (UInt32 right)
		{
		WideBase::operator-=(right);
		return (*this);
		}
	Wide operator+ (UInt32 right) const
		{
		Wide temp = *this;
		temp += right;
		return temp;
		}
	Wide operator- (UInt32 right) const
		{
		Wide temp = *this;
		temp -= right;
		return temp;
		}

	template <class _Wide>
		Wide operator= (const _Wide &right)
		{
		WideBase::operator=(right);
		return (*this);
		}
	template <class _Wide>
		Wide operator+= (const _Wide &right)
		{
		WideBase::operator+=(right);
		return (*this);
		}
	template <class _Wide>
		Wide operator-= (const _Wide &right)
		{
		WideBase::operator-=(right);
		return (*this);
		}
	template <class _Wide>
		Wide operator+ (const _Wide &right) const
		{
		Wide temp = *this;
		temp += right;
		return temp;
		}
	template <class _Wide>
		Wide operator- (const _Wide &right) const
		{
		Wide temp = *this;
		temp -= right;
		return temp;
		}

	Wide operator= (int right)
		{
		WideBase::operator=(right);
		return (*this);
		}
	Wide operator+= (int right)
		{
		WideBase::operator+=(right);
		return (*this);
		}
	Wide operator-= (int right)
		{
		WideBase::operator-=(right);
		return (*this);
		}
	Wide operator+ (int right) const
		{
		return (*this + (SInt32)right);
		}
	Wide operator- (int right) const
		{
		return (*this - (SInt32)right);
		}

	SInt32	hi;
	UInt32	lo;

	};	// struct Wide;


_QUL_END

#endif	// ifndef _wide_H
