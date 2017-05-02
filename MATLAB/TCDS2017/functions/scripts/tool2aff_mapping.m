%% set control variables.
clear
close all
warning('off', 'all')
saveF = 0;
plot_figs = 0;


%            sim       feat_type      datadiv      baseline
params = [    0            0             0            0    ]

%Select robot type. 0 = real, 1 = sim
simF = params(1);
if simF ==0
    robot = 'real';
    source = 'dumpers';
else
    robot = 'sim';
    source = 'collector';
end


%Select features type
if (params(2) == 0)
    feat_t ='OMSEGI';
elseif (params(2) == 1)
    feat_t = 'EGI';   
elseif(params(2) == 2)
    feat_t = 'OCCUP';
end
feat_type = ['toolFeats_' feat_t]

%Select data separation method
if (params(3) == 0)
    datadiv_mode = 'rand';
elseif (params(3) == 1)
    datadiv_mode = '1out';
elseif (params(3) == 2)
    datadiv_mode = 'tool';
end

baseline_test = params(4);
%baseline_test = 1;

regr_mode = 'gnn';
%regr_mode = 'svm';

datadiv_ratio = 4;
numRuns = 10;

if plot_figs
    aff_data_fig = 10;
    aff_SOM_fig = 15;                   %if > 100, affVectors will be plotted on top
    aff_SOM_fig_test = 20;               %if > 100, affVectors will be plotted on top
    tool_SOM_fig_train = 30;             %if > 100, toolpose images will be plotted on top    
else    
    aff_data_fig =  10;
    aff_SOM_fig =  0;                    %if > 100, affVectors will be plotted on top
    aff_SOM_fig_test = 0;                %if > 100, affVectors will be plotted on top
    tool_SOM_fig_train =  0;             %if > 100, toolpose images will be plotted on top    
end

%% Read the data

% Read the Affordance data
[aff_data, aff_labels]  = readAffData(simF,aff_data_fig,source);

% Read the tool 3D features
[tool_data, tools_labels, tp_labels, toolIms]  = read3Dfeatures(feat_type, simF);


%% map feat with aff vectors
map_i = map_aff2tool(aff_labels, tp_labels);

%% Select the kind of loop to do
tools = unique(tools_labels,'stable');
if strcmp(datadiv_mode,'1out')
    numRuns  = length(tools);    
end

exp_name = [robot '_' datadiv_mode num2str(numRuns) '_' feat_t];


% allocate memory for speed and control
net_feat2aff = cell(numRuns,1);
sM_aff = cell(numRuns,1);   sM_tool = cell(numRuns,1); 
aff_data_test = cell(numRuns,1);    aff_data_train = cell(numRuns,1);
aff_labels_test = cell(numRuns,1);  aff_labels_train = cell(numRuns,1);
tool_data_test = cell(numRuns,1);   tool_data_train = cell(numRuns,1);
tp_labels_test = cell(numRuns,1);   tp_labels_train = cell(numRuns,1);

aff_test_pred_affvector = cell(numRuns,1); aff_train_pred_affvector = cell(numRuns,1);
aff_test_pred_bmu_coords = cell(numRuns,1); aff_train_pred_bmu_coords = cell(numRuns,1);
aff_pred_abs_error = zeros(1,numRuns); aff_pred_abs_error_train = zeros(1,numRuns);

%% loop through tool or repetitions
%    runI = 1;
for runI = 1:numRuns


    %% divide data_train data_test
    % Get masks for tool-wise separation:    
    fprintf('\n =========== On experiment %s \n', exp_name);
    if strcmp(datadiv_mode,'rand')
        mask = [];        
        fprintf('---------------------------------------------- Running for run %i \n', runI)
        % get indices
        [aff_train_ids, aff_test_ids] = getIndices(aff_labels, datadiv_mode , datadiv_ratio, mask); % divide all aff_data into training and testing 
        tool_test_ids = map_i(aff_test_ids);                                            % corresponding tool indices for test samples of aff-data
        %tool_train_ids = map_i(aff_train_ids);                                         % corresponding tool indices for train samples of aff-data
        tool_train_ids = 1:size(tool_data,1);
        tool_train_ids(tool_test_ids) = [];                                             % all the remaining non-test samples from tool-data
    elseif strcmp(datadiv_mode,'1out')
        mask = tools(runI)                
        fprintf('---------------------------------------------- Running for run %i \n', runI)
        % get indices
        [aff_train_ids, aff_test_ids] = getIndices(aff_labels, datadiv_mode , datadiv_ratio, mask);   % divide all aff_data into training and testing 
        [tool_train_ids, tool_test_ids] = getIndices(tp_labels, datadiv_mode , datadiv_ratio, mask);  % divide all tool_data into training and testing
        tool_test_ids = map_i(aff_test_ids);                                                          % corresponding tool indices for test samples of tool-data        
    elseif strcmp(datadiv_mode,'tool')
        mask_i = randperm(length(tools));
        mask = tools(mask_i(1:round(length(tools)/datadiv_ratio)))
        fprintf('---------------------------------------------- Running for run %i \n', runI)
    end   
    
    % split data
    [aff_data_train{runI}, aff_data_test{runI}, aff_labels_train{runI}, aff_labels_test{runI}] = splitData(aff_data, aff_labels, aff_test_ids, aff_train_ids);
    % indices of non-test samples of tool-data corresponding with the training ones from aff-data 
    [tool_data_train{runI}, tool_data_test{runI}, tp_labels_train{runI}, tp_labels_test{runI}] = splitData(tool_data, tp_labels, tool_test_ids, tool_train_ids);
    
    % Dividie images too
    if plot_figs
        toolim_test_id  = map_aff2tool(tp_labels_test{runI}, toolIms(3,:),1);
        toolIms_test = toolIms(:,toolim_test_id);
        toolim_train_ids = map_aff2tool(tp_labels_train{runI}, toolIms(3,:),1);
        toolIms_train = toolIms(:,toolim_train_ids);
    else
         toolIms_train = [];
    end
    
    %% Map the data onto SOM
    % Map the tool features onto SOM
    % we use the whole non-test tool dataset for initial mapping, including also those trials not mapped to an affordance vector, to get a better mapping.        
    [tool_bmu_train, clust_tool_train_ids, som_coords_tool_train, sM_tool_train]  = dimred2map(tool_data_train{runI}, tp_labels_train{runI}, 'tool', 'som', toolIms_train, tool_SOM_fig_train);
    
    % Map the affVectors onto SOM
    [ aff_bmu_train, clust_aff_ids, som_coords_aff,  sM_aff{runI}] = dimred2map(aff_data_train{runI}, aff_labels_train{runI}, 'aff', 'som', [], aff_SOM_fig);
    
    %find the tool-pose vectors which have a matching aff-vector
    map_i_train = map_aff2tool(aff_labels_train{runI}, tp_labels_train{runI});  % indices of train samples of tool-data corresponding with the training ones from aff-data
    %map_i_test = map_aff2tool(aff_labels_test{runI}, tp_labels_test{runI});    % indices of test samples of tool-data corresponding with the training ones from aff-data
    tool_bmu_train  = tool_bmu_train(map_i_train,:);                         % extract those samples from the tool mapping for which we do have a corresponding aff vector.
    tool_data_train_match{runI} = tool_data_train{runI}(map_i_train,:);
    tp_labels_train_match{runI} = tp_labels(map_i_train);
    sM_tool{runI} = sM_tool_train;

    %% Shuffle aff and tool coords to obtain random model => baseline prediction
    if baseline_test == 1
        rand_tool_i = randperm(length(tool_bmu_train));
        tool_bmu_train = tool_bmu_train(rand_tool_i,:);

        rand_aff_i = randperm(length(aff_bmu_train));
        aff_bmu_train = aff_bmu_train(rand_aff_i,:);    
    end
    
    %% Learn mapping between toolfeat coordinates and aff coordinates.
    if strcmp(regr_mode, 'gnn')
        [net_feat2aff{runI}, ~] = gnnRegAff(tool_bmu_train, aff_bmu_train, [0 0]);
        net_feat2aff{runI}.userdata = mask;
        %[net_aff2feat, ~] = gnnRegAff(aff_bmu_train, tool_bmu_train, [1 1]);
    elseif strcmp(regr_mode, 'svm')
        [svm_X, ~] = svmRegAff(tool_bmu_train, aff_bmu_train(:,1), [], 0);
        [svm_Y, ~] = svmRegAff(tool_bmu_train, aff_bmu_train(:,2), [], 0);
        net_feat2aff{runI} =  {svm_X, svm_Y};
        net_feat2aff{runI}{3} = mask;
        %[net_aff2feat, ~] = svmRegAff(tool_bmu_train, aff_bmu_train, [], 1);
    end
    
    %% Predict the affordance vectors for the test tool-feature samples   
    [aff_test_pred_affvector{runI}, aff_test_pred_bmu_coords{runI}] = som2som_predict(net_feat2aff{runI}, sM_tool{runI}, sM_aff{runI}, tool_data_test{runI}, regr_mode);
    [aff_train_pred_affvector{runI}, aff_train_pred_bmu_coords{runI}] = som2som_predict(net_feat2aff{runI}, sM_tool{runI}, sM_aff{runI}, tool_data_train_match{runI}, regr_mode);
    
    if  aff_SOM_fig_test ~= 0        
        dimred2map(aff_data_train{runI}, aff_labels_train{runI}, 'aff', 'som', [], aff_SOM_fig_test);           % plot another map to plot pred results on it.
        addMiniPlot(sM_aff{runI}.codebook, som_coords_aff,aff_labels_test, 1, zeros(375,3), aff_SOM_fig_test,2);          % plot all 'imagined' affordances
        som2som_compare(sM_aff{runI}, aff_test_pred_affvector{runI}, aff_test_pred_bmu_coords{runI}, aff_data_test{runI}, aff_SOM_fig_test)
    end
    
    
    %% Compute the error in the prediction in  a few different ways:
    % AFFORDANCE ERROR
    % sample by sample absolute error of predicted data wrt to recorded one.
    aff_pred_abs_error(runI) = mean(mean(abs(aff_data_test{runI}-aff_test_pred_affvector{runI})));
    aff_pred_abs_error_train(runI) = mean(mean(abs(aff_data_train{runI}-aff_train_pred_affvector{runI})));


    fprintf('\n Mean Abs Error %f \n', aff_pred_abs_error(runI));
    fprintf('\n Mean Abs Error Train %f \n', aff_pred_abs_error_train(runI));

end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
fprintf('==================================================================\n');
fprintf('Exp Mean Abs Error %f \n', mean(aff_pred_abs_error));

if saveF
    save_filename = ['som2som_predreal_aff_', robot '_' datadiv_mode num2str(numRuns) '_' feat_t '_' regr_mode];
    if (baseline_test == 1)
        save_filename = [save_filename '_baseline'];
    end
    save_filename
    save(save_filename,'net_feat2aff', 'sM_aff', 'sM_tool', ...    
        'aff_data_test', 'aff_data_train', 'aff_labels_train', 'aff_labels_test',  ...
        'tool_data_test', 'tool_data_train', 'tool_labels_train', 'tool_labels_test', 'tools',...
        'aff_test_pred_affvector', 'aff_train_pred_affvector', 'aff_pred_abs_error', 'aff_pred_abs_error_train' )
end

if strcmp(datadiv_mode,'1out')
    if (aff_data_fig~= 0)
        figure(aff_data_fig)
        plotPredAffVectors(aff_test_pred_affvector{runI},tp_labels,simF)
    end
end
