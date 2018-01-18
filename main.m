%%% main
%%% 15/01/2018
%%% Vick Lau
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

clc; clear variables; close all

%% Initialise Arduino for direct reading and writing
% a0 = arduino('com1', 'uno');

%% Initialise tasks
% 

%% Update cumulative d'
% if(task_finished)
    % save task result to .csv
    
    sample = 'sample.csv';
    mouseID = 123456789;

    [dPrime, c, beta] = d_prime(sample, mouseID);

    display(dPrime);
    display(c);
    display(beta);
% end