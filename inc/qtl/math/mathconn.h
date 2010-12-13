/**
 * === Containers Library ===
 *  Unit Name:  autoptr
 *  Author:     Lincoln Yu
 *  Purpose:    implementation of auto pointer, a reference counting container
 * == History ==        
 *  2008-10-21: (dark) Created, a simple test on function creation 
 *              performed. Compilation by vc9 and g++ passed.
 *  2008-10-21: (light) chief input and return type of operations for the 
 *              core connectible pointer (MathConn) changed to the end type of 
 *              the derivation, to make this type the the working type of the system,
 *              roughly tested.
 *              ToString(_Rconn &sender) method for each object was implemented to facilitate debugging.
 *              Compiling passed on vc9 and g++.
 *  2008-10-22: Separate objects dealing with different operator calculations. operator nodes removed.
 *  2008-10-29: rename method PointTo as SetPointer
 *
 */ 

#ifndef _mathconn_H
#define _mathconn_H
#include <typeinfo>
#include <vector>
#include <cassert>
#include <string>
#include "../all/predef.h"
#include "../ctnr/autoptr.h"

_QTL_BEGIN
struct SampleMcTraits
{
    template <class Type>
    struct VecPack
    {
        typedef std::vector<Type>   Vector;
        typedef std::vector<Type>   ArgList;
    };
};

/* Math connectible core classes */

struct MathConnGlobal
{
    enum States
    {
        k_Unknown = -2,
        k_Invalid = -1,
        k_Normal = 0,
        k_Variable = 0,
        k_Fixed = 1,
    };
    enum OpType
    {
        k_Eq,
        k_Lt,
        k_Gt,
        k_LtEq,
        k_GtEq,
        k_Neg,
        k_Add,
        k_Sub,
        k_Mul,
        k_Div,
    };
	static std::string AddrToString(void *p)
	{
		char buf[32];
		sprintf(buf, "%08x", (unsigned long)p);
		return std::string(buf);
	}
	static std::string StateToString(int state)
	{
		std::string res;
		switch(state)
		{
		case k_Unknown:
			res = "Unknown";
			break;
		case k_Invalid:
			res = "Invalid";
			break;
		case k_Variable:
			res = "Variable";
			break;
		case k_Fixed:
			res = "Fixed";
			break;
		default:
			res = "Panic";
			break;
		}
		return res;
	}
};
template <class _Rconn, class _Traits> class MathConn;
template <class _Rconn, class _Traits> class MathConnInvalid;
/* operator related classes */
template <class _Rconn, class _Traits> class MathConnEqOp;
template <class _Rconn, class _Traits> class MathConnNEqOp;
template <class _Rconn, class _Traits> class MathConnLtOp;
template <class _Rconn, class _Traits> class MathConnGtOp;
template <class _Rconn, class _Traits> class MathConnLtEqOp;
template <class _Rconn, class _Traits> class MathConnGtEqOp;
template <class _Rconn, class _Traits> class MathConnAddOp;
template <class _Rconn, class _Traits> class MathConnSubOp;
template <class _Rconn, class _Traits> class MathConnMulOp;
template <class _Rconn, class _Traits> class MathConnDivOp;
template <class _Rconn, class _Traits> class MathConnNegOp;

template <class _Rconn, class _Traits = SampleMcTraits>
class MathConnBody
{
    typedef MathConnBody<_Rconn, _Traits>		_Myt;

protected:
    typedef MathConn<_Rconn, _Traits>           _Conn;
    typedef autoptr< _Myt >                     _BasePtr;
    typedef MathConnInvalid<_Rconn, _Traits>    _Invalid;

protected:
    virtual _Rconn ValEq(_Rconn senderVal, _Rconn thatVal)
    {
        return senderVal->LEq(senderVal, thatVal);
    }
    virtual _Rconn ValNEq(_Rconn senderVal, _Rconn thatVal)
    {
        return senderVal->LNEq(senderVal, thatVal);
    }
    virtual _Rconn ValLt(_Rconn senderVal, _Rconn thatVal)
    {
        return senderVal->LLt(senderVal, thatVal);
    }
    virtual _Rconn ValGt(_Rconn senderVal, _Rconn thatVal)
    {
        return senderVal->LGt(senderVal, thatVal);
    }
    virtual _Rconn ValLtEq(_Rconn senderVal, _Rconn thatVal)
    {
        return senderVal->LLtEq(senderVal, thatVal);
    }
    virtual _Rconn ValGtEq(_Rconn senderVal, _Rconn thatVal)
    {
        return senderVal->LGtEq(senderVal, thatVal);
    }
    virtual _Rconn ValAdd(_Rconn senderVal, _Rconn thatVal)
    {
        return senderVal->LAdd(senderVal, thatVal);
    }
    virtual _Rconn ValSub(_Rconn senderVal, _Rconn thatVal)
    {
        return senderVal->LSub(senderVal, thatVal);
    }
    virtual _Rconn ValMul(_Rconn senderVal, _Rconn thatVal)
    {
        return senderVal->LMul(senderVal, thatVal);
    }
    virtual _Rconn ValDiv(_Rconn senderVal, _Rconn thatVal)
    {
        return senderVal->LDiv(senderVal, thatVal);
    }
    virtual _Rconn ValNeg(_Rconn senderVal)
    {
        return senderVal->Neg(senderVal);
    }

public:
    typedef typename _Traits::template VecPack<_Rconn>::ArgList  ArgList;

    MathConnBody() {}
    virtual ~MathConnBody() {}

    virtual int GetState(_Rconn &sender)
    {
        return MathConnGlobal::k_Unknown; // not implemented
    }
    virtual bool Valid(_Rconn &sender)
    {
        return GetState(sender) >= 0;
    }
    virtual bool IsVariable(const _Rconn &sender)
    {
        return false;
    }
    virtual void SetAsVariable(_Rconn &sender, bool bVariable = true)
    {
    }
    virtual bool Locked(const _Rconn &sender)
    {
        return false;
    }
    virtual void Lock(_Rconn &sender, bool bLock)
    {
    }
    virtual _Rconn Value(const _Rconn &sender)
    {
        return _Rconn();
    }
    virtual void Update(const _Rconn &sender)
    {
    }
    virtual void LCopy(_Rconn &sender, const _Rconn &that)
    {
    }
    virtual void RCopy(const _Rconn &sender, _Rconn &that)
    {
    }
    virtual void LAssign(_Rconn &sender, const _Rconn &that)
    {
        // default behaviour of assignment
        sender._BasePtr::operator=(that);
    }
    virtual void RAssign(const _Rconn &sender, _Rconn &that)
    {   // in most cases, this function is unused.
        // for the common behaivour of assignment is simply referencing
        // then LAssign is enough to represent this operation, and the 
        // program should never get into this function
    }

    virtual _Rconn LEq(_Rconn &sender, _Rconn &that) 
    { 
        assert(&(*sender) == (_Myt*)this);
        _Rconn conn;
		sender.Update();
		that.Update();
        if (!sender.IsVariable() && !that.IsVariable())
        {
            conn = ValEq(sender.Value(), that.Value());
        }
        else
        {
            MathConnEqOp<_Rconn, _Traits> *pOp = new MathConnEqOp<_Rconn, _Traits>();
            pOp->Connect(sender, that);
            conn.SetPointer(pOp);
        }
        if (!conn)
        {   /* creating error */
            conn.SetPointer(new _Invalid());
        }
        return conn;
    }
    virtual _Rconn REq(_Rconn &sender, _Rconn &that) 
    { 
        return _Rconn(); 
    }
    virtual _Rconn LNEq(_Rconn &sender, _Rconn &that) 
    { 
        assert(&(*sender) == (_Myt*)this);
        _Rconn conn;
		sender.Update();
		that.Update();
        if (!sender.IsVariable() && !that.IsVariable())
        {
            conn = ValNEq(sender.Value(), that.Value());
        }
        else
        {
            MathConnNEqOp<_Rconn, _Traits> *pOp = new MathConnNEqOp<_Rconn, _Traits>();
            pOp->Connect(sender, that);
            conn.SetPointer(pOp);
        }
        if (!conn)
        {   /* creating error */
            conn.SetPointer(new _Invalid());
        }
        return conn;
    }
    virtual _Rconn RNEq(_Rconn &sender, _Rconn &that) 
    { 
        return _Rconn(); 
    }
    virtual _Rconn LLt(_Rconn &sender, _Rconn &that) 
    { 
        assert(&(*sender) == (_Myt*)this);
        _Rconn conn;
		sender.Update();
		that.Update();
        if (!sender.IsVariable() && !that.IsVariable())
        {
            conn = ValLt(sender.Value(), that.Value());
        }
        else
        {
            MathConnLtOp<_Rconn, _Traits> *pOp = new MathConnLtOp<_Rconn, _Traits>();
            pOp->Connect(sender, that);
            conn.SetPointer(pOp);
        }
        if (!conn)
        {   /* creating error */
            conn.SetPointer(new _Invalid());
        }
        return conn;
    }
    virtual _Rconn RLt(_Rconn &sender, _Rconn &that) 
    { 
        return _Rconn(); 
    }
    virtual _Rconn LGt(_Rconn &sender, _Rconn &that) 
    { 
        assert(&(*sender) == (_Myt*)this);
        _Rconn conn;
		sender.Update();
		that.Update();
        if (!sender.IsVariable() && !that.IsVariable())
        {
            conn = ValGt(sender.Value(), that.Value());
        }
        else
        {
            MathConnGtOp<_Rconn, _Traits> *pOp = new MathConnGtOp<_Rconn, _Traits>();
            pOp->Connect(sender, that);
            conn.SetPointer(pOp);
        }
        if (!conn)
        {   /* creating error */
            conn.SetPointer(new _Invalid());
        }
        return conn;
    }
    virtual _Rconn RGt(_Rconn &sender, _Rconn &that) 
    { 
        return _Rconn(); 
    }
    virtual _Rconn LLtEq(_Rconn &sender, _Rconn &that) 
    { 
        assert(&(*sender) == (_Myt*)this);
        _Rconn conn;
		sender.Update();
		that.Update();
        if (!sender.IsVariable() && !that.IsVariable())
        {
            conn = ValLtEq(sender.Value(), that.Value());
        }
        else
        {
            MathConnLtEqOp<_Rconn, _Traits> *pOp = new MathConnLtEqOp<_Rconn, _Traits>();
            pOp->Connect(sender, that);
            conn.SetPointer(pOp);
        }
        if (!conn)
        {   /* creating error */
            conn.SetPointer(new _Invalid());
        }
        return conn;
    }
    virtual _Rconn RLtEq(_Rconn &sender, _Rconn &that) 
    { 
        return _Rconn(); 
    }
    virtual _Rconn LGtEq(_Rconn &sender, _Rconn &that) 
    { 
        assert(&(*sender) == (_Myt*)this);
        _Rconn conn;
		sender.Update();
		that.Update();
        if (!sender.IsVariable() && !that.IsVariable())
        {
            conn = ValGtEq(sender.Value(), that.Value());
        }
        else
        {
            MathConnGtEqOp<_Rconn, _Traits> *pOp = new MathConnGtEqOp<_Rconn, _Traits>();
            pOp->Connect(sender, that);
            conn.SetPointer(pOp);
        }
        if (!conn)
        {   /* creating error */
            conn.SetPointer(new _Invalid());
        }
        return conn;
    }
    virtual _Rconn RGtEq(_Rconn &sender, _Rconn &that) 
    { 
        return _Rconn(); 
    }
    virtual _Rconn LAdd(_Rconn &sender, _Rconn &that) 
    { 
        assert(&(*sender) == (_Myt*)this);
        _Rconn conn;
		sender.Update();
		that.Update();
        if (!sender.IsVariable() && !that.IsVariable())
        {
            conn = ValAdd(sender.Value(), that.Value());
        }
        else
        {
            MathConnAddOp<_Rconn, _Traits> *pOp = new MathConnAddOp<_Rconn, _Traits>();
            pOp->Connect(sender, that);
            conn.SetPointer(pOp);
        }
        if (!conn)
        {   /* creating error */
            conn.SetPointer(new _Invalid());
        }
        return conn;
    }
    virtual _Rconn RAdd(_Rconn &sender, _Rconn &that) 
    { 
        return _Rconn(); 
    }
    virtual _Rconn LSub(_Rconn &sender, _Rconn &that) 
    { 
        assert(&(*sender) == (_Myt*)this);
        _Rconn conn;
		sender.Update();
		that.Update();
        if (!sender.IsVariable() && !that.IsVariable())
        {
            conn = ValSub(sender.Value(), that.Value());
        }
        else
        {
            MathConnSubOp<_Rconn, _Traits> *pOp = new MathConnSubOp<_Rconn, _Traits>();
            pOp->Connect(sender, that);
            conn.SetPointer(pOp);
        }
        if (!conn)
        {   /* creating error */
            conn.SetPointer(new _Invalid());
        }
        return conn;
    }
    virtual _Rconn RSub(_Rconn &sender, _Rconn &that) 
    { 
        return _Rconn(); 
    }
    virtual _Rconn LMul(_Rconn &sender, _Rconn &that) 
    { 
        assert(&(*sender) == (_Myt*)this);
        _Rconn conn;
		sender.Update();
		that.Update();
        if (!sender.IsVariable() && !that.IsVariable())
        {
            conn = ValMul(sender.Value(), that.Value());
        }
        else
        {
            MathConnMulOp<_Rconn, _Traits> *pOp = new MathConnMulOp<_Rconn, _Traits>();
            pOp->Connect(sender, that);
            conn.SetPointer(pOp);
        }
        if (!conn)
        {   /* creating error */
            conn.SetPointer(new _Invalid());
        }
        return conn;
    }
    virtual _Rconn RMul(_Rconn &sender, _Rconn &that) 
    { 
        return _Rconn(); 
    }
    virtual _Rconn LDiv(_Rconn &sender, _Rconn &that) 
    { 
        assert(&(*sender) == (_Myt*)this);
        _Rconn conn;
		sender.Update();
		that.Update();
        if (!sender.IsVariable() && !that.IsVariable())
        {
            conn = ValDiv(sender.Value(), that.Value());
        }
        else
        {
            MathConnDivOp<_Rconn, _Traits> *pOp = new MathConnDivOp<_Rconn, _Traits>();
            pOp->Connect(sender, that);
            conn.SetPointer(pOp);
        }
        if (!conn)
        {   /* creating error */
            conn.SetPointer(new _Invalid());
        }
        return conn;
    }
    virtual _Rconn RDiv(_Rconn &sender, _Rconn &that) 
    { 
        return _Rconn(); 
    }
    virtual _Rconn Neg(_Rconn &sender) 
    { 
        assert(&(*sender) == (_Myt*)this);
        _Rconn conn;
		sender.Update();
        if (!sender.IsVariable())
        {
            conn = ValNeg(sender.Value());
        }
        else
        {
            MathConnNegOp<_Rconn, _Traits> *pOp = new MathConnNegOp<_Rconn, _Traits>();
            pOp->Connect(sender);
            conn.SetPointer(pOp);
        }
        if (!conn)
        {   /* creating error */
            conn.SetPointer(new _Invalid());
        }
        return conn;
    }
    // flexible args assignment
    virtual _Rconn operator()(_Rconn &sender, ArgList &args)
    {
        return _Rconn();
    }
    // support up to 8 ordinary args
    // 1 arg
    virtual _Rconn operator()(_Rconn &sender, _Rconn &arg1)
    {
        return _Rconn();
    }
    // 2 args
    virtual _Rconn operator()(_Rconn &sender,
        _Rconn &arg1, _Rconn &arg2)
    {
        return _Rconn();
    }
    // 3 args
    virtual _Rconn operator()(_Rconn &sender,
        _Rconn &arg1, _Rconn &arg2, _Rconn &arg3)
    {
        return _Rconn();
    }
    // 4 args
    virtual _Rconn operator()(_Rconn &sender,
        _Rconn &arg1, _Rconn &arg2, _Rconn &arg3, _Rconn &arg4)
    {
        return _Rconn();
    }
    // 5 args
    virtual _Rconn operator()(_Rconn &sender,
        _Rconn &arg1, _Rconn &arg2, _Rconn &arg3, _Rconn &arg4,
        _Rconn &arg5)
    {
        return _Rconn();
    }
    // 6 args
    virtual _Rconn operator()(_Rconn &sender,
        _Rconn &arg1, _Rconn &arg2, _Rconn &arg3, _Rconn &arg4,
        _Rconn &arg5, _Rconn &arg6)
    {
        return _Rconn();
    }
    // 7 args
    virtual _Rconn operator()(_Rconn &sender,
        _Rconn &arg1, _Rconn &arg2, _Rconn &arg3, _Rconn &arg4,
        _Rconn &arg5, _Rconn &arg6, _Rconn &arg7)
    {
        return _Rconn();
    }
    // 8 args
    virtual _Rconn operator()(_Rconn &sender,
        _Rconn &arg1, _Rconn &arg2, _Rconn &arg3, _Rconn &arg4,
        _Rconn &arg5, _Rconn &arg6, _Rconn &arg7, _Rconn &arg8)
    {
        return _Rconn();
    }
    virtual std::string ToString(_Rconn &sender)
    {
        std::string res = "{ MathConnBody[";
		res += MathConnGlobal::AddrToString(this);
        res += "] }";
        return res;
    }
};  /* class MathConnBody */

template <class _Rconn, class _Traits = SampleMcTraits>
class MathConnInvalid : public MathConnBody<_Rconn, _Traits>
{
    typedef MathConnBody<_Rconn, _Traits>    _Mybase;
public:
    virtual int GetState()
    {
        return MathConnGlobal::k_Invalid;
    }
    
    virtual bool Valid()
    {
        return false;
    }
    virtual std::string ToString(_Rconn &sender)
    {
        std::string res = "{ MathConnInvalid[";
		res += MathConnGlobal::AddrToString(this);
        res += "] }";
        return res;
    }
};  /* class MathConnInvalid */

template <class _Rconn, class _Traits = SampleMcTraits>
class MathConn : public autoptr< MathConnBody<_Rconn, _Traits> >
{
    typedef autoptr< MathConnBody<_Rconn, _Traits> >	_Mybase;
    typedef MathConn<_Rconn, _Traits>					_Myt;
    typedef MathConnBody<_Rconn, _Traits>				_Body;

public:
    typedef typename _Body::ArgList						ArgList;
    typedef _Mybase										_BasePtr;

    MathConn() : _Mybase()
    {
    }
    MathConn(_Body *pBody) : _Mybase(pBody)
    {
    }
    ~MathConn()
    {
    }
    bool Attached()
    {
        return this->_Mybase::operator bool();
    }
    /* for debugging */
    std::string ToString()
    {
        if (*this)
        {
            return (*this)->ToString((_Rconn &)*this);
        }
        else
        {
            return "{ NULL }";
        }
    }
    /*
     * ATTENTION!
     * some of the following methods do not perform checking 
     * on the pointing validity of this math-connectible object 
     */
    int GetState()
    {
        return (*this)->GetState((_Rconn &)*this);
    }
    bool Valid()
    {
        return (*this)->Valid((_Rconn &)*this);
    }
    bool IsVariable() const
    {
        return (*this)->IsVariable((_Rconn &)*this);
    }
    void SetAsVariable(bool bVariable = true)
    {
        (*this)->SetAsVariable((_Rconn &)*this, bVariable);
    }
    bool Locked()
    {
        return (*this)->Locked((_Rconn &)*this);
    }
    void Lock(bool bLock)
    {
        (*this)->Lock((_Rconn &)*this, bLock);
    }
    _Rconn Value() const
    {
        return (*this)->Value((_Rconn &)*this);
    }
    void Update() const
    {
		return (*this)->Update((_Rconn &)*this);
    }
    /* operations */
    void SetPointer(_Body *p)
    {
        _Mybase::operator=(p);
    }
    _Rconn & operator=(const _Rconn &that)
    {
        if (!that)
        {
            _Mybase::operator=(NULL);
        }
        if (*this)
        {
            (*this)->LAssign((_Rconn &)*this, that);
        }
        else
        {   /* default manner of assignment */
            _Mybase::operator=(that);
        }
        return ((_Rconn &)*this);
    }
    _Body & operator*() const
    {
        return _Mybase::operator*();
    }
    _Body * operator->() const
    {
        return _Mybase::operator ->();
    }
    _Rconn operator==(_Rconn &that)
    {
        return (*this)->LEq((_Rconn &)*this, that);
    }
    _Rconn operator!=(_Rconn &that)
    {
        return (*this)->LNEq((_Rconn &)*this, that);
    }
    _Rconn operator<(_Rconn &that)
    {
        return (*this)->LLt((_Rconn &)*this, that);
    }
    _Rconn operator>(_Rconn &that)
    {
        return (*this)->LGt((_Rconn &)*this, that);
    }
    _Rconn operator<=(_Rconn &that)
    {
        return (*this)->LLtEq((_Rconn &)*this, that);
    }
    _Rconn operator>=(_Rconn &that)
    {
        return (*this)->LGtEq((_Rconn &)*this, that);
    }
    _Rconn operator-()
    {
        return (*this)->Neg((_Rconn &)*this);
    }
    _Rconn operator+(_Rconn &that)
    {
        return (*this)->LAdd((_Rconn &)*this, that);
    }
    _Rconn operator-(_Rconn &that)
    {
        return (*this)->LSub((_Rconn &)*this, that);
    }
    _Rconn operator*(_Rconn &that)
    {
        return (*this)->LMul((_Rconn &)*this, that);
    }
    _Rconn operator/(_Rconn &that)
    {
        return (*this)->LDiv((_Rconn &)*this, that);
    }
    _Rconn operator()(ArgList &args)
    {
        return (*this)->operator()((_Rconn &)*this, args);
    }
    _Rconn operator()(_Rconn arg1)
    {
        return (*this)->operator()((_Rconn &)*this, arg1);
    }
    _Rconn operator()(_Rconn arg1, _Rconn arg2)
    {
        return (*this)->operator()((_Rconn &)*this, arg1, arg2);
    }
    _Rconn operator()(_Rconn arg1, _Rconn arg2, _Rconn arg3)
    {
        return (*this)->operator()((_Rconn &)*this, arg1, arg2, arg3);
    }
    _Rconn operator()(_Rconn arg1, _Rconn arg2, _Rconn arg3, _Rconn arg4)
    {
        return (*this)->operator()((_Rconn &)*this, arg1, arg2, arg3, arg4);
    }
    _Rconn operator()(_Rconn arg1, _Rconn arg2, _Rconn arg3, _Rconn arg4,
        _Rconn arg5)
    {
        return (*this)->operator()((_Rconn &)*this, arg1, arg2, arg3, arg4,
            arg5);
    }
    _Rconn operator()(_Rconn arg1, _Rconn arg2, _Rconn arg3, _Rconn arg4,
        _Rconn arg5, _Rconn arg6)
    {
        return (*this)->operator()((_Rconn &)*this, arg1, arg2, arg3, arg4,
            arg5, arg6);
    }
    _Rconn operator()(_Rconn arg1, _Rconn arg2, _Rconn arg3, _Rconn arg4,
        _Rconn arg5, _Rconn arg6, _Rconn arg7)
    {
        return (*this)->operator()((_Rconn &)*this, arg1, arg2, arg3, arg4,
            arg5, arg6, arg7);
    }
    _Rconn operator()(_Rconn arg1, _Rconn arg2, _Rconn arg3, _Rconn arg4,
        _Rconn arg5, _Rconn arg6, _Rconn arg7, _Rconn arg8)
    {
        return (*this)->operator()((_Rconn &)*this, arg1, arg2, arg3, arg4,
            arg5, arg6, arg7, arg8);
    }
};  /* class MathConn */

template <class _Rconn, class _Traits = SampleMcTraits>
class MathConnOp : public MathConnBody<_Rconn, _Traits>
{
protected:
    _Rconn  m_Value;    // output value, it is a `leaf' rather than a `node'
    int     m_State;

public:
    virtual _Rconn Value(const _Rconn &sender)
    {
        return m_Value;
    }
    virtual int GetState(_Rconn &sender)
    {
        return m_State;
    }
    virtual bool IsVariable(const _Rconn &sender)
    {
        return (m_State == MathConnGlobal::k_Variable);
    }
	virtual std::string ToString(_Rconn &sender)
	{
		std::string res = "{ MathConnOp[";
		res += MathConnGlobal::AddrToString(this);
		res += "] m_Value( ";
		res += m_Value.ToString();
		res += " ), ";
		res += "m_State(";
		res += MathConnGlobal::StateToString(m_State);
		res += ") }";
		return res;
	}
};

template <class _Rconn, class _Traits = SampleMcTraits>
class MathConnUnaryOp: public MathConnOp <_Rconn, _Traits>
{
    typedef MathConnOp <_Rconn, _Traits>    _Mybase;

protected:
    typedef MathConnInvalid<_Rconn, _Traits>    _Invalid;

protected:
    _Rconn  m_Num0;
    
protected:
    virtual void Compute(_Rconn &val0) = 0;

public:
    virtual void Update(const _Rconn &sender)
    {
		if (!m_Num0)
		{
			return;
		}
		m_Num0.Update();
        _Rconn val0 = m_Num0.Value();
        bool bHavingValue = (bool)val0 && val0.Valid();
        bool bVariable = m_Num0.IsVariable();
        if (!bHavingValue)
        {
            _Mybase::m_Value.SetPointer(new _Invalid());
            _Mybase::m_State = MathConnGlobal::k_Invalid;
            // detach its subnodes
            m_Num0.SetPointer(NULL);
            return;
        }
        _Mybase::m_Value.SetPointer(NULL);
        Compute(val0);
        if (!_Mybase::m_Value)
        {   // error getting values
            _Mybase::m_Value.SetPointer(new _Invalid());
            _Mybase::m_State = MathConnGlobal::k_Invalid;
            // detach its subnodes
            m_Num0.SetPointer(NULL);
            return;
        }
        _Mybase::m_State = bVariable? MathConnGlobal::k_Variable : MathConnGlobal::k_Fixed;
        if (!bVariable && !Locked(sender))
        {
            m_Num0.SetPointer(NULL);
        }
    }
    virtual void Connect(_Rconn &num0)
    {
        m_Num0 = num0;
    }
};  /* class MathConnUnaryOp */

template <class _Rconn, class _Traits = SampleMcTraits>
class MathConnBinaryOp : public MathConnOp <_Rconn, _Traits>
{
    typedef MathConnOp <_Rconn, _Traits>    _Mybase;

protected:
    typedef MathConnInvalid<_Rconn, _Traits>    _Invalid;

protected:
    _Rconn  m_Num0;
    _Rconn  m_Num1;

protected:
    virtual void Compute(_Rconn &val0, _Rconn &val1) = 0;

public:
    virtual void Update(const _Rconn &sender)
    {
		if (!m_Num0 || !m_Num1)
		{
			return;
		}
		m_Num0.Update();
		m_Num1.Update();
        _Rconn val0 = m_Num0.Value();
        _Rconn val1 = m_Num1.Value();
        bool bHavingValue = (bool)val0 && val0.Valid()
            && (bool)val1 && val1.Valid();
        bool bVariable = m_Num0.IsVariable() || m_Num1.IsVariable();
        if (!bHavingValue)
        {
            _Mybase::m_Value.SetPointer(new _Invalid());
            _Mybase::m_State = MathConnGlobal::k_Invalid;
            // detach its subnodes
            m_Num0.SetPointer(NULL);
            m_Num1.SetPointer(NULL);
            return;
        }
        _Mybase::m_Value.SetPointer(NULL);
        Compute(val0, val1);
        if (!_Mybase::m_Value)
        {   // error getting values
            
            _Mybase::m_Value.SetPointer(new _Invalid());
            _Mybase::m_State = MathConnGlobal::k_Invalid;
            // detach its subnodes
            m_Num0.SetPointer(NULL);
            m_Num1.SetPointer(NULL);
            return;
        }
        _Mybase::m_State = bVariable? MathConnGlobal::k_Variable : MathConnGlobal::k_Fixed;
        if (!bVariable && !Locked(sender))
        {
            m_Num0.SetPointer(NULL);
            m_Num1.SetPointer(NULL);
			_Mybase::m_State = MathConnGlobal::k_Fixed;
        }
    }
    virtual void Connect(_Rconn &num0, _Rconn &num1)
    {
        m_Num0 = num0;
        m_Num1 = num1;
    }
};  /* class MathConnBinaryOp */

/* unary operators */
template <class _Rconn, class _Traits = SampleMcTraits>
class MathConnNegOp : public MathConnUnaryOp<_Rconn, _Traits>
{
	typedef MathConnUnaryOp<_Rconn, _Traits>	_Mybase;
protected:
    typedef MathConnOp<_Rconn, _Traits> _BaseOp;
    virtual void Compute(_Rconn &val0)
    {
        _BaseOp::m_Value = -val0;
    }
	virtual std::string ToString(_Rconn &sender)
	{
		std::string res = "{ -.[";
		res += MathConnGlobal::AddrToString(this);
		res += "] ";
		res += " -";
		res += _Mybase::m_Num0.ToString();
		res += " : ";
		res += _Mybase::ToString(sender);
		res += "}";
		return res;
	}
};

/* binary operators */
template <class _Rconn, class _Traits = SampleMcTraits>
class MathConnEqOp : public MathConnBinaryOp<_Rconn, _Traits>
{
	typedef MathConnBinaryOp<_Rconn, _Traits>	_Mybase;
protected:
    typedef MathConnOp<_Rconn, _Traits> _BaseOp;
    virtual void Compute(_Rconn &val0, _Rconn &val1)
    {
        _BaseOp::m_Value = val0 == val1;
    }
	virtual std::string ToString(_Rconn &sender)
	{
		std::string res = "{ ==[";
		res += MathConnGlobal::AddrToString(this);
		res += "] ";
		res += _Mybase::m_Num0.ToString();
		res += " == ";
		res += _Mybase::m_Num1.ToString();
		res += " : ";
		res += _Mybase::ToString(sender);
		res += "}";
		return res;
	}
};
template <class _Rconn, class _Traits = SampleMcTraits>
class MathConnNEqOp : public MathConnBinaryOp<_Rconn, _Traits>
{
	typedef MathConnBinaryOp<_Rconn, _Traits>	_Mybase;
protected:
    typedef MathConnOp<_Rconn, _Traits> _BaseOp;
    virtual void Compute(_Rconn &val0, _Rconn &val1)
    {
        _BaseOp::m_Value = val0 != val1;
    }
	virtual std::string ToString(_Rconn &sender)
	{
		std::string res = "{ !=[";
		res += MathConnGlobal::AddrToString(this);
		res += "] ";
		res += _Mybase::m_Num0.ToString();
		res += " != ";
		res += _Mybase::m_Num1.ToString();
		res += " : ";
		res += _Mybase::ToString(sender);
		res += "}";
		return res;
	}
};
template <class _Rconn, class _Traits = SampleMcTraits>
class MathConnLtOp : public MathConnBinaryOp<_Rconn, _Traits>
{
	typedef MathConnBinaryOp<_Rconn, _Traits>	_Mybase;
protected:
    typedef MathConnOp<_Rconn, _Traits> _BaseOp;
    virtual void Compute(_Rconn &val0, _Rconn &val1)
    {
        _BaseOp::m_Value = val0 < val1;
    }
	virtual std::string ToString(_Rconn &sender)
	{
		std::string res = "{ <[";
		res += MathConnGlobal::AddrToString(this);
		res += "] ";
		res += _Mybase::m_Num0.ToString();
		res += " < ";
		res += _Mybase::m_Num1.ToString();
		res += " : ";
		res += _Mybase::ToString(sender);
		res += "}";
		return res;
	}
};
template <class _Rconn, class _Traits = SampleMcTraits>
class MathConnGtOp : public MathConnBinaryOp<_Rconn, _Traits>
{
	typedef MathConnBinaryOp<_Rconn, _Traits>	_Mybase;
protected:
    typedef MathConnOp<_Rconn, _Traits> _BaseOp;
    virtual void Compute(_Rconn &val0, _Rconn &val1)
    {
        _BaseOp::m_Value = val0 > val1;
    }
	virtual std::string ToString(_Rconn &sender)
	{
		std::string res = "{ >[";
		res += MathConnGlobal::AddrToString(this);
		res += "] ";
		res += _Mybase::m_Num0.ToString();
		res += " > ";
		res += _Mybase::m_Num1.ToString();
		res += " : ";
		res += _Mybase::ToString(sender);
		res += "}";
		return res;
	}
};
template <class _Rconn, class _Traits = SampleMcTraits>
class MathConnLtEqOp : public MathConnBinaryOp<_Rconn, _Traits>
{
	typedef MathConnBinaryOp<_Rconn, _Traits>	_Mybase;
protected:
    typedef MathConnOp<_Rconn, _Traits> _BaseOp;
    virtual void Compute(_Rconn &val0, _Rconn &val1)
    {
        _BaseOp::m_Value = val0 <= val1;
    }
	virtual std::string ToString(_Rconn &sender)
	{
		std::string res = "{ <=[";
		res += MathConnGlobal::AddrToString(this);
		res += "] ";
		res += _Mybase::m_Num0.ToString();
		res += " <= ";
		res += _Mybase::m_Num1.ToString();
		res += " : ";
		res += _Mybase::ToString(sender);
		res += "}";
		return res;
	}
};
template <class _Rconn, class _Traits = SampleMcTraits>
class MathConnGtEqOp : public MathConnBinaryOp<_Rconn, _Traits>
{
	typedef MathConnBinaryOp<_Rconn, _Traits>	_Mybase;
protected:
    typedef MathConnOp<_Rconn, _Traits> _BaseOp;
    virtual void Compute(_Rconn &val0, _Rconn &val1)
    {
        _BaseOp::m_Value = val0 >= val1;
    }
	virtual std::string ToString(_Rconn &sender)
	{
		std::string res = "{ >=[";
		res += MathConnGlobal::AddrToString(this);
		res += "] ";
		res += _Mybase::m_Num0.ToString();
		res += " >= ";
		res += _Mybase::m_Num1.ToString();
		res += " : ";
		res += _Mybase::ToString(sender);
		res += "}";
		return res;
	}
};
template <class _Rconn, class _Traits = SampleMcTraits>
class MathConnAddOp : public MathConnBinaryOp<_Rconn, _Traits>
{
	typedef MathConnBinaryOp<_Rconn, _Traits>	_Mybase;
protected:
    typedef MathConnOp<_Rconn, _Traits> _BaseOp;
    virtual void Compute(_Rconn &val0, _Rconn &val1)
    {
        _BaseOp::m_Value = val0 + val1;
    }
	virtual std::string ToString(_Rconn &sender)
	{
		std::string res = "{ +[";
		res += MathConnGlobal::AddrToString(this);
		res += "] ";
		res += _Mybase::m_Num0.ToString();
		res += " + ";
		res += _Mybase::m_Num1.ToString();
		res += " : ";
		res += _Mybase::ToString(sender);
		res += "}";
		return res;
	}
};
template <class _Rconn, class _Traits = SampleMcTraits>
class MathConnSubOp : public MathConnBinaryOp<_Rconn, _Traits>
{
	typedef MathConnBinaryOp<_Rconn, _Traits>	_Mybase;
protected:
    typedef MathConnOp<_Rconn, _Traits> _BaseOp;
    virtual void Compute(_Rconn &val0, _Rconn &val1)
    {
        _BaseOp::m_Value = val0 - val1;
    }
	virtual std::string ToString(_Rconn &sender)
	{
		std::string res = "{ -[";
		res += MathConnGlobal::AddrToString(this);
		res += "] ";
		res += _Mybase::m_Num0.ToString();
		res += " - ";
		res += _Mybase::m_Num1.ToString();
		res += " : ";
		res += _Mybase::ToString(sender);
		res += "}";
		return res;
	}
};
template <class _Rconn, class _Traits = SampleMcTraits>
class MathConnMulOp : public MathConnBinaryOp<_Rconn, _Traits>
{
	typedef MathConnBinaryOp<_Rconn, _Traits>	_Mybase;
protected:
    typedef MathConnOp<_Rconn, _Traits> _BaseOp;
    virtual void Compute(_Rconn &val0, _Rconn &val1)
    {
        _BaseOp::m_Value = val0 * val1;
    }
	virtual std::string ToString(_Rconn &sender)
	{
		std::string res = "{ *[";
		res += MathConnGlobal::AddrToString(this);
		res += "] ";
		res += _Mybase::m_Num0.ToString();
		res += " * ";
		res += _Mybase::m_Num1.ToString();
		res += " : ";
		res += _Mybase::ToString(sender);
		res += "}";
		return res;
	}
};
template <class _Rconn, class _Traits = SampleMcTraits>
class MathConnDivOp : public MathConnBinaryOp<_Rconn, _Traits>
{
	typedef MathConnBinaryOp<_Rconn, _Traits>	_Mybase;
protected:
    typedef MathConnOp<_Rconn, _Traits> _BaseOp;
    virtual void Compute(_Rconn &val0, _Rconn &val1)
    {
        _BaseOp::m_Value = val0 / val1;
    }
	virtual std::string ToString(_Rconn &sender)
	{
		std::string res = "{ /[";
		res += MathConnGlobal::AddrToString(this);
		res += "] ";
		res += _Mybase::m_Num0.ToString();
		res += " / ";
		res += _Mybase::m_Num1.ToString();
		res += " : ";
		res += _Mybase::ToString(sender);
		res += "}";
		return res;
	}
};

template <class _Rconn, class _Traits = SampleMcTraits>
class MathConnLeaf : public MathConnBody<_Rconn, _Traits>
{
    typedef MathConnBody<_Rconn, _Traits>                _Mybase;
    typedef MathConnLeaf<_Rconn, _Traits>                  _Myt;
    typedef typename MathConn<_Rconn, _Traits>::_BasePtr   _BasePtr;
public:
    MathConnLeaf() : m_State(MathConnGlobal::k_Variable)
    {
    }
    MathConnLeaf(bool bVariable)
    {
        m_State = bVariable? MathConnGlobal::k_Variable : MathConnGlobal::k_Fixed;
    }
    virtual _Rconn Value(const _Rconn &sender)
    {
        if (IsVariable(sender))
        {
            _Rconn res;
            sender->RCopy(sender, res);
            res->SetAsVariable(res, false);
            return res;
        }
        else
        {
            return sender;
        }
    }
    virtual bool IsVariable(const _Rconn &sender)
    {
        return (m_State == MathConnGlobal::k_Variable);
    }
    virtual void SetAsVariable(_Rconn &sender, bool bVariable = true)
    {
        if (m_State == MathConnGlobal::k_Invalid)
        {
            return;
        }
        
        if (bVariable)
        {
            m_State = MathConnGlobal::k_Variable;
        }
        else
        {
            m_State = MathConnGlobal::k_Fixed;
        }
    }
    virtual void LAssign(_Rconn &sender, const _Rconn &that)
    {
        assert(&(*sender) == (_Mybase*)this);
		if (!that)
		{
			sender.SetPointer(NULL);
			return;
		}
		that.Update();
        if (m_State == MathConnGlobal::k_Variable)
        {
            // value setting
            _Rconn thatVal = that.Value();
            if (thatVal)
            {
                LCopy(sender, thatVal);
            }
            else
            {   // left unchanged
            }
        }
        else if (m_State == MathConnGlobal::k_Fixed && that.IsVariable())
        {
            // value setting
            _Rconn thatVal = that.Value();
            if (thatVal)
            {
                _Myt *pNew = new _Myt(false);   // as fixed
                sender.SetPointer(pNew); 
                /* a bit hapharzard, this object may be destroyed here */
                pNew->LCopy(sender, thatVal);  
            }
            else
            {   // left unchanged
            }
        }
        else
        {   // neither is variable
            sender._BasePtr::operator=(that);  // this may be destroyed here
        }
    }
    virtual int GetState(_Rconn &sender)
    {
        return m_State;
    }
    virtual std::string ToString(_Rconn &sender)
    {
        std::string strState = "state(";
		strState += MathConnGlobal::StateToString(m_State);
        strState += ")";
        std::string res = "{ MathConnLeaf[";
		res += MathConnGlobal::AddrToString(this);
        res += "] " + strState + " }";
        return res;
    }
protected:
    int m_State;
};  /* class MathConnLeaf */

template <class _Real , class _Traits> class McReal;

template <class _Real = double, class _Traits = SampleMcTraits>
class McRealLeaf : public MathConnLeaf< McReal<_Real, _Traits>, _Traits >
{
public:
    typedef McReal<_Real, _Traits>          _Rconn;

private:
    typedef McRealLeaf<_Real, _Traits>      _Myt;

public:
    typedef MathConnLeaf<_Rconn, _Traits>      _Mybase;
    typedef MathConnBody<_Rconn, _Traits>    _Body;
    typedef MathConnInvalid<_Rconn, _Traits>   _Invalid;

public:
    typedef _Real   Real;
    McRealLeaf(Real r = 0, bool bVariable = true) : m_Real(r), _Mybase(bVariable)
    {
    }
    bool GetBool()
    {
        return (m_Real != Real(0));
    }
    Real GetReal()
    {
        return m_Real;
    }
    void SetReal(Real r)
    {
        m_Real = r;
    }
    bool IsPositive()
    {
        return (m_Real > Real(0));
    }
    bool IsNegative()
    {
        return (m_Real < Real(0));
    }
    bool IsZero()
    {
        return (m_Real == Real(0));
    }

    virtual void LCopy(_Rconn &sender, const _Rconn &that)
    {
        if (!that)
        {
            return;	// possibly an error
        }
        assert(&(*sender) == (_Body*)this);
        bool bVariable = sender.IsVariable();
        if (typeid(*that) == typeid(*this))
        {
            memcpy(this, &(*that), sizeof(*this));
        }
        else
        {
            that->RCopy(that, sender);
        }
        SetAsVariable(sender, bVariable);
    }
    virtual void RCopy(const _Rconn &sender, _Rconn &that)
    {
        assert(&(*sender) == (_Body*)this);
        _Myt *pNew = new _Myt();
        memcpy(pNew, this, sizeof(*this));
        that.SetPointer((_Body *)pNew);
    }

protected:
    virtual _Rconn ValEq(_Rconn senderVal, _Rconn thatVal)
    {
        _Rconn conn;
        if (typeid(*thatVal) == typeid(*this))
        {   /* i can handle that */
            Real rval = (Real)(m_Real == ((_Myt &)*thatVal).m_Real);
            conn.SetPointer(new _Myt(rval, false));
        }
        else if(thatVal)
        {
            conn = thatVal->REq(thatVal, senderVal);
        }
        return conn;
    }
    virtual _Rconn ValNEq(_Rconn senderVal, _Rconn thatVal)
    {
        _Rconn conn;
        if (typeid(*thatVal) == typeid(*this))
        {   /* i can handle that */
            Real rval = (Real)(m_Real != ((_Myt &)*thatVal).m_Real);
            conn.SetPointer(new _Myt(rval, false));
        }
        else if(thatVal)
        {
            conn = thatVal->RNEq(thatVal, senderVal);
        }
        return conn;
    }
    virtual _Rconn ValLt(_Rconn senderVal, _Rconn thatVal)
    {
        _Rconn conn;
        if (typeid(*thatVal) == typeid(*this))
        {   /* i can handle that */
            Real rval = (Real)(m_Real < ((_Myt &)*thatVal).m_Real);
            conn.SetPointer(new _Myt(rval, false));
        }
        else if(thatVal)
        {
            conn = thatVal->RLt(thatVal, senderVal);
        }
        return conn;
    }
    virtual _Rconn ValGt(_Rconn senderVal, _Rconn thatVal)
    {
        _Rconn conn;
        if (typeid(*thatVal) == typeid(*this))
        {   /* i can handle that */
            Real rval = (Real)(m_Real > ((_Myt &)*thatVal).m_Real);
            conn.SetPointer(new _Myt(rval, false));
        }
        else if(thatVal)
        {
            conn = thatVal->RGt(thatVal, senderVal);
        }
        return conn;
    }
    virtual _Rconn ValLtEq(_Rconn senderVal, _Rconn thatVal)
    {
        _Rconn conn;
        if (typeid(*thatVal) == typeid(*this))
        {   /* i can handle that */
            Real rval = (Real)(m_Real <= ((_Myt &)*thatVal).m_Real);
            conn.SetPointer(new _Myt(rval, false));
        }
        else if(thatVal)
        {
            conn = thatVal->RLtEq(thatVal, senderVal);
        }
        return conn;
    }
    virtual _Rconn ValGtEq(_Rconn senderVal, _Rconn thatVal)
    {
        _Rconn conn;
        if (typeid(*thatVal) == typeid(*this))
        {   /* i can handle that */
            Real rval = (Real)(m_Real >= ((_Myt &)*thatVal).m_Real);
            conn.SetPointer(new _Myt(rval, false));
        }
        else if(thatVal)
        {
            conn = thatVal->RGtEq(thatVal, senderVal);
        }
        return conn;
    }
    virtual _Rconn ValAdd(_Rconn senderVal, _Rconn thatVal)
    {
        _Rconn conn;
        if (typeid(*thatVal) == typeid(*this))
        {   /* i can handle that */
            Real rval = (Real)(m_Real + ((_Myt &)*thatVal).m_Real);
            conn.SetPointer(new _Myt(rval, false));
        }
        else if(thatVal)
        {
            conn = thatVal->RAdd(thatVal, senderVal);
        }
        return conn;
    }
    virtual _Rconn ValSub(_Rconn senderVal, _Rconn thatVal)
    {
        _Rconn conn;
        if (typeid(*thatVal) == typeid(*this))
        {   /* i can handle that */
            Real rval = (Real)(m_Real - ((_Myt &)*thatVal).m_Real);
            conn.SetPointer(new _Myt(rval, false));
        }
        else if(thatVal)
        {
            conn = thatVal->RSub(thatVal, senderVal);
        }
        return conn;
    }
    virtual _Rconn ValMul(_Rconn senderVal, _Rconn thatVal)
    {
        _Rconn conn;
        if (typeid(*thatVal) == typeid(*this))
        {   /* i can handle that */
            Real rval = (Real)(m_Real * ((_Myt &)*thatVal).m_Real);
            conn.SetPointer(new _Myt(rval, false));
        }
        else if(thatVal)
        {
            conn = thatVal->RMul(thatVal, senderVal);
        }
        return conn;
    }
    virtual _Rconn ValDiv(_Rconn senderVal, _Rconn thatVal)
    {
        _Rconn conn;
        if (typeid(*thatVal) == typeid(*this))
        {   /* i can handle that */
            Real rval = (Real)(m_Real / ((_Myt &)*thatVal).m_Real);
            conn.SetPointer(new _Myt(rval, false));
        }
        else if(thatVal)
        {
            conn = thatVal->RDiv(thatVal, senderVal);
        }
        return conn;
    }
	virtual _Rconn ValNeg(_Rconn senderVal)
	{
		_Rconn conn;
		conn.SetPointer((_Body *)new _Myt(-m_Real, false));
		return conn;
	}
    virtual std::string ToString(_Rconn &sender)
    {
        std::string strReal = "value(";
        char buf[32];
        sprintf(buf, "%f", m_Real);
        strReal += buf;
        strReal += ")";
        std::string res = "{ McRealLeaf[";
		res += "]  " + strReal + " : " + _Mybase::ToString(sender) + " }";
        return res;
    }

protected:
    Real    m_Real;
};  /* class McRealLeaf */

template <class _Real = double, class _Traits = SampleMcTraits>
class McReal : public MathConn<McReal<_Real, _Traits>, _Traits>
{
    typedef McReal<_Real, _Traits>          _Myt;
    typedef MathConn<_Myt, _Traits>         _Mybase;

protected:
    typedef McRealLeaf<_Real, _Traits>      _RealLeaf;
    typedef MathConnBody<_Myt, _Traits>		_Body;

public:
    typedef typename _Mybase::_BasePtr      _BasePtr;
    typedef _Real                           Real;

    McReal() : _Mybase()
    {   // not associated with anything, as hung-up
    }
    McReal(Real value, bool bVariable = false) : _Mybase((_Body*)new _RealLeaf(value))
    {   // by default the connectible is created with a leaf to serve as a pointer
        _Mybase::SetAsVariable(bVariable);
    }
    //MATHCONN_OVERLOADS
    Real GetReal()
    {
		_Mybase::Update();
        _Myt val = _Mybase::Value();
        if (typeid(*val) == typeid(_RealLeaf))
        {
            return ((_RealLeaf&)(*val)).GetReal();
        }
        return Real(0);
    }
    bool GetBool()
    {
		_Mybase::Update();
        _Myt val = _Mybase::Value();
        if (typeid(*val) == typeid(_RealLeaf))
        {
            return (*val).GetBool();
        }
        return false;
    }
    _Myt & operator=(Real rval)
    {
		if (_Mybase::Attached())
		{
			_Mybase::Update();
		}
        if (_Mybase::Attached() && _Mybase::IsVariable() && typeid(*(*this)) == typeid(_RealLeaf))
        {
            // this provides the most signifcant feature this math-connectible system
            // can support -- "variable assignment from outside"
            ((_RealLeaf&)*(*this)).SetReal(rval);
        }
        else
        {
            // redirect to the real value, current attachment will be lost
            _Body *pLeaf = (_Body *)(new _RealLeaf(rval));
            _BasePtr::operator=(pLeaf);
        }
        return (*this);
    }
    void Detach()
    {
        _Mybase::SetPointer(NULL);
    }
};

_QTL_END

#endif  /* ifndef _mathconn_H */

