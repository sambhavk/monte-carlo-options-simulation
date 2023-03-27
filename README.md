# monte-carlo-options-simulation
Implementation of monte carlo simulation using geometric brownian motion(GBM) model to simulate strike price of options.

Execution:
1) For cpu execution run command : ./run_OptionsSimulation.sh
2) For gpu execution run command : ./q run_OptionsSimulation.sh

Sample output:

    Spot price of option is: 100
    Average Strike price after running 1000 simulations is: 125.934
    The average interest wrt the spot price is: 25.934%

    Number of simulation returning 0-10% interest wrt spot price = 532
    Number of simulation returning 10-30% interest wrt spot price = 118
    Number of simulation returning 30-50% interest wrt spot price = 75
    Number of simulation returning more than 50% interest wrt spot price = 275
