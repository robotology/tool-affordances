%% set control variables.
clear
close all
warning('off', 'all')
saveF = 0;
plot_figs = 1;

%            robot       feat_type      datadiv      baseline
params = [     1            2              1             0]

%Select robot type. 0 = real, 1 = sim
simF = params(1);
if simF ==0
    robot = 'real';
else
    robot = 'sim';
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
    datadiv_mode = 'rand';          %Random division
elseif (params(3) == 1)
    datadiv_mode = 'rinc';          %Random divisoin incremental
elseif (params(3) == 2)
    datadiv_mode = '1out';          %1-out
elseif (params(3) == 3)
    datadiv_mode = 'tool';          %N-out
end

baseline_test = params(4);
%baseline_test = 1;

%regr_mode = 'gnn';
regr_mode = 'osvr';
%regr_mode = 'svm';



if plot_figs
    aff_data_fig = 1;
    aff_SOM_fig = 10;                   %if > 100, affVectors will be plotted on top
    tool_SOM_fig = 20;                   %if > 100, affVectors will be plotted on top
    pred_res_fig = 50;    
else    
    aff_data_fig =  0;
    aff_SOM_fig =  0;                    %if > 100, affVectors will be plotted on top
    tool_SOM_fig =  0;             %if > 100, toolpose images will be plotted on top
    pred_res_fig = 0; 
end

%% Read the data

% Read the Affordance data
[aff_data, aff_labels]  = readAffData(simF,aff_data_fig);

% Read the tool 3D features
[tool_data, tools_labels, tool_labels, toolIms]  = read3Dfeatures(feat_type, simF);

%% map feat with aff vectors
tool_ids_exp = map_aff2tool(aff_labels, tool_labels);       % indices of tool trials with matching aff-vector
tool_ids_aug = 1:length(tool_labels);
tool_ids_aug(tool_ids_exp) = [];                            % indices of tool trials with NO matching aff-vector
tool_labels_exp = tool_labels(tool_ids_exp);                % labels of tool trials with matching aff-vector
tool_labels_aug = tool_labels(tool_ids_aug);                % labels of tool trials with NO matching aff-vector

%% Select the kind of loop to do
tools = unique(tools_labels,'stable');
num_tools  = length(tools);    
exp_name = [robot '_' datadiv_mode num2str(num_tools) '_' feat_t '_online'];

% change the order in which tools are presented:
%rand_i = randperm(length(tools));
%tools = tools(rand_i);

datadiv_ratio = 4;
num_tools = 100;
%% loop through tool or repetitions
for tool_i = 1:num_tools
    
    %% XXX from here on changes to online learning     
    % testing is done alltimes by predicting the outcome of the next tool before using it. 
    % - Divide by taking data from one tool one tool.
    % After using it use the data to continue learning.    
    % To have some proper evaluation this would need to be run with a few random tool orders. 
    
    % for each tool
    % - Predict outcome 
    % - Map its feats to SOM
    % - Train OSVM with the BMU coords
    % - 


    %% divide data_train data_test
    if strcmp(datadiv_mode,'rinc')
        mask = tool_i;                
        datadiv_ratio = num_tools;        
        r_inds_aff = randperm(length(aff_labels));
        r_inds_tool = randperm(length(tool_labels));
        fprintf('---------------------------------------------- Running for run %i \n', tool_i)
    elseif strcmp(datadiv_mode,'1out')
        mask = tools(tool_i);
        r_inds_tool = [];
        r_inds_aff= [];
        fprintf('---------------------------------------------- Running for run %i: tool %s \n', tool_i, mask{1})
    end
    fprintf('\n =========== On experiment %s \n', exp_name);

    % get indices of the next tool to be learned
    % We only need the indices of the next tool. We will use them to test the system, and then to train train it
    [aff_train_ids, aff_test_ids] = getIndices(aff_labels, datadiv_mode , datadiv_ratio, mask, r_inds_aff); % divide all aff_data into training and testing 75/25  %
    %[tool_train_ids_som, tool_test_ids_som] = getIndices(tool_labels, datadiv_mode , datadiv_ratio, mask, r_inds_tool); % divide all aff_data into training and testing 75/25  %    
    
    % split aff and tool data
    [~, aff_data_next, ~, aff_labels_next] = splitData(aff_data, aff_labels, aff_test_ids);    
    
    tool_ids_test_exp = tool_ids_exp(aff_test_ids);        % only experimental samples
    tool_ids_aug_test_ptr =  map_aff2tool(aff_labels_next, tool_labels_aug,2);    % augmented and expereimental samples
    tool_ids_test_aug = tool_ids_aug(tool_ids_aug_test_ptr);
    
    [~, tool_data_next_som, ~ , tool_labels_next_som] = splitData(tool_data, tool_labels, tool_ids_test_aug);
    [~, tool_data_next_reg, ~ , tool_labels_next_reg] = splitData(tool_data, tool_labels, tool_ids_test_exp);
    
    % in order to train the SOM, we use the tool-pose augmented data, while
    % for training the regressor we need to keep the samples with a corresponding aff-vec
%    map_aug2real = map_aff2tool(aff_labels_next, tool_labels);          % indices of samples of tool-data corresponding with the training ones from aff-data    
    
 %   tool_data_next = tool_data(map_aug2real,:);
 %  tool_labels_next = tool_labels(map_aug2real);    
    
    %XXX check that no train/test overlap happens here
    
    %map_tool2aff = map_aff2tool(tool_labels,aff_labels,1);
    
    
    % Dividie images too
    if plot_figs
        toolim_next_som_ids = map_aff2tool(tool_labels_next_som, toolIms(3,:),1);
        toolim_next_reg_ids = map_aff2tool(tool_labels_next_reg, toolIms(3,:),1);
        toolIms_next_reg = toolIms(:,toolim_next_reg_ids);
        toolIms_next_som = toolIms(:,toolim_next_som_ids);
    else
        toolIms_next = [];
    end
    
    %% Test the tool before its used for further learning
    if tool_i ~= 1
        [aff_test_pred_affvector{tool_i}, aff_train_pred_bmu_coords{tool_i}] = som2som_predict(net_feat2aff{tool_i-1}, sM_tool, sM_aff, tool_data_next_reg, regr_mode);
        aff_pred_abs_error(tool_i) = mean(mean(abs(aff_data_next-aff_test_pred_affvector{tool_i})));
        figure(pred_res_fig)
        plot(aff_pred_abs_error);
    end
    
    %% After testing, learn next tool
    %% Map the data onto SOM
    % Map the tool features onto SOM   
    if tool_i == 1
        % Initialize Map with first tool
        [sM_aff, sD_aff, aff_bmu_coords] = trainSOMseq([],aff_data_next, aff_labels_next, 'aff', aff_SOM_fig);
        [sM_tool, ~, tool_bmu_aug] = trainSOMseq([],tool_data_next_som, tool_labels_next_som, 'tool', tool_SOM_fig, toolIms_next_som);                        
        [sM_tool, ~, tool_bmu_exp] = trainSOMseq(sM_tool,tool_data_next_reg, tool_labels_next_reg, 'tool', tool_SOM_fig, toolIms_next_reg);                        
    else
        [sM_aff, sD_aff, aff_bmu_coords] = trainSOMseq(sM_aff, aff_data_next, aff_labels_next,'aff' , aff_SOM_fig);
        [sM_tool, ~, tool_bmu_aug] = trainSOMseq(sM_tool, tool_data_next_som, tool_labels_next_som, 'tool', tool_SOM_fig, toolIms_next_som); 
        [sM_tool, ~, tool_bmu_exp] = trainSOMseq(sM_tool, tool_data_next_reg, tool_labels_next_reg, 'tool', tool_SOM_fig, toolIms_next_reg); 
    end    
    %tool_bmu_test  = tool_bmu_som(map_aug2real,:);                          % extract those samples from the tool mapping for which we do have a corresponding aff vector.
    tool_bmu_test = tool_bmu_exp;
 
    %% Learn mapping between toolfeat coordinates and aff coordinates.
    % XXX change by online SVM
    if strcmp(regr_mode, 'gnn')
        [net_feat2aff{tool_i}, ~] = gnnRegAff(tool_bmu_test, aff_bmu_coords, [0 0]);
        net_feat2aff{tool_i}.userdata = mask;
        %[net_aff2feat, ~] = gnnRegAff(aff_bmu_train, tool_bmu_train, [1 1]);
    elseif strcmp(regr_mode, 'svm')
        [svm_X, ~] = svmRegAff(tool_bmu_coords, aff_bmu_coords(:,1), [], 0);
        [svm_Y, ~] = svmRegAff(tool_bmu_coords, aff_bmu_coords(:,2), [], 0);
        net_feat2aff{tool_i} =  {svm_X, svm_Y};
    elseif strcmp(regr_mode, 'osvr')
        svr_X = trainSVR(tool_bmu_test, aff_bmu_coords(:,1), 5);
        svr_Y = trainSVR(tool_bmu_test, aff_bmu_coords(:,2), 5);
        net_feat2aff{tool_i} =  {svr_X, svr_Y};
    end
    
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
fprintf('==================================================================\n');
fprintf('Exp Mean Abs Error %f \n', mean(aff_pred_abs_error));

if saveF
    save_filename = ['som2som_predreal_aff_', robot '_' datadiv_mode num2str(num_tools) '_' feat_t '_' regr_mode];
    if (baseline_test == 1)
        save_filename = [save_filename '_baseline'];
    end
    save_filename
    save(save_filename,'net_feat2aff', 'sM_aff', 'sM_tool', ...    
        'aff_data_test', 'aff_data_train', 'aff_labels_train', 'aff_labels_test',  ...
        'tool_data_test', 'tool_data_train','tool_data_nontest',  'tool_labels_train', 'tool_labels_nontest', 'tool_labels_test', 'tools',...
        'aff_test_pred_affvector', 'aff_train_pred_affvector', 'aff_pred_abs_error', 'aff_pred_abs_error_train' )
end

if strcmp(datadiv_mode,'1out')
    if (aff_data_fig~= 0)
        plotPredAffVectors(aff_test_pred_affvector,tool_labels, aff_data_fig)
    end
end