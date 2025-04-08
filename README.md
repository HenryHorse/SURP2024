# Empirical Support for Algorithmic Conjectures

This repository is for the code I developed during SURP 2024 and Fall Quarter of 2024. It is code for the Markov Chain Monte Carlo, Glauber dynamics, which randomly samples independent sets on trees. I implemented Glauber dynamics in C++, and wrote code that uses the sampler to estimate the total number of independent sets in a tree. There are two versions of the code for the counter in glauber_dynamics.cpp, using two different methods of estimating the total count. I also wrote code for finding the total number of independent sets in the tree using dynamic programming, in order to compare the Markov chain counter's performance against the actual value. This was intended as empirical evidence of Glauber dynamics' theoretical runtime. 

The repository also includes code for randomly generating trees of size n using prufer sequences.

The provided compilation method is the CMake. I did most of my development on MacOS, so the CMake may need reconfiguring for your particular machine.


