%%% Tone Generator for Speakers
%%% 18/01/2018
%%% Vick Lau
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%function tone_generator
clear all; close all; clc

    fs = 44100;
    duration = 2;
    toneFreq = 1000;
    amplitude = 0.2;
    
    t = 0+1/fs:1/fs:duration;
    y = amplitude*cos(2*pi*toneFreq*t);
    sound(y, fs);
    
    % time-domain
    figure()
    plot(t, y)
   
    % freq-domian
    f_ax = -pi+pi/length(t):2*pi/length(t):pi-pi/length(t);
    figure()
    plot(f_ax,fftshift(fft(y)));

%end