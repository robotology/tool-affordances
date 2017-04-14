function [target_test_pred_affvector, target_test_pred_bmu_coords] = som2som_predict(net, sM_source, sM_target, input_data_test, regr_mode)

if ((nargin <5) || (isempty (regr_mode)))
    regr_mode = 'gnn';
end

%% Predict the affordance vectors for the test tool-feature samples
% Find the mapping on tool SOM for the test samples (BMU, index of cell in SOM)
sD_tool_test = som_data_struct(input_data_test,'name','tool_test_data');
[~,tool_test_bmu_coords] = get_bmu_coords(sM_source, sD_tool_test);

% Predict the mapping from the tool SOM to the affordance SOM
if strcmp(regr_mode, 'gnn')
    target_test_pred_bmu_coords = net(tool_test_bmu_coords')';
elseif strcmp(regr_mode, 'svm')
    coord_x = svmpredict(ones(size(tool_test_bmu_coords,1),1), tool_test_bmu_coords, net{1});
    coord_y = svmpredict(ones(size(tool_test_bmu_coords,1),1), tool_test_bmu_coords, net{2});
    target_test_pred_bmu_coords = [coord_x coord_y];
elseif strcmp(regr_mode, 'osvr')
    coord_x = predictSVR(net{1}, tool_test_bmu_coords, 0);
    coord_y = predictSVR(net{2}, tool_test_bmu_coords, 0);
    target_test_pred_bmu_coords = [coord_x coord_y];    
end

% Look for the closest neuron to the predicted position
% XXX look for them taking in account that not all are at same distance(U-MAT)
som_coords_aff = som_unit_coords(sM_target);           % get neuron coordinates
som_coords_aff(:,2) = - som_coords_aff(:,2) + sM_target.topol.msize(1);
aff_test_pred_bmu_neurons = dsearchn(som_coords_aff, target_test_pred_bmu_coords); 

% Get its prototype vector = predicted affordance vector
target_test_pred_affvector = sM_target.codebook(aff_test_pred_bmu_neurons,:);
