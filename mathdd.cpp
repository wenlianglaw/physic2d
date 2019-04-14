//
//  mathdd.cpp
//  helloworld
//
//  Created by Jiuhua on 1/24/18.
//  Copyright © 2018 Jiuhua. All rights reserved.
//

#include "mathdd.hpp"
#include <vector>
#include <iostream>
#include <numeric>
using namespace std;

unsigned long dd::nrand(unsigned long n){
    unsigned long r;
    if(n <= RAND_MAX){
        unsigned long b = RAND_MAX/n;
        do {
            unsigned long ran = rand();
            r = ran/ b;
        }while(r >= n);
        return r;
    }else if(n > RAND_MAX){
        unsigned long k = n/RAND_MAX + 1;
        do r = rand() * k + nrand(k);
        while(r >= n);
        return r;
    }else{
        throw std::domain_error("Argument to nrand(int) is out of range");
    }
}
unsigned long dd::factorial(unsigned int n){
    if(n > 20)
        throw out_of_range("Factorial of " + to_string(n) + " is too large to be hold by a unsigned long.");
    unsigned long ret = 1;
    for(unsigned int i = n; i > 1; i--)
        ret *= i;
    return ret;
}
unsigned long dd::numberOfPermutations(unsigned long m, unsigned long n){
    unsigned long ret = 1;
    for(unsigned long i = n;i > n-m;i--)
        ret *= i;
    for(unsigned long i = m;i > 1;i--)
        ret /= i;
    return ret;
}

unsigned long dd::numberOfCombinations(unsigned long m, unsigned long n){
    unsigned long ret = 1;
    for(unsigned long i = n;i > n-m;i--)
        ret *= i;
    return ret;
}


dd::TwoDoubles dd::multiplyWithStdError(dd::TwoDoubles x, dd::TwoDoubles y){
    double c_number = x.a * y.a;
    double c_std_error = c_number * sqrt(pow(x.b/x.a, 2) + pow(y.b/y.a, 2));
    dd::TwoDoubles ret;
    ret.a = c_number;
    ret.b = c_std_error;
    return ret;
}



double dd::volumeOfSphere(double r){
    return (4.0/3.0) * PI * pow(r, 3);
}
double dd::areaOfSphere(double r){
    return 4.0 * PI * pow(r, 3);
}


double dd::electronvoltToJoules(double electronvolts){
    return electronvolts * 1.6E-19;
}
double dd::joulesToElectronvolt(double electronvolts){
    return electronvolts / 1.6E-19;
}


double dd::energyOfPhotonFreq(double freq){
    return PLANCKS_CONSTANT * freq;
}
double dd::lightFreqToWavelength(double freq){
    return SPEED_OF_LIGHT / freq;
}

double dd::lightWavelengthToFreq(double wavelength){
    return SPEED_OF_LIGHT / wavelength;
}
double dd::hydrogenEmitLightFreq(unsigned int n1, unsigned int n2){
    return HYDROGEN_ORBITAL_ENERGY_CONSTANT * (1.0/pow(n2, 2.0) - 1.0/pow(n1, 2.0)) / PLANCKS_CONSTANT;
}
