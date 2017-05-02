%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                     ONLINE SUPPORT VECTOR REGRESSION                    %
%                    Copyright 2006 - Francesco Parrella                  %
%                                                                         %
%      This program is distributed under the terms of the GNU License     %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function SVR = trainSVR(input, target, plotF)

% Build the OnlineSVR
SVR = OnlineSVR;

% Set Parameters
SVR = set(SVR,      'C',                    10, ...
                    'Epsilon',              0.1, ...
                    'KernelType',           'RBF', ...
                    'KernelParam',          30, ...
                    'AutoErrorTollerance',  true, ...
                    'Verbosity',            1, ...
                    'StabilizedLearning',   true, ...
                    'ShowPlots',            true, ...
                    'MakeVideo',            false, ...
                    'VideoTitle',           '');
% Training
SVR = Train(SVR, input,target);

% Show Info
if plotF > 0
    ShowInfo (SVR);
end

