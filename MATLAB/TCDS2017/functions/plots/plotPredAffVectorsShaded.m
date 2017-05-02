function plotPredAffVectorsShaded(aff_test_pred_affvector,toolpose_labels,aff_data_fig)
%% Plot the predicted affordances:

shadedErrorBar(0:45:315, aff_test_pred_affvector(4:33,:))
tpLabels  = unique(toolpose_labels);
% Plot the data by tool type:
plotTPaffsShaded(aff_test_pred_affvector(4:33,:), tpLabels(4:33), aff_data_fig )
plotTPaffsShaded(aff_test_pred_affvector(34:63,:), tpLabels(34:63), aff_data_fig + 1)
plotTPaffsShaded(aff_test_pred_affvector(64:93,:), tpLabels(64:93), aff_data_fig + 2)
plotTPaffsShaded(aff_test_pred_affvector(94:123,:), tpLabels(94:123), aff_data_fig + 3)
plotTPaffsShaded(aff_test_pred_affvector(127:156,:), tpLabels(127:156), aff_data_fig + 4)
