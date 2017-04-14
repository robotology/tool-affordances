function [target_test_pred_affvector, target_test_pred_bmu_coords] = som2som_predict(net, sM_source, sM_target, input_data_test)


%% Predict the affordance vectors for the test tool-feature samples
% Find the mapping on tool SOM for the test samples (BMU, index of cell in SOM)
sD_tool_test = som_data_struct(input_data_test,'name','tool_test_data');
[~,tool_test_bmu_coords] = get_bmu_coords(sM_source, sD_tool_test);

% Predict the mapping from the tool SOM to the affordance SOM
target_test_pred_bmu_coords = net(tool_test_bmu_coords')';

% Look for the closest neuron to the predicted position
som_coords_aff = som_unit_coords(sM_target);           % get neuron coordinates
som_coords_aff(:,2) = - som_coords_aff(:,2) + sM_target.topol.msize(1);
aff_test_pred_bmu_neurons = dsearchn(som_coords_aff, target_test_pred_bmu_coords); 

% Get its prototype vector = predicted affordance vector
target_test_pred_affvector = sM_target.codebook(aff_test_pred_bmu_neurons,:);
