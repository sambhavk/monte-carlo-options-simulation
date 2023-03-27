#!/bin/bash
source /opt/intel/oneapi/setvars.sh > /dev/null 2>&1
/bin/echo "##" $(whoami) is compiling Monte Carlo Simulation OptionsSimulation.cpp
icpx -fsycl OptionsSimulation.cpp
if [ $? -eq 0 ]; then ./a.out; fi

