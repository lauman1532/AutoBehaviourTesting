%%% main
%%% 27/11/2018
%%% Vick Lau
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

clc; clear variables; close all

sample = 'sample.csv';

[dPrime, c, beta] = d_prime(sample);

display(dPrime);
display(c);
display(beta);