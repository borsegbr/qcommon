#include <cmath>

#include "qtl/math/mathconn.h"


using namespace qtl;

typedef double      Real;
typedef McReal<>    MyReal;

struct Xrng
{
    Real low;
    Real high;

    Xrng(Real l, Real h)
    {
        low = l;
        high = h;
    }
};

Real Intg(MyReal x, MyReal a, Xrng xrng, MyReal func)
{
    Real res = 0.0;
    Real dx = 0.001;
    for (Real rx = xrng.low; rx < xrng.high; rx+=dx)
    {
        x = rx;

        MyReal y = func;
        Real vy = y.GetReal();
        res += vy * dx;
    }
    return res;
}


struct SampleExpMaxTraits
{
	typedef McReal<>                McVar;
	typedef McRealLeaf<>			McRLeaf;
	typedef MathConnBody<McVar>		McBody;
	typedef MathConnOp<McVar>		McOp;
	typedef MathConnUnaryOp<McVar>	McUnary;
	typedef MathConnBinaryOp<McVar>	McBinary;
	typedef McVar::Real				Real;

	struct McLog : public McUnary
	{
        McLog()
        {
        }

        McLog(McVar &x)
        {
            McUnary::m_Num0 = x;
        }

        virtual void Compute(McVar &val0)
        {
            Real r = ((McRLeaf&)*val0).GetReal();
            Real y = log(r);
            McOp::m_Value.SetPointer(new McRLeaf(y, false));
        }

        virtual McVar operator()(McVar &sender, McVar &x)
        {
            assert(&(*sender) == (McBody*)this);

            McVar res;
            res.SetPointer(new McLog(x));
            return res;
        }

        virtual std::string ToString(McVar &sender)
        {
            assert(&(*sender) == (McBody*)this);
            sender.Update();
            Real r = ((McRLeaf&)*McOp::m_Value).GetReal();
            char buf[32];
            sprintf(buf, "{ McLog m_Real(%f) }", r);
            return buf;
        }
	};
};

void LocalTest()
{
	MyReal  x;  // x now is uninitialized, cannot be used in creating expression
	x = 0.0;    // assigning after defination makes x a variable
	printf("x = %s\n\n", x.ToString().c_str());

	MyReal a = 3;
	MyReal c = 1.5;
	printf("c = %s\n\n", c.ToString().c_str());
	MyReal b = a / c; // a and b are fixed items

	Real vx = 4;
	x = vx;

	printf("a = %s\n\n", a.ToString().c_str());
	printf("c = %s\n\n", c.ToString().c_str());
	printf("b = %s\n\n", b.ToString().c_str());
	printf("x = %s\n\n", x.ToString().c_str());

	MyReal z =  x + x + x;
	printf("z = %s\n\n", z.ToString().c_str());
	MyReal t = z + x;
	printf("t = %s\n\n", t.ToString().c_str());
	return;

	MyReal y = a * x * x + b;

	x = 4;
	return;

	printf("y = %s\n", y.ToString().c_str());

	printf(": y(%f) = %f\n", vx, y.GetReal());
}

void LocalTestIntg()
{
	MyReal  x;  // x now is uninitialized, cannot be used in creating expression
	x = 0.0;    // assigning after defination makes x a variable

	// 3 * x^2 + 2
	// x^3 + 2 * x
	// (27+6) - (1+2) = 30
	MyReal a = 3;
	MyReal c = 1.5;
	MyReal b = a / c; // a and b are fixed items
	Real rInt = Intg(x, a, Xrng(1, 3), a * x * x + b);

	printf(": The result of the integral is %f\n", rInt);
}

void LocalTestLog()
{
	SampleExpMaxTraits::McVar x;
	x = 2;
	SampleExpMaxTraits::McVar Log;
	Log.SetPointer(new SampleExpMaxTraits::McLog());
	SampleExpMaxTraits::McVar y = Log(x*x);

	x = 5;

	printf("y = %s\n", y.ToString().c_str());
	printf("y = %f\n", y.GetReal());
	printf("log(25) = %f\n", log((double)5*5));
}

int main()
{
	//LocalTest();
	//LocalTestIntg();
	LocalTestLog();
	return 0;
}

