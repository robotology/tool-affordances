% plotAffDataVsPred()

load ws_pred_sim_1out50_OMSEGI_gnn.mat
 
 
simF = 1;
% read and print aff data
[aff_data, tp_labels]  = readAffData(1, 10);

numRep = 4;
for t = 1:length(tools)
    toolpose_labels{t} = tp_labels((t-1)*(3*numRep)+1:t*(3*numRep));    
end
plotPredAffVectors(aff_test_pred_affvector,toolpose_labels,simF)