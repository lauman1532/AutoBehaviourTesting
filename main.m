%%% main
%%% 27/11/2018
%%% Vick Lau
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

clc; clear variables; close all

sample = 'sample.csv';
mouseID = 123456789;

[dPrime, c, beta] = d_prime(sample, mouseID);

display(dPrime);
display(c);
display(beta);