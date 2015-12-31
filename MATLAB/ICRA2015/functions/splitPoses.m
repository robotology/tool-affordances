function [trainDataAff, trainDataFeat, trainLabels, testDataAff, testDataFeat, testLabels] = splitPoses(dataAff, dataFeats, testIdx)

numPoses = length(dataAff);
trainTestRatio = 4;
if (nargin < 3 )|| isempty(testIdx)
    testIdx = randsample(numPoses, ceil(length(dataAff)/trainTestRatio));
end

trainIdx = 1:numPoses;
trainIdx(testIdx) = [];

trainDataAff = [];
testDataAff = [];
trainDataFeat = [];
testDataFeat = [];
classLabels =[];
trainLabels = [];
testLabels = [];

for i=1:numPoses
    numAffs = size(dataAff{i}, 1);
    toolClass = i*ones(numAffs,1);
    
    if (ismember(i,trainIdx))    
        trainDataAff = [trainDataAff; dataAff{i}];    
        trainDataFeat = [trainDataFeat; dataFeats{i}(1:numAffs,:)];        
        trainLabels = [trainLabels; toolClass];    
    end
    if (ismember(i,testIdx))
        testDataAff = [testDataAff; dataAff{i}];    
        testDataFeat = [testDataFeat; dataFeats{i}(1:numAffs,:)];        
        testLabels = [testLabels; toolClass]; 
    end
end
