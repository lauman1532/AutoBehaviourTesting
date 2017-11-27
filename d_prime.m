%%% d_prime calculation
%%% 27/11/2018
%%% Vick Lau
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function [dPrime, c, beta] = d_prime(sample)
    %% Import data
    fileID = fopen(sample);
    data = textscan(fileID, '%u8 %u8 %u8 %u8 %u8', 'Delimiter', ',', ...
        'CommentStyle', '//');
    fclose(fileID);

    %% Data
    calInterval = 1; % d' measuring interval
    nTrials = length(data{2}); % total no. of trials
    nHit = sum(data{2}); % no. of hits
    nFA = sum(data{3}); % no. of false alarms
    nM = sum(data{4}); % no. of misses
    nCR = sum(data{5}); % no. of correct rejections

    %% Calculate d' (SDT)
    HR = nHit/(nHit+nFA); % Hit rate
    FAR = nFA/(nM+nFA); % False alarm rate

    zHR = norminv(HR); % z-score of HR
    zFAR = norminv(FAR); % z-score of FAR

    dPrime = zHR - zFAR; % Sensitivity
    c = -1/2*(zHR - zFAR); % Response Bias
    beta = exp(dPrime*c); % Likelihood Ratio
    
end
