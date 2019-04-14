//
//  mathdd.hpp
//  helloworld
//
//  Created by Jiuhua on 1/24/18.
//  Copyright © 2018 Jiuhua. All rights reserved.
//

#ifndef GUARD_mathdd_hpp
#define GUARD_mathdd_hpp
#include <iostream>
#include <vector>
#include <fstream>
#include <math.h>

namespace dd{
    struct TwoDoubles{
        double a;
        double b;
        TwoDoubles(){
        }
        TwoDoubles(double aa, double bb){
            a = aa;
            b = bb;
        }
        void cout(){
            std::cout << "a = " << a << std::endl << "b = " << b << std::endl;
        }
    };
    
    
    template<class T1> double avg(const T1&);
    unsigned long nrand(unsigned long);
    unsigned long factorial(unsigned int);
    unsigned long numberOfPermutations(unsigned long, unsigned long);
    unsigned long numberOfCombinations(unsigned long, unsigned long);
    
    template<class T1> double stdDeviation(const T1&);
    template<class T1> double stdError(const T1&);
    TwoDoubles multiplyWithStdError(TwoDoubles, TwoDoubles);
    
    double volumeOfSphere(double);
    double areaOfSphere(double);
    
    template<class T1> void readNumbers(std::string, T1&);
    
    double electronvoltToJoules(double);
    double joulesToElectronvolt(double);
    
    double energyOfPhotonFreq(double);
    double lightFreqToWavelength(double);
    double lightWavelengthToFreq(double);
    double hydrogenEmitLightFreq(unsigned int, unsigned int);
    
    const static double PI = 3.14159265359;
    const static double PLANCKS_CONSTANT = 6.62607004E-34;
    const static double MOLE_CONSTANT = 6.02214179E23;
    const static double SPEED_OF_LIGHT = 2.99792458E8;
    const static double MASS_OF_ELECTRON = 9.10938356E-31;
    const static double MASS_OF_PROTON = 1.6726219E-27;
    const static double HYDROGEN_ORBITAL_ENERGY_CONSTANT = 2.18E-18;
    
    
}

//--------------------------template function definitions--------------------------
template<class T1> double dd::avg(const T1& ctn){
    double sum = 0;
    std::size_t size = ctn.size();
    for(std::size_t i = 0; i != size; i++){
        sum += *(ctn.begin()+i);
    }
    return sum/size;
}
template<class T1> double dd::stdDeviation(const T1& ctn){
    double average = avg(ctn);
    double sum = 0;
    std::size_t size = ctn.size();
    if(size <= 1){
        throw std::out_of_range("Too few elements to calculate std deviation.");
    }
    for(std::size_t i = 0; i != size; i++){
        sum += pow(((*(ctn.begin()+i)) - average), 2);
    }
    return sqrt(sum/(size-1));
}
template<class T1> double dd::stdError(const T1& ctn){
    return stdDeviation(ctn)/(sqrt(ctn.size()));
}

template<class T1> void dd::readNumbers(std::string file_name, T1& ctn){
    std::ifstream in_file;
    in_file.open(file_name);
    if(in_file.is_open())
        std::cout << "Open(" << file_name << ") succeed." << std::endl;
    else{
        std::cout << "Open(" << file_name << ") failed." << std::endl;
        return;
    }
    unsigned long n = 0;
    double x;
    while(in_file >> x){
        ctn.push_back(x);
        n++;
    }
    std::cout << "Read " << n << " numbers" << std::endl;
    in_file.close();
}
//--------------------------template function definitions--------------------------
#endif /* mathdd_hpp */
