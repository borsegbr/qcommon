#include <stdio.h>

#if 1

/* embedded */

struct Surface
{
    virtual float Transfer () = 0;
};

struct BoundaryData
{
    float   m_AToB;
    float   m_BToA;
};

struct Boundary
{

    struct SurfaceAToB : public Surface
    {
        virtual float Transfer ()
        {
            printf("a2b\n");
            BoundaryData *bd = reinterpret_cast<BoundaryData *>
                (reinterpret_cast<char*>(this) - sizeof(BoundaryData));
            return bd->m_AToB / bd->m_BToA;
        }
    };

    struct SurfaceBToA : public Surface
    {
        virtual float Transfer ()
        {
            printf("b2a\n");
            BoundaryData *bd = 
                reinterpret_cast<BoundaryData *>
                (reinterpret_cast<char*>(this) - sizeof(SurfaceAToB) - sizeof(BoundaryData));
            return bd->m_BToA / bd->m_AToB;
        }
    };

    BoundaryData    m_Data;
    SurfaceAToB     m_SurfAToB;
    SurfaceBToA     m_SurfBToA;

    void SetAToB (float v)
    {
        m_Data.m_AToB = v;
    }
    void SetBToA (float v)
    {
        m_Data.m_BToA = v;
    }

    Surface * GetSurfaceAToB ()
    {
        return &m_SurfAToB;
    }

    Surface * GetSurfaceBToA ()
    {
        return &m_SurfBToA;
    }
};

#else

/* object oriented? nge */

struct Surface
{
    virtual float Transfer () = 0;
};

struct BoundaryData
{
    float   m_AToB;
    float   m_BToA;

    void SetAToB (float v)
    {
        m_AToB = v;
    }
    void SetBToA (float v)
    {
        m_BToA = v;
    }
};

struct SurfaceAToB : virtual public BoundaryData, public Surface
{
    virtual float Transfer ()
    {
        printf("a2b\n");
        return m_AToB / m_BToA;
    }
};

struct SurfaceBToA : virtual public BoundaryData, public Surface
{
    virtual float Transfer ()
    {
        printf("b2a\n");
        return m_BToA / m_AToB;
    }
};

struct Boundary : public SurfaceAToB, public SurfaceBToA
{
    Surface * GetSurfaceAToB ()
    {
        return dynamic_cast<SurfaceAToB *>(this);
    }

    Surface * GetSurfaceBToA ()
    {
        return dynamic_cast<SurfaceBToA *>(this);
    }
};

#endif

struct Object
{
    float   m_Data;

    Surface *m_pSurface;
};

Object  g_Obj1, g_Obj2;
Boundary    g_Bound;

void Create ()
{
    g_Obj1.m_Data = 1;
    g_Obj2.m_Data = 2;
    g_Bound.SetAToB(3);
    g_Bound.SetBToA(4);
    g_Obj1.m_pSurface = g_Bound.GetSurfaceAToB();
    g_Obj2.m_pSurface = g_Bound.GetSurfaceBToA();
}

float Cast (Object *obj1, Object *obj2)
{
    Surface *surface = obj1->m_pSurface;
    return obj1->m_Data + obj2->m_Data + surface->Transfer();
}

void ViewBoundaryInBinary (Boundary *boundary, int size)
{
    unsigned char *b = reinterpret_cast<unsigned char *>(boundary);
    for (int i = 0; i < size; i++)
    {
        printf("%2x, ", b[i]);
    }
    printf("\n");
}

int main ()
{
    printf("size of boundary is %d, of boundary data is %d\n", sizeof(Boundary), sizeof(BoundaryData));
    Create();
    ViewBoundaryInBinary(&g_Bound, sizeof(Boundary));
    float res = Cast(&g_Obj1, &g_Obj2);
    printf("Cast 1 to 2 is %f\n", res);
    res = Cast(&g_Obj2, &g_Obj1);
    printf("Cast 2 to 1 is %f\n", res);
    return 0;
}


