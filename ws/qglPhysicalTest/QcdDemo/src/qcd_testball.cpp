#include "qcd_testball.h"


/* class TestBall */

void TestBall::VirtualClash (Object *that, Time &t)
{
    //assert(typeid(*that) == typeid(TestBall));
    TestBall *pball = dynamic_cast<TestBall *>(that);
	assert(pball);
    
    // equation (quadratic)
    // |(this.pos + this.vel * t) - (that.pos + that.vel * t)| == this.rad + that.rad
    
    Real dx = (x-pball->x);
    Real dy = (y-pball->y);
    Real dz = (z-pball->z);
    Real sr = (radius + pball->radius);

    Real dvx = (vx-pball->vx);
    Real dvy = (vy-pball->vy);
    Real dvz = (vz-pball->vz);

    Real ddv = 2*(dx*dvx+dy*dvy+dz*dvz);
    Real dd = dx*dx+dy*dy+dz*dz;
    Real dvdv = dvx*dvx+dvy*dvy+dvz*dvz;
    Real srsr = sr * sr;

    //dvdv*t^2+ ddv*t+(dd-srsr)=0

    Real a = dvdv;
    Real b = ddv;
    Real c = dd-srsr;
    Real delta = b*b-4*a*c;

    if (delta <= 0)
    {   // == a false touch
        t = ClashList::k_InfTime;
        return;
    }

    t = (-b-(Real)sqrt(delta))/(a*2);
    if (t < 0)
    {   // already cut each other
		// this case should be treated as a valid one
		// because it may result from a multiple clashing
		// e.g. `A' pushed back by colliding with `B', and pushed
		// forward by `C' and again going to hit `B', then it is 
		// very possible for `t' to be negative in the 
		// floating calculation which in a theoretical sense
		// should be zero
        // however, taking two departing objects with negative clashing time
        // for a future one (0 clashing time) can cause deadlock

        // So, it's this section's task to identify this case

        // Advice on performance enhancement
        // This sticky characteristic of this clashing modeling is disgusting
        // We may find that two rigid simple objects can never hit twice 
        // consequently, so we can take advantage of it to avoid redundant checking.
        // Or we can ensure any two objects never overlap each other by keeping the 
        // hitting objects from running full journey

        Real ax = pball->x-x;
        Real ay = pball->y-y;
        Real az = pball->z-z;
        Real thisVa = vx*ax+vy*ay+vz*az;
        Real thatVa = pball->vx*ax+pball->vy*ay+pball->vz*az;
        if (thatVa - thisVa > 0)
        {
            t = ClashList::k_InfTime;
        }
        else
        {
            t = 0;
        }
    }
}

void TestBall::FreeMove (Time t)
{
    x += vx*t;
    y += vy*t;
    z += vz*t;
}

bool TestBall::IsMoving ()
{
    const Real k_MovThr = (Real)0.01;
    if (!bMovable)
    {
        return false;
    }
    return (vx*vx+vy*vy+vz*vz>k_MovThr*k_MovThr);
}

void TestBall::Hit (Object *that)
{
    //assert(typeid(*that) == typeid(TestBall));
    TestBall *pball = dynamic_cast<TestBall *>(that);

    // vector from the center of this to the center of that
    Real ax = pball->x-x;
    Real ay = pball->y-y;
    Real az = pball->z-z;
    Real ad = sqrt(ax*ax+ay*ay+az*az);
    ax/=ad;
    ay/=ad;
    az/=ad;
    
    // inner product
    Real thisVa = vx*ax+vy*ay+vz*az;
    Real thatVa = pball->vx*ax+pball->vy*ay+pball->vz*az;

    if (thatVa-thisVa >= 0) return;

    // theorem of momentum
    // m1*v1+m2*v2==m1*v1'+m2*v2'
    // theorem of conservation of energy
    // m1*v1^2+m2*v2^2==m1*v1'^2+m2*v2'^2
    // these two theorems together applied to the collision process 
    // in the direction perpendicular to the surface
    // Solve the two-term quadratic equations

    Real thatMass = pball->mass;
    Real mom = mass*thisVa + thatMass*thatVa;
    Real kin = mass*thisVa*thisVa + thatMass*thatVa*thatVa;
    // m1*v1+m2*v2=mom-->v1=(mom-m2*v2)/m1
    // m1*v1^2+m2*v2^2=kin
    // (mom-m2*v2)^2+m1*m2*v2^2=m1*kin
    // m2*(m2+m1)*v2^2 - 2*mom*m2*v2 + (mom^2-m1*kin) = 0
    Real a = (thatMass+mass)*thatMass;
    Real b = -2*mom*thatMass;
    Real c = mom*mom-mass*kin;
    Real delta = b*b-4*a*c;

    Real v2, v1;
    if (delta > 0)
    {
        Real sqrtDelta = sqrt(delta);
        Real v21 = (-b-sqrtDelta)/(2*a);
        Real v22 = (-b+sqrtDelta)/(2*a);
        Real v11 = (mom-v21*thatMass)/mass;
        Real v12 = (mom-v22*thatMass)/mass;

        if (v21 > v11)
        {
            v2=v21;v1=v11;
        }
        else if (v22 > v12) 
        { 
            v2=v22;v1=v12;
        }
        else
        {   // error
            return;
        }
    }
    else if (delta == 0)
    {
        v2 = (-b)/(2*a);
        v1 = (mom-v2*2)/mass;
    }
    else
    {   // error
        return;
    }
    
    // momentum not involved in the collision

    vx += (v1 - thisVa)*ax;
    vy += (v1 - thisVa)*ay;
    vz += (v1 - thisVa)*az;

    pball->vx += (v2 - thatVa)*ax;
    pball->vy += (v2 - thatVa)*ay;
    pball->vz += (v2 - thatVa)*az;
}


