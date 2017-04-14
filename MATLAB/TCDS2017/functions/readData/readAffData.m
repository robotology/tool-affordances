function [aff_data, tp_labels]  = readAffData(simF, plotF, source, numTP, numAct)
%% HELP
% simF is a flag to indicate whether data comes from simluation (1) or the real robot (0)
% plotF is a flag to indicate whether the data should be plotted (1) or not(0)
% source indicates the source and format of the data, and it can be set to:
% - 'collector': data will be read from affCollector format
% - 'dumpers': data will be read from the aff dumpers format.
% numTP indicates the number of toolposes to be read (tools x poses)
% numAct indicates the number of actions performed with each tool-pose.
%

%% initalize variables / check arguments
if (nargin <2) || isempty(plotF)
    plotF = 0;            % for TCDS experiment
end

if (nargin <3) || isempty(source)
    source = 'collector';            % for TCDS experiment
end
if (nargin <4) || isempty(numTP)
    if simF==0
        numTP = 45;             % for TCDS experiment
    else
        numTP = 150;            % for TCDS experiment
    end
end
if (nargin <5) || isempty(numAct)
    if simF ==0
        numAct = 4;             % for TCDS experiment
    else
        numAct = 8;             % for TCDS experiment
    end
end

%% Read from affCollectors format
if strcmp(source, 'collector')
    % Set paths and constants
    if (simF==0)
        dataDir= './data/affData/real';
        filename = [dataDir,'/affData_collector_real.txt']; 
        numRep = 2;
    elseif (simF ==1)
        dataDir= './data/affData/sim';
        filename = [dataDir,'/affData_collector_sim.txt'];  
        numRep = 4;
    end

    % intialize counters and allocate matrices
    act_i = 1;
    tp_i = 1;
    affsMeans = zeros(numTP,numAct);
    indexVec_tp = zeros(numTP,1);
    indexVec = [];
    affsStds = zeros(numTP,numAct);
    tpLabels = cell(numTP,1);
    affDataMat = [];
    tpMat = zeros(numRep,numAct, numTP);

    % Open the text file.
    fid = fopen(filename,'r');

    % Read line by line
    tline = fgetl(fid);
    tpLabels{1} = tline;        % read first name
    while ischar(tline)           
        tline = fgetl(fid);
        if (tline == -1) %  EOF
            break;
        end

        if (act_i <= numAct)        
            %Read inside line

            act_affs = sscanf(tline,'%f')';
            aff_mean = mean(act_affs);
            aff_std = std(act_affs);

            tpMat(1:numRep, act_i,tp_i ) = act_affs (1:numRep)';
            indexVec_tp = ones(numRep,1) * tp_i;        

            affsMeans(tp_i, act_i) = aff_mean;
            affsStds(tp_i, act_i) = aff_std;       

            if plotF == 1
                fprintf('Act_i = %i \n', act_i)               
                fprintf('%s \n', tline) 
            end

            act_i = act_i + 1;
        else        
            affDataMat = [affDataMat; tpMat(:,:,tp_i)];   
            indexVec = [indexVec; indexVec_tp];

            %Save the label
            fprintf('-- %s \n', tline)        
            tp_i = tp_i + 1;  
            tpLabels{tp_i} = tline;
            act_i = 1;             
        end


    end
    affDataMat = [affDataMat; tpMat(:,:,tp_i)];   % save last tpMat
    indexVec = [indexVec; indexVec_tp];           % save last indexVEc_tp
    fclose(fid);


    % Separe data from labels
    aff_data = affDataMat;

    % Expand labels for  number ot total trials
    for i = 1: length(affDataMat) 
        tp_labels{i} = tpLabels{indexVec(i)}; %(1:8);
    end
    
%% Read from Dumpers format
elseif strcmp(source, 'dumpers')    
    if simF == 0
        filename = './data/affData/real/affData_dumpers_real.txt';
        numRep = 4;
    else
        filename = './data/affData/real/affData_dumpers_sim.txt';
        numRep = 4;
    end
    
    % Set format
    delimiter = {' ','(',')'};
    formatSpec = '%*s%*s%s%f%f%f%*s%f%*s%*s%[^\n\r]';       % Format string for each line of text:
    fileID = fopen(filename,'r');                           % Open the text file.
    
    % Read columns of data according to format string.
    dataArray = textscan(fileID, formatSpec, 'Delimiter', delimiter, 'MultipleDelimsAsOne', true,  'ReturnOnError', false);
    fclose(fileID);
    tool_labs = dataArray{:, 1};
    %tool_id = dataArray{:, 2};
    deg_all = dataArray{:, 3};
    %act = dataArray{:, 4};
    eff = dataArray{:, 5};
    
    aff_data = reshape(eff,numRep,length(eff)/numRep)';
    deg = deg_all(1:numRep:end);
    tool_labels = tool_labs(1:numRep:end);
    
    N = size(tool_labels,1);
    ori = deg2ori(deg);
    tp_labels = cell(1,N);
    for tp = 1:N
        tp_labels{tp} = [tool_labels{tp} '_' ori{tp}];
    end
    tpMat= permute(reshape(eff',4,4,45),[2,1,3]);
    tpLabels = unique(tp_labels,'stable');
end

%% plot if required
if any(plotF) 
    %color = 'b';
    predF = -1;             % -1 for colors, 0 for blue(data), 1 for red (prediction)
    color = jet(15);
    %color = hsv(15);
    figure(plotF)
    if (simF)
        % Plot the all toolposes in each dy tool type:
         plot_toolpose_affs(tpMat(:,:,1:30), tpLabels(1:30), plotF, simF)
         plot_toolpose_affs(tpMat(:,:,31:60), tpLabels(31:60), plotF+1, simF)
         plot_toolpose_affs(tpMat(:,:,61:90), tpLabels(61:90), plotF+2, simF)
         plot_toolpose_affs(tpMat(:,:,91:120), tpLabels(91:120), plotF+3, simF)
         plot_toolpose_affs(tpMat(:,:,121:150), tpLabels(121:150), plotF +4 , simF)
         
        %Plot data from each tooltype-pose in a single plot:
        figure(plotF+5)
        plotTTPaffs(tpMat(:,:,1:30), 'hoes',0, color, predF);
        plotTTPaffs(tpMat(:,:,31:60), 'hooks',1, color(4:6,:), predF);
        plotTTPaffs(tpMat(:,:,61:90), 'rakes',2, color(7:9,:), predF);
        plotTTPaffs(tpMat(:,:,91:120), 'sticks',3, color(10:12,:), predF);
        plotTTPaffs(tpMat(:,:,121:150), 'shovels',4, color(13:15,:), predF);
        
    else
        plot_toolpose_affs(tpMat(:,:,1:9),tpLabels(1:9), plotF, simF)
        plot_toolpose_affs(tpMat(:,:,10:18), tpLabels(10:18), plotF+1, simF)
        plot_toolpose_affs(tpMat(:,:,19:27), tpLabels(19:27), plotF+2, simF)
        plot_toolpose_affs(tpMat(:,:,28:36), tpLabels(28:36), plotF+3, simF)
        plot_toolpose_affs(tpMat(:,:,37:45), tpLabels(37:45), plotF+4, simF)
        
        figure(plotF+5)
        plotTTPaffs(tpMat(:,:,1:9), 'hoes',0, color(1:3,:), predF);
        plotTTPaffs(tpMat(:,:,10:18), 'hooks',1, color(4:6,:), predF);
        plotTTPaffs(tpMat(:,:,19:27), 'rakes',2, color(7:9,:), predF);
        plotTTPaffs(tpMat(:,:,28:36), 'sticks',3, color(10:12,:), predF);
        plotTTPaffs(tpMat(:,:,36:45), 'shovels',4, color(13:15,:), predF);
    end
end