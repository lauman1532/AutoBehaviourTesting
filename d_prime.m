%%% d_prime calculation
%%% 07/12/2017
%%% Vick Lau
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
clc; clear all; close all;
%% Import data
%{
fileID = fopen(sample);
data = textscan(fileID, '%u8 %u8 %u8 %u8 %u8', 'Delimiter', ',', ...
    'CommentStyle', '//');
fclose(fileID);
%}

%% Generate data

meanS1 = 25;
varS1 = 2;
meanS2 = 25;
varS2 = 2;
nSignalTrials = 50;
nNoiseTrials = 50;

for i = 1:10
    while (true)    
        nHit(i) = round(meanS1*abs(randn(1))+varS1);
        if (nHit(i) <= nSignalTrials)
            nM(i) = nSignalTrials - nHit(i);
            break
        end
    end
    
    while (true)
        nCR(i) = round(meanS2*abs(randn(1))+varS2);
        if (nCR(i) <= nNoiseTrials)
            nFA(i) = nNoiseTrials - nCR(i);
            break
        end
    end
    meanS1 = meanS1+i^(1/4);
    meanS2 = meanS2+i^(1/4);
end

%% Data Pre-processing
%{
calInterval = 1; % d' measuring interval
nTrials = length(data{2}); % total no. of trials
nHit = sum(data{2}); % no. of hits
nFA = sum(data{3}); % no. of false alarms
nM = sum(data{4}); % no. of misses
%nCR = sum(data{5}); % no. of correct rejections
%}

%% Calculate d' (SDT equal-var model)

    HR = nHit./(nHit+nM); % Hit rate
    FAR = nFA./(nM+nFA); % False alarm rate

    zHR = norminv(HR); % z-score of HR
    zFAR = norminv(FAR); % z-score of FAR

    dPrime = zHR - zFAR; % Sensitivity
    c = -1/2.*(zHR - zFAR); % Response Bias
    beta = exp(dPrime.*c); % Likelihood Ratio


%% Save Data
dataFileName = ['d_prime_', num2str(mouseID) ,'.csv'];

fileID1 = fopen(dataFileName, 'a');

dataExport = [dPrime, c, beta];
formatSpec = '%s %.4f %.4f %.4f\n';
fprintf(fileID1, formatSpec, datestr(now), dataExport);

fclose(fileID1);
