%% set control variables.
clear
close all
warning('off', 'all')
res_file = fopen('exp_results_deep.txt', 'w');
saveF = 0;


%                          datadiv      baseline/regr_mode
%               REAL 
param_mat = [                 0                   0;           0             1;      %OMSEGI RAND
                              1                   0;           1             1;      %OMSEGI 1OUT
            ];
                 

                  
for (exp_i = 1:size(param_mat,1))
    params = param_mat(exp_i,:);


%Select data separation method
if (params(1) == 0)
    datadiv_mode = 'rand';
elseif (params(1) == 1)
    datadiv_mode = '1out';
elseif (params(1) == 2)
    datadiv_mode = 'tool';
end

baseline_test = params(2);
%baseline_test = 1;

regr_mode = 'gnn';
%regr_mode = 'svm';

robot = 'real';
source = 'dumpers';

datadiv_ratio = 4;
noshov =0;
numRuns = 50;
simF = 0;

aff_data_fig = 0;
aff_SOM_fig =  0;                    %if > 100, affVectors will be plotted on top
aff_SOM_fig_test = 0;                %if > 100, affVectors will be plotted on top
tool_SOM_fig_train =  0;             %if > 100, toolpose images will be plotted on top
tool_SOM_fig_nontest =  0;           %if > 100, toolpose images will be plotted on top

%% Read the data

% Read the Affordance data
[aff_data, aff_labels]  = readAffData(simF,aff_data_fig,source);

% Read the tool 3D features
[tool_data, tool_labels, tp_labels]  = readDeepFeats();
toolIms = readToolImages('./figs/toolIms_trans/real');

%% map feat with aff vectors
map_i = map_aff2tool(aff_labels, tp_labels);

%% Select the kind of loop to do
tools = unique(tool_labels,'stable');
if strcmp(datadiv_mode,'1out')
    numRuns  = length(tools);    
end

exp_name = ['exp' robot '_' datadiv_mode num2str(numRuns) '_deep_' regr_mode];
if (baseline_test == 1)
    exp_name = [exp_name '_baseline'];
end

% allocate memory for speed and control
net_feat2aff = cell(numRuns,1);
sM_aff = cell(numRuns,1);   sM_tool = cell(numRuns,1); 
aff_data_test = cell(numRuns,1);    aff_data_train = cell(numRuns,1);
aff_labels_test = cell(numRuns,1);  aff_labels_train = cell(numRuns,1);
tool_data_test = cell(numRuns,1);   tool_data_train = cell(numRuns,1);  tool_data_nontest = cell(numRuns,1);
tp_labels_test = cell(numRuns,1);   tp_labels_train = cell(numRuns,1);  tp_labels_nontest = cell(numRuns,1);

aff_test_pred_affvector = cell(numRuns,1); aff_train_pred_affvector = cell(numRuns,1);
aff_test_pred_bmu_coords = cell(numRuns,1); aff_train_pred_bmu_coords = cell(numRuns,1);
aff_pred_abs_error = zeros(1,numRuns); aff_pred_abs_error_train = zeros(1,numRuns);

%% loop through tool or repetitions
for runI = 1:numRuns


    %% divide data_train data_test
    % Get masks for tool-wise separation:    
    fprintf('\n =========== On experiment %s \n', exp_name);
    if strcmp(datadiv_mode,'rand')
        mask = [];        
        fprintf('---------------------------------------------- Running for run %i \n', runI)
    elseif strcmp(datadiv_mode,'1out')
        mask = tools(runI)                
        fprintf('---------------------------------------------- Running for run %i \n', runI)
    elseif strcmp(datadiv_mode,'tool')
        mask_i = randperm(length(tools));
        mask = tools(mask_i(1:round(length(tools)/datadiv_ratio)))
        fprintf('---------------------------------------------- Running for run %i \n', runI)
    end
    
    
    %[tool_data_train, tool_data_test, test_ids] = splitData(tool_data, tp_labels_tool, datadiv_mode , 4,1, test_ids);
    % get indices
    [aff_train_ids, aff_test_ids] = getIndices(aff_labels, datadiv_mode , datadiv_ratio, mask); % divide all aff_data into training and testing 75/25  %
    tool_test_ids = map_i(aff_test_ids);                                            % corresponding tool indices for test samples of aff-data
    %tool_train_ids = map_i(aff_train_ids);                                          % corresponding tool indices for train samples of aff-data
    
    tool_nontest_ids = 1:size(tool_data,1);
    tool_nontest_ids(tool_test_ids) = [];                                           % all the remaining non-test samples from tool-data
    
    
    % split data
    [aff_data_train{runI}, aff_data_test{runI}, aff_labels_train{runI}, aff_labels_test{runI}] = splitData(aff_data, aff_labels, aff_test_ids);
    % indices of non-test samples of tool-data corresponding with the training ones from aff-data 
    [tool_data_nontest{runI}, tool_data_test{runI}, tp_labels_nontest{runI}, tp_labels_test{runI}] = splitData(tool_data, tp_labels, tool_test_ids);
    % extract those samples from the tool train data for which we do have a corresponding aff vector.
    %[tool_data_train{runI}, tool_data_test{runI}, tp_labels_train{runI}, tp_labels_test{runI}] = splitData(tool_data, tool_labels, tool_test_ids, tool_train_ids);
    

    toolIms_nontest = [];

    %% Map the data onto SOM
    % Map the tool features onto SOM
    % we use the whole non-test tool dataset for initial mapping, including also those trials not mapped to an affordance vector, to get a better mapping.    
    %[ tool_bmu_train, clust_tool_train_ids, som_coords_tool_train, sM_tool{runI}]= dimred2map(tool_data_train{runI}, tp_labels_train{runI}, 'tool', 'som', toolIms, tool_SOM_fig_train);
    [tool_bmu_nontest, clust_tool_nontest_ids, som_coords_tool_nontest, sM_tool{runI}]  = dimred2map(tool_data_nontest{runI}, tp_labels_nontest{runI}, 'tool', 'som', toolIms_nontest, tool_SOM_fig_nontest);
    
    % Map the affVectors onto SOM
    [ aff_bmu_train, clust_aff_ids, som_coords_aff,  sM_aff{runI}] = dimred2map(aff_data_train{runI}, aff_labels_train{runI}, 'aff', 'som', [], aff_SOM_fig);
    
    map_i_train = map_aff2tool(aff_labels_train{runI}, tp_labels_nontest{runI});              % indices of non-test samples of tool-data corresponding with the training ones from aff-data
    tool_bmu_train  = tool_bmu_nontest(map_i_train,:);                          % extract those samples from the tool mapping for which we do have a corresponding aff vector.
    tool_data_train{runI} = tool_data_nontest{runI}(map_i_train,:);
    tp_labels_train{runI} = tp_labels(map_i_train);    

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
        %[net_aff2feat, ~] = svmRegAff(tool_bmu_train, aff_bmu_train, [], 1);
    end
    
    %% Predict the affordance vectors for the test tool-feature samples   
    [aff_test_pred_affvector{runI}, aff_test_pred_bmu_coords{runI}] = som2som_predict(net_feat2aff{runI}, sM_tool{runI}, sM_aff{runI}, tool_data_test{runI}, regr_mode);
    [aff_train_pred_affvector{runI}, aff_train_pred_bmu_coords{runI}] = som2som_predict(net_feat2aff{runI}, sM_tool{runI}, sM_aff{runI}, tool_data_train{runI}, regr_mode);
    
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

exp_mae(exp_i) = mean(aff_pred_abs_error);

save_filename = ['ws_pred_', robot '_' datadiv_mode num2str(numRuns) '_deep_' regr_mode];
if (baseline_test == 1)
    save_filename = [save_filename '_baseline'];
end
if saveF == 1
    save_filename
    save(save_filename,'net_feat2aff', 'sM_aff', 'sM_tool', 'tools', ...            
        'aff_test_pred_affvector', 'aff_train_pred_affvector', 'aff_pred_abs_error', 'aff_pred_abs_error_train' )
    %'aff_data', 'aff_labels', 'tool_data', 'tools_labels', 'tool_labels',  ...
        %'aff_data_test', 'aff_data_train', 'aff_labels_train', 'aff_labels_test',  ...        
        %'tool_data_test', 'tool_data_train','tool_data_nontest',  'tool_labels_train', 'tool_labels_nontest', 'tool_labels_test', ...
        

    fprintf(res_file, '%s \n', save_filename);
    fprintf(res_file, '%f \n', mean(aff_pred_abs_error));

end

end

save params_and_maes param_mat exp_mae

fclose(res_file);