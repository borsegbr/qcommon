#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <malloc.h>

#include "qtl/math/matrix.h"


using namespace qtl;
using namespace std;


double getrand(double fmin=-10, double fmax=10)
{
    return (fmax-fmin)*rand()/RAND_MAX + fmin;
}


const int n = 5;
typedef square_matrix<double, n>    TestMatrix;
void test_oldinv(TestMatrix m)
{
    printf("\nin test_oldinv...\n");

    double detval = 0;

    cout<<m<<endl;
    m.det(detval);
    cout << "detval ="  <<detval << endl;
    cout <<m<<endl;
    m.inverse(detval);
    cout <<m<<endl;
}

void test_inv(double *in)
{
    printf("\nin test_inv...\n");

    int ip[n];
    double raw_m[n][n];
    memcpy(raw_m, in, sizeof(double)*n*n);

    square_matrix_inv<> minv;

    minv((double*)raw_m, ip, n, fabs);

    printf("[");
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            printf("%f", raw_m[i][j]);
            if (j < n-1) printf(", ");
        }
        if (i < n-1) printf("; ");
    }
    printf("]\n");
}

void test_invdet(double *in)
{
    printf("\nin test_invdet...\n");

    int ip[n];
    double detval = 0;
    double raw_m[n][n];
    memcpy(raw_m, in, sizeof(double)*n*n);

    square_matrix_inv<> minv;

    bool b = minv((double*)raw_m, detval, ip, n, fabs);
    if (!b)
    {
        printf("singular\n");
    }
    cout << "detval = "  << detval << endl;

    printf("[");
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            printf("%f", raw_m[i][j]);
            if (j < n-1) printf(", ");
        }
        if (i < n-1) printf("; ");
    }
    printf("]\n");
}

void test_inv_gje(double *in)
{
    printf("\nin test_inv_gje...\n");
    int ip[n], ip2[n];
    double raw_m[n][n];
    memcpy(raw_m, in, sizeof(double)*n*n);

    square_matrix_det<> detcalc;
    double val;
    detcalc.det(val, (double*)raw_m, ip, n);
    printf("detval = %f\n", val);

    square_matrix_gje<> minv;
    double bufout[5][5];
    MoutSample<> mout((double*)bufout, 5);
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            bufout[i][j] = (i==j)?1:0;
        }
    }

    memcpy(raw_m, in, sizeof(double)*n*n);
    minv((double*)raw_m, mout, ip, ip2, n, fabs);
#if 0
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            printf("%f ", raw_m[i][j]);
        }
        printf("; ");
    }
    printf("\n");
#endif
    printf("[");
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            printf("%f", bufout[i][j]);
            if (j < n-1) printf(", ");
        }
        if (i < n-1) printf("; ");
    }
    printf("]\n");
}

void testsample1(void)
{
    printf("\nin test_inv_gje...\n");
    int ip[n], ip2[n];
    double raw_m[n][n] = 
    {
        {66.164932,-27.156523,-2.240817,-15.978860,-6.574138},
        {-27.156523,21.624143,-0.585237,12.805969,-5.512204},
        {-2.240817,-0.585237,9.714200,-7.072931,10.807959},
        {-15.978860,12.805969,-7.072931,25.821748,-10.449708},
        {-6.574138,-5.512204,10.807959,-10.449708,16.603487},
    };
/*
    square_matrix_det<> detcalc;
    double val;
    detcalc.det(val, (double*)raw_m, ip, n);
    printf("detval = %f\n", val);
*/
    double bufout[5][5];
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            bufout[i][j] = (i==j)?1:0;
        }
    }

    printf("minving...\n");
#if 0
    square_matrix_gje<> minv;
    MoutSample<> mout((double*)bufout, 5);
    if (!minv((double*)raw_m, mout, ip, ip2, n, fabs))
    {
        printf("WA error\n"); return;
    }
#else
    square_matrix_inv<> minv;
    if (!minv((double*)raw_m, ip, n, fabs))
    {
        printf("WA error\n"); return;
    }
#endif
    printf("minving ok.\n");
#if 1
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            printf("%f ", raw_m[i][j]);
        }
        printf(";\n");
    }
    printf("\n");
#endif
    printf("[");
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            printf("%f", bufout[i][j]);
            if (j < n-1) printf(", ");
        }
        if (i < n-1) printf(";\n");
    }
    printf("]\n");
}

int main(void)
{
#if 0
    //srand(time(0));
    square_matrix<double, n>    m;

    double raw_m[n][n];
    printf("m = [");
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            raw_m[i][j] = m[i][j] = getrand();
            printf("%f", raw_m[i][j]);
            if (j < n - 1) printf(", ");
        }
        if (i < n - 1) printf("; ");
    }
    printf("]\n\n");

    //test_oldinv(m);
    //test_inv((double*)raw_m);
    //test_invdet((double*)raw_m);
    test_inv_gje((double*)raw_m);

#else
    testsample1();
#endif
    return 0;
}
