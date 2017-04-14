function plotPredAffVectors(aff_test_pred_affvector,toolpose_labels,simF)
%% Plot the predicted affordances:
affvector_means = [];
affvector_stds = [];
tp_i = 1;
tp_labels = {};
for t = 1:length(aff_test_pred_affvector)
    affs_tool = aff_test_pred_affvector{t};
    for i = 1:3
        if (simF ==1)
            aff_toolpose = affs_tool(4*(i-1)+1:4*(i),:);        % select the trials corresponding to a tool pose
            tp_label = toolpose_labels{t}(4*i);
        else
            aff_toolpose = affs_tool(4*(i-1)+1:4*(i),:);        % select the trials corresponding to a tool pose
            tp_label = toolpose_labels{t}(4*i);
        end
        tpMat(:,:,tp_i) = aff_toolpose;
        affvector_means  = [affvector_means; mean(aff_toolpose)];
        affvector_stds  = [affvector_stds; std(aff_toolpose)];
        tp_labels = [tp_labels tp_label];
        tp_i = tp_i+1;
    end
end

% XXX Check that consistency between trials an dlabels hold after 'unique', which returns alphabetically
aff_data_fig = 10;
% tpLabels  = unique(toolpose_labels, 'sorted');
% Plot the data by tool type:
 color_test = 'r';
 plotTPaffs(affvector_means(1:30,:),affvector_stds(1:30,:), tp_labels(1:30), aff_data_fig , color_test)
 plotTPaffs(affvector_means(31:60,:),affvector_stds(31:60,:), tp_labels(31:60), aff_data_fig + 1, color_test)
 plotTPaffs(affvector_means(61:90,:),affvector_stds(61:90,:), tp_labels(61:90), aff_data_fig + 2, color_test)
 plotTPaffs(affvector_means(91:120,:),affvector_stds(91:120,:), tp_labels(91:120), aff_data_fig + 3, color_test)
 plotTPaffs(affvector_means(121:150,:),affvector_stds(121:150,:), tp_labels(121:150), aff_data_fig + 4, color_test)
color = 'r' ;
predF = 1;
% 
% if simF
% %Plot data from each tooltype-pose in a single plot:
%     plotTTPaffs(tpMat(:,:,1:30), 'hoes',0, color,predF);
%     plotTTPaffs(tpMat(:,:,31:60), 'hooks',1, color,predF);
%     plotTTPaffs(tpMat(:,:,61:90), 'rakes',2, color,predF);
%     plotTTPaffs(tpMat(:,:,91:120), 'sticks',3, color,predF);
%     plotTTPaffs(tpMat(:,:,121:150), 'shovels',4, color,predF);
% else
%     plotTTPaffs(tpMat(:,:,1:9), 'hoes',0, color, predF);
%     plotTTPaffs(tpMat(:,:,10:18), 'hooks',1, color, predF);
%     plotTTPaffs(tpMat(:,:,19:27), 'rakes',2, color, predF);
%     plotTTPaffs(tpMat(:,:,28:36), 'sticks',3, color, predF);
%     plotTTPaffs(tpMat(:,:,36:45), 'shovels',4, color, predF);
% end