function [train_ids, test_ids] = getIndices(labels, mode, ratio, mask, prev_ind)

if (nargin < 4) || isempty(mask)
    mask = [];
end

if (nargin < 5) || isempty(prev_ind)
    prev_ind= [];
end

%% get test indices
num_trials = length(labels);
if strcmp(mode, 'rand')
    %test_ids = 1:ratio:num_trials;
    numTest = ceil(num_trials/ratio);
    %test_ids = randsample(num_trials, numTest );        % M2016
    shuf_ids =  randperm(num_trials);                 % M2014...
    test_ids = shuf_ids(1:numTest);                     % ... M2014

elseif strcmp(mode, 'rinc')
    batch_size = floor(num_trials/ratio);
    batch_ids = batch_size*(mask-1)+1:batch_size*(mask);
    test_ids = prev_ind(batch_ids);

elseif strcmp(mode, 'tool')    
    %Find the trials of the selected test tools by comparing the name
    test_ids = [];
    for toolN =1:length(mask)
        foundCells = strfind(labels, mask{toolN});
        tool_ids= find(~cellfun('isempty',foundCells));
        test_ids = [test_ids; tool_ids];
    end
   
elseif strcmp(mode, '1out')
    % Find the trials corresponding with the 1 out tool
    foundCells = strfind(labels, mask{1});
    test_ids= find(~cellfun('isempty',foundCells));    
    
end    

%% get train indices
train_ids = 1:num_trials;
train_ids(test_ids) = [];
