#pragma once

#include <stdlib.h>
#include <math.h>
//#define DLLexport __declspec(dllexport)
// 3 party
class Bspline{
private:
    static const float A[16];
    float* c;    // used for spline coefficient computation
    static const float pole, b0;                                  // used for spline coefficient computation
    float k0;
    int inarrLength;                                  // the length of the input array
    int outarrLength;                                 // the length of the output array
    int j, k;                                         // counters
    int end;
    float d, d2, d3;
public:
    Bspline(int inArrayLength);            // use this constructor for debugging spline_coeff() only; NEVER use for actual splining
    Bspline(int inArrayLength, int outArrayLength); // general purpose constructor
    ~Bspline();                            // destructor
    void spline_coeff(float* initArray);  // computes the spline coefficient
    void b_spline(float *x, float* yi);              // performs b-spline, x contains the locations of interest; MUST be between 0 and inarrLength (data array length)
};

//void DLLexport spline_coeff(float* initArray, float* spline_coeff, int ArrayLength);        // This function is for debugging/outputting spline coefficients only; not required in general
//void DLLexport b_spline(float* initArray, float* outArray, float* locArray, int inArrayLength, int outArrayLength);  // General purpose dll function used for bsplining


const float Bspline::A[16] = {
    0,
    float(1)/float(6),
    float(4)/float(6),
    float(1)/float(6),
    0,
    float(-3)/float(6),
    0,
    float(3)/float(6),
    0,
    float(3)/float(6),
    float(-6)/float(6),
    float(3)/float(6),
    float(-1)/float(6),
    float(3)/float(6),
    float(-3)/float(6),
    float(1)/float(6)
};

const float Bspline::pole = float(-2) + sqrt(float(3));

const float Bspline::b0 = -6*pole/(1-pow(pole, float(2)));

Bspline::Bspline(int inArrayLength){
    c = new float[inArrayLength];
    k0 = min(inArrayLength, floor(-8/log10(fabs(pole))));
    inarrLength = inArrayLength;
    j = 0; k = 0;
}



Bspline::Bspline(int inArrayLength, int outArrayLength){
    //A[0] = 0;
    //A[1] = float(1)/float(6);
    //A[2] = float(4)/float(6);
    //A[3] = float(1)/float(6);
    //A[4] = 0;
    //A[5] = float(-3)/float(6);
    //A[6] = 0;
    //A[7] = float(3)/float(6);
    //A[8] = 0;
    //A[9] = float(3)/float(6);
    //A[10] = float(-6)/float(6);
    //A[11] = float(3)/float(6);
    //A[12] = float(-1)/float(6);
    //A[13] = float(3)/float(6);
    //   A[14] = float(-3)/float(6);
    //A[15] = float(1)/float(6);
    
    /*cp = new float[inArrayLength];
     cm = new float[inArrayLength];*/
    c = new float[inArrayLength];
    k0 = min(inArrayLength, floor(-8/log10(fabs(pole))));
    inarrLength = inArrayLength;
    outarrLength = outArrayLength;
    j = 0; k = 0;
    end = inArrayLength;
    d = 0; d2 = 0; d3 = 0;
}

Bspline::~Bspline(){
    delete [] c;
}

void Bspline::spline_coeff(float* initArray){
    float* cp; float* cm;
    cp = new float[inarrLength];
    cm = new float[inarrLength];
    cp[0] = 0;
    for (k = 0; k < k0; k++){
        cp[0] = cp[0] + pow(pole,k)*initArray[k];
    }
    for (k = 1; k < inarrLength; k++){
        cp[k] = initArray[k] + pole*cp[k-1];
    }
    cm[inarrLength - 1] = cp[inarrLength - 1];
    for (k = inarrLength - 2; k>=0; k--){
        cm[k] = initArray[k] + pole*cm[k+1];
    }
    for (k = 0; k < inarrLength; k++){
        c[k] = b0 * (cp[k] + cm[k] - initArray[k]);
    }
    delete [] cp;
    delete [] cm;
}

void Bspline::b_spline(float* x, float* yi){
    for (j = 0; j < outarrLength; j++){
        d = floor(x[j]) - x[j];
        d2 = d*d;
        d3 = d*d*d;
        k = int(floor(x[j]));  // take the floor of (j+1)th x value
        if (k > 0 && k < (inarrLength-2)){
            yi[j] =    (                 A[1]  * c[k+1] + A[2]  * c[k] + A[3]  * c[k-1])
            + d  * (                 A[5]  * c[k+1]                + A[7]  * c[k-1])
            + d2 * (                 A[9]  * c[k+1] + A[10] * c[k] + A[11] * c[k-1])
            + d3 * (A[12] * c[k+2] + A[13] * c[k+1] + A[14] * c[k] + A[15] * c[k-1]);
        }
        else if (k == 0){
            yi[j] =    (               A[1]  * c[1] + A[2]  * c[0] + A[3]  * c[1])
            + d  * (               A[5]  * c[1]                + A[7]  * c[1])
            + d2 * (               A[9]  * c[1] + A[10] * c[0] + A[11] * c[1])
            + d3 * (A[12] * c[2] + A[13] * c[1] + A[14] * c[0] + A[15] * c[1]);
        }
        else if (k == inarrLength - 2){
            yi[j] =    (                   A[1]  * c[end-1] + A[2]  * c[end-2] + A[3]  * c[end-3])
            + d  * (                   A[5]  * c[end-1]                    + A[7]  * c[end-3])
            + d2 * (                   A[9]  * c[end-1] + A[10] * c[end-2] + A[11] * c[end-3])
            + d3 * (A[12] * c[end-2] + A[13] * c[end-1] + A[14] * c[end-2] + A[15] * c[end-3]);
        }
        else if (k == inarrLength - 1){
            yi[j] =    (                   A[1]  * c[end-2] + A[2]  * c[end-1] + A[3]  * c[end-2])
            + d  * (                   A[5]  * c[end-2]                    + A[7]  * c[end-2]) 
            + d2 * (                   A[9]  * c[end-2] + A[10] * c[end-1] + A[11] * c[end-2])
            + d3 * (A[12] * c[end-3] + A[13] * c[end-2] + A[14] * c[end-1] + A[15] * c[end-2]); 
        }
        else{                // in other words, if (k < 0 || k > end-1)
            yi[j] = -1000;
        }
    }
}