function som2som_compare(sM_target, target_test_pred_affvector, target_test_pred_bmu_coords, target_data_test, plotF)

if (nargin < 5) || isempty(plotF)
    plotF = 0;
end

%% Compare these predicted affordances with the recorded ones.
sD_aff_test = som_data_struct(target_data_test,'name','aff_test_data');
[target_test_real_bmu_neurons, target_test_real_bmu_coords] = get_bmu_coords(sM_target, sD_aff_test);
target_test_real_affvector = sM_target.codebook(target_test_real_bmu_neurons,:);

if any(plotF)    
    red = zeros(size(target_test_pred_affvector,1),3); red(:,1) = 1;
    addMiniPlot(target_test_pred_affvector, target_test_pred_bmu_coords,[], 1, red , plotF, 2);

    green = zeros(size(target_test_real_affvector,1),3); green(:,2) = 1;
    addMiniPlot(target_data_test, target_test_real_bmu_coords,[], 1, green, plotF, 2);
end