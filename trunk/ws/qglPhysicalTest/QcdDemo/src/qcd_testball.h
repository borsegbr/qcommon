#ifndef qcd_testball_H
#define qcd_testball_H


#include <vector>
#include <typeinfo>
#include <cmath>
#include <cassert>
#include <conio.h>

#include "../stdafx.h"
#include "qgl/physical/collision_detection.h"


using namespace std;
using namespace qgl;

typedef QG_CD_001_ClashList<>   ClashList;
typedef ClashList::Object   Object;


/*
 * <Summary>
 * Ball for test
 * </Summary>
 */
struct TestBall : public Object
{
    typedef ClashList::Real     Real;
    typedef ClashList::Time     Time;

    Real    radius;
    Real    mass;
    bool    bMovable;

    TestBall() : radius(1), bMovable(false) {}

    virtual void VirtualClash(Object *that, Time &t);
    void FreeMove(Time dt);
    bool IsMoving();

    /*
     * On any error, states of both balls concerned are left unchanged
     */
    void Hit(Object *that);

};



#endif	// ifndef qcd_testball_H


