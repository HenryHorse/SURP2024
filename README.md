# Empirical Support for Algorithmic Conjectures

This repository is for the code I developed during SURP 2024 and Fall Quarter of 2024. It is code for the Markov Chain Monte Carlo, Glauber dynamics, which randomly samples independent sets on trees. I implemented Glauber dynamics in C++, and wrote code that uses the sampler to estimate the total number of independent sets in a tree. I also wrote code for finding the total number of independent sets in the tree using dynamic programming, in order to compare the Markov chain counter's performance against the actual value. This was intended as empirical evidence of Glauber dynamics' theoretical runtime. 

We ran into difficulties on running large enough experiments, so I rewrote the code such that it could be used on San Diego's The Expanse supercomputer using MPI programming. This branch is the version that is intended for the cluster, whereas the **laptop** branch is intended for use on a local machine.

The repository also includes code for randomly generating trees of size n using prufer sequences.

The provided compilation method is the Makefile, intended for use on Unix machines. I did most of my development on MacOS, so the Makefile may need reconfiguring for your particular machine. Otherwise, install Boost Graph Library (I used version 1.83) and type:

```sh
make
```
Then run the code with ./test. This will run test.cpp, which runs the chain for trees of size 2,4,8 and so on up until 1024. It runs the chain untils its estimate count is within 1.4x the actual value. 

