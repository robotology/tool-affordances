function [affInput, affOutput] = organizeByCluster(data,clusterIdx,labels,plotF)
numPoses = length(toolNames);

if ((nargin <2 )||isempty(clusterIdx))
    clusterIdx = ones(numPoses,1); % if no indices are given, consider all to belong to the same class (for testing)
end
if (nargin <5)
    plotF = 0;
end

numClusters = max(clusterIdx);
for m = unique(clusterIdx)'
    % Separe data by clusters kepping poses index
    posesClust = find(clusterIdx==m); 
    numPosesClust = length(posesClust); 
    modelDataGrasp= graspDataPose(:,:,clusterIdx==m);    
    modelDataAct= actDataPose(:,:,clusterIdx==m);
    modelDataEff= effDataPose(:,:,clusterIdx==m);   

    % concatenate data from all poses for each cluster
    dA = [];
    dG = [];
    dE = [];
    for t = 1: size(modelDataAct,3)
        dA = [dA;modelDataAct(:,:,t)];
        dG = [dG;modelDataGrasp(:,:,t)];
        dE = [dE;modelDataEff(:,:,t)];
    end
    
   affInput{m} = [dA(:,1) dG(:,2) repmat(m,length(dA(:,1)),1)];            
   
    affOutput{m} = dE(:,1);           % consider only displacement
    %affOutput{m} = [dE(:,1) dE(:,2)];  % Consider displacement and angle
    
end

if numClusters == 1
    affInput = affInput{1};
    affOutput = affOutput{1};
end