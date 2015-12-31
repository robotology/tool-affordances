%splitTooAffData

% Randomly select training and testing sample indices
trainTestRatio = 4;
numTrialsTest = ceil(numPoses/trainTestRatio);
testIdx = randsample(numPoses, numTrialsTest );
trainIdx = 1:numPoses;
trainIdx(testIdx) = [];

% Split train data
feats3Dtrain = feats3D(trainIdx,:);
graspDataTrain = graspDataPose(:,:,trainIdx);
actDataTrain = actDataPose(:,:,trainIdx);
effDataTrain = effDataPose(:,:,trainIdx);

% Split test data
feats3Dtest = feats3D(testIdx,:);

graspDataTest = graspDataPose(:,:,testIdx);
actDataTest = actDataPose(:,:,testIdx);
effDataTest = effDataPose(:,:,testIdx);

%split labels for further use:
toolNamesTrain = toolNames(trainIdx);
toolNamesTest = toolNames(testIdx);