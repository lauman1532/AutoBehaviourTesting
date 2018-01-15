%%% main
%%% 15/01/2018
%%% Vick Lau
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

clc; clear variables; close all

%% Initialise Arduino for direct reading and writing
% a0 = arduino('com1', 'uno');

%%
sample = 'sample.csv';
mouseID = 123456789;

[dPrime, c, beta] = d_prime(sample, mouseID);

display(dPrime);
display(c);
display(beta);