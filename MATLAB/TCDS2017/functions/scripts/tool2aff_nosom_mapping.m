%% set control variables.
clear
close all
warning('off', 'all')

res_file = fopen('exp_results_noSom.txt', 'w');

robot = 'real';
%robot = 'sim';

feat_t ='OMSEGI';
%feat_t = 'EGI';   
%feat_t = 'OCCUP';
feat_type = ['toolFeats_' feat_t]

%datadiv_mode = 'rand';
datadiv_mode = '1out';
%datadiv_mode = 'tool';

baseline_test = 0;

regr_mode = 'gnn';
%regr_mode = 'svm';

if strcmp(robot, 'real')
    simF = 0;
else 
    simF = 1;
end

datadiv_ratio = 4;
numRuns = 50;

aff_data_fig = 0;
aff_SOM_fig = 0;               %if > 100, affVectors will be plotted on top
aff_SOM_fig_test = 0;          %if > 100, affVectors will be plotted on top
tool_SOM_fig_train = 0;        %if > 100, toolpose images will be plotted on top
tool_SOM_fig_nontest = 0;      %if > 100, toolpose images will be plotted on top

%% Read the data
% Read the tool 3D features
[tool_data, tools_labels, tool_labels, toolIms]  = read3Dfeatures(feat_type, simF);

% Read the Affordance data
[aff_data, aff_labels]  = readAffData(simF,aff_data_fig);


%% map feat with aff vectors
map_i = map_aff2tool(aff_labels, tool_labels);

%% Select the kind of loop to do
tools = unique(aff_labels);
if strcmp(datadiv_mode,'1out')
    runs = 1:length(tools);
else
    runs = 1:numRuns;   
end

%% loop through tool or repetitions
for runI = runs

    %% divide data_train data_test
    % Get masks for tool-wise separation:    
    if strcmp(datadiv_mode,'1out')
        mask = tools(runI)        
        fprintf('---------------------------------------------- Running for run %i \n', runI)
    elseif strcmp(datadiv_mode,'tool')
        mask_i = randperm(length(tools));
        mask = tools(mask_i(1:round(length(tools)/datadiv_ratio)))
        fprintf('---------------------------------------------- Running for run %i \n', runI)
    elseif strcmp(datadiv_mode,'rand')
        mask = [];        
        fprintf('---------------------------------------------- Running for run %i \n', runI)
    end
    
    
    %[tool_data_train, tool_data_test, test_ids] = splitData(tool_data, tp_labels_tool, datadiv_mode , 4,1, test_ids);
    % get indices
    [aff_train_ids, aff_test_ids] = getIndices(aff_labels, datadiv_mode , datadiv_ratio, mask); % divide all aff_data into training and testing 75/25  %
    tool_test_ids = map_i(aff_test_ids);                                            % corresponding tool indices for test samples of aff-data
    tool_train_ids = map_i(aff_train_ids);                                          % corresponding tool indices for train samples of aff-data
    
    %tool_nontest_ids = 1:size(tool_data,1);
    %tool_nontest_ids(tool_test_ids) = [];                                           % all the remaining non-test samples from tool-data

    % split data
    [aff_data_train, aff_data_test, aff_labels_train, aff_labels_test] = splitData(aff_data, aff_labels, aff_train_ids, aff_test_ids);
    % indices of non-test samples of tool-data corresponding with the training ones from aff-data 
    %[tool_data_nontest, tool_data_test, tool_labels_nontest, tool_labels_test] = splitData(tool_data, tool_labels, tool_nontest_ids, tool_test_ids);
    % extract those samples from the tool train data for which we do have a corresponding aff vector.
    [tool_data_train, tool_data_test, tool_labels_train, tool_labels_test] = splitData(tool_data, tool_labels, tool_train_ids, tool_test_ids);
    
    % Dividie images too
    toolim_test_id  = map_aff2tool(tool_labels_test, toolIms(3,:),1);
    toolIms_test = toolIms(:,toolim_test_id);
    toolim_train_ids = map_aff2tool(tool_labels_train, toolIms(3,:),1);
    toolIms_train = toolIms(:,toolim_train_ids);
%     
%     %% Map the data onto SOM
%     % Map the tool features onto SOM
%     % we use the whole non-test tool dataset for initial mapping, including also those trials not mapped to an affordance vector, to get a better mapping.
%     
%     [tool_nontest_K, tool_bmu_nontest, clust_tool_nontest_ids, som_coords_tool_nontest, sM_tool_nontest]  = dimred2map(tool_data_nontest, tool_labels_nontest, 'tool', 'som', toolIms_nontest, tool_SOM_fig_nontest);
%     %[tool_train_K, tool_bmu_train, clust_tool_train_ids, som_coords_tool_train, sM_tool_train]= dimred2map(tool_data_train, tool_labels_train, 'tool', 'som', toolIms, tool_SOM_fig_train);
%     
%     % Map the affVectors onto SOM
%     [aff_K, aff_bmu_train, clust_aff_ids, som_coords_aff,  sM_aff] = dimred2map(aff_data_train, aff_labels_train, 'aff', 'som', [], aff_SOM_fig);
%     %dimred2map(aff_data_train, aff_labels_train, 'aff', 'som', [], aff_SOM_fig_test);
%     %addMiniPlot(sM_aff.codebook, som_coords_aff, 1, zeros(375,3), aff_SOM_fig,2);          % plot all 'imagined' affordances
% 
%     map_i_train = map_aff2tool(aff_labels_train, tool_labels_nontest);              % indices of non-test samples of tool-data corresponding with the training ones from aff-data
%     tool_bmu_train  = tool_bmu_nontest(map_i_train,:);                     % extract those samples from the tool mapping for which we do have a corresponding aff vector.
%     tool_data_train = tool_data_nontest(map_i_train,:);
%     sM_tool = sM_tool_nontest;
    
    
    %% Shuffle aff and tool coords to obtain random model => baseline prediction
    if baseline_test == 1
        rand_tool_i = randperm(length(tool_labels_train));
        tool_data_train = tool_data_train(rand_tool_i,:);

        rand_aff_i = randperm(length(aff_labels_train));
        aff_data_train = aff_data_train(rand_aff_i,:);    
    end
    
    %% Learn mapping between toolfeat coordinates and aff coordinates.
    if strcmp(regr_mode, 'gnn')
        [net_feat2aff{runI}, ~] = gnnRegAff(tool_data_train, aff_data_train, [0 0]);
        net_feat2aff{runI}.userdata = mask;
        %[net_feat2aff{runI}, ~] = gnnRegAff(tool_bmu_train, aff_bmu_train, [0 0]);
        %[net_aff2feat, ~] = gnnRegAff(aff_bmu_train, tool_bmu_train, [1 1]);
    elseif strcmp(regr_mode, 'svm')
        [net_feat2aff{runI}, ~] = svmRegAff(tool_bmu_train, aff_bmu_train, [], 1);
        %[net_aff2feat, ~] = svmRegAff(tool_bmu_train, aff_bmu_train, [], 1);
    end
    
    %% Predict the affordance vectors for the test tool-feature samples
    % Predict the mapping from the tool SOM to the affordance SOM
    net = net_feat2aff{runI};
    aff_test_pred_affvector{runI} = net(tool_data_test')';
    aff_train_pred_affvector{runI}  = net(tool_data_train')';

    %% Compute the error in the prediction in  a few different ways:
    % AFFORDANCE ERROR 
    % sample by sample absolute error of predicted data wrt to recorded one.
    aff_pred_abs_error(runI) = mean(mean(abs(aff_data_test-aff_test_pred_affvector{runI})));
    aff_pred_abs_error_train(runI) = mean(mean(abs(aff_data_train-aff_train_pred_affvector{runI})));
    
    fprintf('\n Mean Abs Error %f \n', aff_pred_abs_error(runI));
    fprintf('\n Mean Abs Error Train %f \n', aff_pred_abs_error_train(runI));

end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
fprintf('==================================================================\n');
fprintf('Mean Abs Error %f \n', mean(aff_pred_abs_error));

if saveF
    save_filename = ['nosom_pred_aff_', robot '_' datadiv_mode num2str(runI) '_' feat_t '_' regr_mode];
    if (baseline_test == 1)
        save_filename = [save_filename '_baseline'];
    end
    save_filename
    save(save_filename,'net_feat2aff', 'aff_test_pred_affvector', 'aff_train_pred_affvector', 'tool_labels')
    
    
    fprintf(res_file, '%s \n', save_filename);
    fprintf(res_file, '%f \n', mean(aff_pred_abs_error));

end
if strcmp(datadiv_mode,'1out')
    if (aff_data_fig~= 0)
        plotPredAffVectors(aff_test_pred_affvector,tool_labels, aff_data_fig)
    end
end


fclose(res_file);