%%% main.m
%%% 02/06/2018
%%% Vick Lau
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

fclose all; clear all; delete(instrfindall)

%% Variables


%% Initialisation
arduino = serial('COM5', 'Baudrate', 9600); % Change COM according to your port
fopen(arduino);
pause(1);

send_str_2_ard(arduino,"MATLAB_RDY");

%%
while(true)
    mouse_entry(arduino);
end

fclose(arduino);

%% Function Definitions
function send_str_2_ard(serialObj, str)
    fprintf(serialObj, '$%s\n', str);
end

function [data] = read_data_ard(serialObj, size, type)
    x = fread(serialObj, 1, 'char');
    if (x == '>')
        data = fread(serialObj, size, type);
    else
        data = '';
    end
end

function [mouseID] = tag_conversion(tagData)
%   tag bytes conversion to actual ID
%   first byte is ignored because it is (usually) a version reference
    mouseID = uint32(bitsll(tagData(2), 24) + ...
        bitsll(tagData(3), 16) + bitsll(tagData(4), 8) + tagData(5));
end

function mouse_entry(serialObj)
    tagData = read_data_ard(serialObj, 5, 'uint8');
    if(~isempty(tagData))
       mouseID = tag_conversion(tagData)
       entryTime = clock;
       entry_data(mouseID, entryTime);
    end
end

%function [isEntry, mouseID, entryTime] = mouse_entry(serialObj, startMarker)
%    isEntry = 0;
%    mouseID = 0;
%    entryTime = 0;
%    
%    x = fread(serialObj, 1, 'char')
%    if(x == startMarker)
%        tagData = fread(serialObj, 5, 'uint8')
%       entryTime = clock;
%        isEntry = 1;
%        send_str_2_ard(serialObj, "ENTRY");
%        
%        % tagData conversion: first byte is ignored because it is (usually) a version number
%        mouseID = uint32(bitsll(tagData(2), 24) + ...
%            bitsll(tagData(3), 16) + bitsll(tagData(4), 8) + tagData(5))
%    end
%end

function entry_data(mouseID, entryTime)
	fName = ['test_',num2str(mouseID),'.csv'];
    if(exist(fName,'file') == 0) % first time, CSV not found
        fileID0 = fopen(fName, 'A'); % 'A' instead of 'a' to disable auto flushing to improve performance
        if(fileID0 ~= -1)
            fprintf(fileID0, '%s,%s,%s,%s,%s\n', "Date-time", "taskID", "Trial", "Results", "Progression");

            formatSpec = '%s,%u,%u,%u,%u\n';
            fprintf(fileID0, formatSpec, datestr(entryTime), 1, 1, 0, 0);
            fclose(fileID0);
            curTaskID = 1;
        else
            msgbox(['Cannot create ', fName], 'Error', 'error');
            curTaskID = -1;
        end
    else
        fileID0 = fopen(fName, 'A+');
        if(fileID0 ~= -1)
            fseek(fileID0, -8, 'eof'); % The position of taskID is 8 bytes earlier than eof
            curTaskID = fscanf(fileID0, '%u', 1)
            fseek(fileID0, 0, 'eof');
            formatSpec = '%s\n';
            fprintf(fileID0, formatSpec, datestr(entryTime));
            fclose(fileID0);
        else
            curTaskID = -1;
            msgbox(['Cannot open', fName, ' for appending'], 'Error', 'error');
        end
    end   
end

function save_data(mouseID)
	fName = ['test_',num2str(mouseID),'.csv'];
    fileID0 = fopen(fName, 'A+');
    if(fileID0 ~= -1)
        fseek(fileID0, -8, 'eof'); % The position of taskID is 8 bytes earlier than eof
        curTaskID = fscanf(fileID0, '%u', 1)
        fseek(fileID0, 0, 'eof');
        formatSpec = '%s\n';
        fprintf(fileID0, formatSpec, datestr(entryTime));
        fclose(fileID0);
    else
        curTaskID = -1;
        msgbox(['Cannot open', fName, ' for appending'], 'Error', 'error');
    end
end

function d_prime()
    calInterval = 1; % d' measuring interval
    nTrials = length(data{2}); % total no. of trials
    nHit = sum(data{2}); % no. of hits
    nFA = sum(data{3}); % no. of false alarms
    nM = sum(data{4}); % no. of misses
    nCR = sum(data{5}); % no. of correct rejections

    % Calculate d' (SDT equal-var model)
    HR = nHit/(nHit + nM); % Hit rate
    FAR = nFA/(nFA + nM); % False alarm rate

    zHR = norminv(HR); % z-score of HR
    zFAR = norminv(FAR); % z-score of FAR

    dPrime = zHR - zFAR; % Sensitivity/discriminability
    c = -1/2*(zHR - zFAR); % Response Bias
    beta = exp(dPrime*c); % Likelihood Ratio
end

% 5-point unweighted MA filter
function [Y] = moving_average(data)
    Y = zeros(1:length(data));
    for i = 1:4
        Y(i) = 0;
    end
    for i = 5:1:length(data) 
        Y(i) = 1/5*(data(i-4) + data(i-3) + data(i-2) + data(i-1) + data(i));
    end
end
