#include <sycl/sycl.hpp>
#include <time.h>
#include <iostream>
#include <cmath>
#include <iomanip>
#include <random>

using namespace sycl;
using namespace std;

void simulateOptionPricing(float spot_price, int time_period, float r, float sigma, int Nsimulation, int Nsteps, float dt){
    
    //TODO: Run this simulation with multiple devices and compare the results.
    //queue cpu(cpu_selector_v);
    queue gpu(gpu_selector_v);
    
    std::random_device rd;
    std::uniform_real_distribution<float> distr(-5.0, 5.0);
    
    //TODO: 1) Run the simulation with training dataset.
    //TODO: 2) Run the simulation with actual data.
    
    float random[Nsimulation][Nsteps];
    for(int i = 0; i < Nsimulation; ++i){
      for(int j = 0;  j < Nsteps; ++j){
          random[i][j] = distr(rd);
      }
    }
    
    /*
    std::cout<< "Random matrix = " << "\n\n";
    for(int i = 0; i < Nsimulation; ++i){
      for(int j = 0; j < Nsteps; ++j)
         std::cout<<random[i][j]<<'\t';
       std::cout<<'\n';
    }
    
    std::cout<< "\n\n";
    */
    
    float drift = (r - ((sigma*sigma)/2))*dt;
    
    /*
    std::cout<< "Drift = " << drift << "\n\n"; 
    */
    
    float priceMatrix[Nsimulation][Nsteps];
    for(int i = 0; i < Nsimulation; ++i){
        priceMatrix[i][0] = spot_price;
        for(int j = 1;  j < Nsteps; ++j)
         priceMatrix[i][j] = 0;
    }
    
    /*
    std::cout<< "Price matrix without simulation = " << "\n\n";
    for(int i = 0; i < Nsteps; ++i){
      for(int j = 0; j < Nsimulation; ++j)
         std::cout<<priceMatrix[i][j]<<'\t';
       std::cout<<'\n';
    }
    
    std::cout<< "\n\n";
    */
    
    float a = sigma*sqrt(dt);
    
    /*
    cout<< "Value of a = " << a << "\n\n";
    */
    
    //TODO: Add parallelism in below loop using parallel_for.
    for(int i = 0; i < Nsimulation; ++i){
      for(int j = 1;  j < Nsteps; ++j)
         priceMatrix[i][j] = priceMatrix[i][j-1] * exp(drift + (a*random[i][j]));
    }
    
    /*
    std::cout<< "Price matrix with simulation = " << "\n\n";
    for(int i = 0; i < Nsteps; ++i){
      for(int j = 0; j < Nsimulation; ++j)
         std::cout<<priceMatrix[i][j]<<'\t';
       std::cout<<'\n';
    }
    
    std::cout<< "\n\n";
    */
    
    // stores count of simulation returning interest 0-10%
    int simulationCount1 = 0;
    // stores count of simulation returning interest 10-30%
    int simulationCount2 = 0;
    // stores count of simulation returning interest 30-50%
    int simulationCount3 = 0;
    // stores count of simulation returning interest > 50%
    int simulationCount4 = 0;
    
    float avgOptionPrice = 0;
    for(int i = 0; i < Nsimulation; ++i){
        if(priceMatrix[i][Nsteps - 1] >= 0){
            float interestPercent = ((priceMatrix[i][Nsteps - 1] - spot_price) / spot_price) * 100;
            if (interestPercent < 10)
                simulationCount1++;
            else if (interestPercent < 30)
                simulationCount2++;
            else if (interestPercent < 50)
                simulationCount3++;
            else 
                simulationCount4++;
            
            avgOptionPrice = avgOptionPrice + priceMatrix[i][Nsteps - 1];
        }
    }
    
    avgOptionPrice = avgOptionPrice / Nsimulation;
    float avgInterestPercent = ((avgOptionPrice - spot_price) / spot_price) * 100;
    
    std::cout << "Spot price of option is: " << spot_price << "\n";
    std::cout << "Average Strike price after running " << Nsimulation << " simulations is: " << avgOptionPrice << "\n" << "The average interest wrt the spot price is: " << avgInterestPercent << "%"; 
    std::cout<< "\n\n";
    
    std::cout << "Number of simulation returning 0-10% interest wrt spot price = " << simulationCount1 << "\n";
    std::cout << "Number of simulation returning 10-30% interest wrt spot price = " << simulationCount2 << "\n";
    std::cout << "Number of simulation returning 30-50% interest wrt spot price = " << simulationCount3 << "\n";
    std::cout << "Number of simulation returning more than 50% interest wrt spot price = " << simulationCount4 << "\n\n";
}


int main(){

    float spot_price = 100.0;
    float time_period = 1;
    float r = 0.07;
    float sigma = 0.2;
    int Nsimulation = 1000;
    float Nsteps = 30;
    float dt = time_period/Nsteps;
    
    simulateOptionPricing(spot_price, time_period, r, sigma, Nsimulation, Nsteps, dt);
}