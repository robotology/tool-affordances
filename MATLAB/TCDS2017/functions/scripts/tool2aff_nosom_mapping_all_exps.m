%% set control variables.
clear
close all
warning('off', 'all')
saveF = 1;

res_file = fopen('exp_results_noSom3.txt', 'w');



%               simF       feat_type      datadiv      baseline/regr_mode
%               REAL
param_mat = [     0            0              0             0          ;   0            0              0             1;
                  0            0              1             0          ;   0            0              1             1;
                 %0            0              2             0           ;   0            0              2             1;
                  0            1              0             0          ;   0            1              0             1;
                  0            1              1             0          ;   0            1              1             1;
                 %0            1              2             0           ;   0            1              2             1;
                  0            2              0             0          ;   0            2              0             1;
                  0            2              1             0          ;   0            2              1             1;
                 %0            2              2             0           ;   0            2              2             1;    
                %SIM
                   1            0              0             0;   1            0              0             1;      %OMSEGI RAND
                   1            0              1             0;   1            0              1             1;      %OMSEGI 1OUT
%                   1            0              2             0;   1            0              2             1;     %OMSEGI TOOL 
                   1            1              0             0;   1            1              0             1;      %EGI RAND
                   1            1              1             0;   1            1              1             1;      %EGI 1OUT
%                   1            1              2             0;   1            1              2             1;     %EGI TOOL   
                   1            2              0             0;   1            2              0             1;      %OCCUP RAND
                   1            2              1             0;   1            2              1             1;      %OCCUP 1OUT
%                   1            2              2             0;   1            2              2             1;     %OCCUP TOOL    

            ];

                  
for (exp_i = 1:size(param_mat,1))
    params = param_mat(exp_i,:);

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

regr_mode = 'gnn';
baseline_test = params(4);
datadiv_ratio = 4;
numRuns = 50;

aff_data_fig = 0;
aff_SOM_fig = 0;               %if > 100, affVectors will be plotted on top
aff_SOM_fig_test = 0;          %if > 100, affVectors will be plotted on top
tool_SOM_fig_train = 0;        %if > 100, toolpose images will be plotted on top

%% Read the data
% Read the tool 3D features
[tool_data_aug, tools_labels_aug, tp_labels_aug, toolIms]  = read3Dfeatures(feat_type, simF);

% Read the Affordance data
[aff_data, aff_labels]  = readAffData(simF,aff_data_fig, source);


%% map feat with aff vectors
map_i = map_aff2tool(aff_labels, tp_labels_aug);
% Keep only tool data with matching aff-vectors (i.e., remove data augmentation vectors)
tool_data = tool_data_aug(map_i,:);
tools_labels = tools_labels_aug(map_i);
tp_labels = tp_labels_aug(map_i);


%% Select the kind of loop to do
tools = unique(tools_labels, 'stable');
if strcmp(datadiv_mode,'1out')
    numRuns  = length(tools);    
end

runs = 1:numRuns;   
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

exp_name = ['exp_nosom_' robot '_' datadiv_mode num2str(numRuns) '_' feat_t '_' regr_mode];
if (baseline_test == 1)
    exp_name = [exp_name '_baseline'];
end

%% loop through tool or repetitions
for runI = runs

    %% divide data_train data_test
    % Get masks for tool-wise separation:
    fprintf('\n =========== On experiment %s \n', exp_name);
    if strcmp(datadiv_mode,'rand')
        mask = [];        
        fprintf('---------------------------------------------- Running for run %i \n', runI)
        [aff_train_ids, aff_test_ids] = getIndices(aff_labels, datadiv_mode , datadiv_ratio, mask); % divide all aff_data into training and testing 
        [aff_data_train{runI}, aff_data_test{runI}, aff_labels_train{runI}, aff_labels_test{runI}] = splitData(aff_data, aff_labels, aff_test_ids, aff_train_ids);

        tool_test_ids = map_aff2tool(aff_labels_test{runI}, tp_labels);        
        [tool_data_train{runI}, tool_data_test{runI}, tp_labels_train{runI}, tp_labels_test{runI}] = splitData(tool_data, tp_labels, tool_test_ids);                                             
  
        
    elseif  strcmp(datadiv_mode,'1out')
        mask = tools(runI)                
        fprintf('---------------------------------------------- Running for run %i \n', runI)
        % get indices
        [aff_train_ids, aff_test_ids] = getIndices(aff_labels, datadiv_mode , datadiv_ratio, mask); % divide all aff_data into training and testing 
        [tool_train_ids, tool_test_ids] = getIndices(tp_labels, datadiv_mode , datadiv_ratio, mask);  % divide all tool_data into training and testing
        [aff_data_train{runI}, aff_data_test{runI}, aff_labels_train{runI}, aff_labels_test{runI}] = splitData(aff_data, aff_labels, aff_test_ids, aff_train_ids);    
        [tool_data_train{runI}, tool_data_test{runI}, tp_labels_train{runI}, tp_labels_test{runI}] = splitData(tool_data, tp_labels, tool_test_ids, tool_train_ids);                                             
   

    elseif strcmp(datadiv_mode,'tool')
        mask_i = randperm(length(tools));
        mask = tools(mask_i(1:round(length(tools)/datadiv_ratio)))
        fprintf('---------------------------------------------- Running for run %i \n', runI)
    
    end                                    % all the remaining non-test samples from tool-data


    % split data
    %[aff_data_train, aff_data_test, aff_labels_train, aff_labels_test] = splitData(aff_data, aff_labels, aff_test_ids, aff_train_ids);
    % extract those samples from the tool train data for which we do have a corresponding aff vector.
    %[tool_data_train{runI}, tool_data_test{runI}, tp_labels_train{runI}, tp_labels_test{runI}] = splitData(tool_data, tp_labels, tool_test_ids, tool_train_ids);                                             
   
    % Dividie images too
    toolim_test_id  = map_aff2tool(tp_labels_test, toolIms(3,:),1);
    toolIms_test = toolIms(:,toolim_test_id);
    toolim_train_ids = map_aff2tool(tp_labels_train, toolIms(3,:),1);
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
        rand_tool_i = randperm(length(tp_labels_train{runI}));
        tool_data_train{runI} = tool_data_train{runI}(rand_tool_i,:);

        rand_aff_i = randperm(length(aff_labels_train{runI}));
        aff_data_train{runI} = aff_data_train{runI}(rand_aff_i,:);    
    end
    
    %% Learn mapping between toolfeat coordinates and aff coordinates.
    if strcmp(regr_mode, 'gnn')
        [net_feat2aff{runI}, ~] = gnnRegAff(tool_data_train{runI}, aff_data_train{runI}, [0 0]);
        net_feat2aff{runI}.userdata = mask;
        %[net_feat2aff{runI}, ~] = gnnRegAff(tool_bmu_train, aff_bmu_train, [0 0]);
        %[net_aff2feat, ~] = gnnRegAff(aff_bmu_train, tool_bmu_train, [1 1]);
    elseif strcmp(regr_mode, 'svm')
        %Does not wor, would need a separate SVM per input dimension
        [svm_X, ~] = svmRegAff(tool_bmu_train, aff_bmu_train(:,1), [], 0);
        [svm_Y, ~] = svmRegAff(tool_bmu_train, aff_bmu_train(:,2), [], 0);
        net_feat2aff{runI} =  {svm_X, svm_Y};
        net_feat2aff{runI}{3} = mask;           % save the name of the tool trained without
        %[net_aff2feat, ~] = svmRegAff(tool_bmu_train, aff_bmu_train, [], 1);
    end
    
    %% Predict the affordance vectors for the test tool-feature samples
    % Predict the mapping from the tool SOM to the affordance SOM
    net = net_feat2aff{runI};
    aff_test_pred_affvector{runI} = net(tool_data_test{runI}')';
    aff_train_pred_affvector{runI}  = net(tool_data_train{runI}')';

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
fprintf('Mean Abs Error %f \n', mean(aff_pred_abs_error));

if saveF
    save_filename = ['nosom_pred_aff_', robot '_' datadiv_mode num2str(runI) '_' feat_t '_' regr_mode];
    if (baseline_test == 1)
        save_filename = [save_filename '_baseline'];
    end
    save_filename
    save(save_filename,'net_feat2aff', 'aff_test_pred_affvector', 'aff_train_pred_affvector', 'tp_labels')
    
    
    fprintf(res_file, '%s \n', save_filename);
    fprintf(res_file, '%f \n', mean(aff_pred_abs_error));
    fprintf(res_file, '%f \n', std(aff_pred_abs_error));

end

end

fclose(res_file);