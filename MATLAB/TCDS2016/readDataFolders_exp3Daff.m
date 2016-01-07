
%dataDir= 'C:\Users\Tanis Mar\Dropbox\experiments\data50tools\affData';
% dataDir= 'C:\Users\Tanis Mar\Dropbox\experiments\data50tools\featsNewBB';
dataDir= 'C:\Users\Tanis\Dropbox\experiments\data50tools\featsNewBB' 
dd=dir(dataDir);

dataFeats = [];
actData = [];
effData = [];
graspData = [];
toolLabels =[];


for dirI =1:length(dd)
    if (length(dd(dirI).name)>3) && (strcmp(dd(dirI).name(1:4),'tool'))
        filename = [dataDir,'\',dd(dirI).name];
        fprintf('\nReading data from folder %s \n', filename);
        tool = dd(dirI).name;
        actDataF = readActData(dataDir,tool);        
        graspDataF = readGraspData(dataDir,tool);
        effDataF= readEffData(dataDir,tool);
        [dataFeatsF, toolLabelsF]  = read3Dfeatures(dataDir,tool);
        
        dataFeats = [dataFeats; dataFeatsF];
        actData = [actData; actDataF];
        effData = [effData; effDataF];
        graspData = [graspData; graspDataF];
        toolLabels = [toolLabels; toolLabelsF];
    end      
end

% Clean data from outliers
actData = cleanOutliers(actData);
graspData = cleanOutliers(graspData);
effData = cleanOutliers(effData);
        
% Separe 3D features into the oriented and the canonical ones.
canonIdx = 1:10:size(dataFeats,1); 
canon3DFeats = dataFeats(canonIdx, :);
canonToolNames = toolLabels(canonIdx);
orient3DFeats = dataFeats;
orientToolNames = toolLabels;
orient3DFeats(canonIdx, :) = [];
orientToolNames(canonIdx, :) = [];

if (canonF==1)    
    feats3D = canon3DFeats;
    toolNames = canonToolNames;
else
    feats3D = orient3DFeats;
    toolNames = orientToolNames;
end

numPoses =length(toolNames);
if (canonF==1)
    graspDataPose=permute(reshape(graspData, 72,numPoses, 3), [1 3 2]);
    actDataPose=permute(reshape(actData, 72,numPoses, 2), [1 3 2]);
    effDataPose=permute(reshape(effData, 72,numPoses, 3), [1 3 2]);
else
    graspDataPose=permute(reshape(graspData, 8,numPoses, 3), [1 3 2]);
    actDataPose=permute(reshape(actData, 8,numPoses, 2), [1 3 2]);
    effDataPose=permute(reshape(effData, 8,numPoses, 3), [1 3 2]);   
end


clear actDataF graspDataF effDataF dataFeatsF toolLabelsF dirI 