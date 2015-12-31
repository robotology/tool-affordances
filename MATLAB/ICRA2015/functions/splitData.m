function [trainDataAff, trainDataFeat, trainLabels, testDataAff, testDataFeat, testLabels, classLabels] = splitData(dataAff, dataFeats, toolLabels, trainToolIdx)
if (nargin < 4 )|| isempty(trainToolIdx)
    trainToolIdx = 1:length(dataAff);
end

trainTestRatio = 4;
trainDataAff = [];
testDataAff = [];
trainDataFeat = [];
testDataFeat = [];
classLabels =[];
trainLabels = [];
testLabels = [];

for i=trainToolIdx
    numTrials = size(dataAff{i}, 1);
    numLabels = size(toolLabels{i}, 1);
    if (numTrials ~= numLabels)
        error('On tool %d, Number of datapoints %d doesnt match the number of labels %d',i,numTrials,numLabels );
    end
    
    toolClass = i*ones(numTrials,1);
    numTrialsTest = ceil(numTrials/trainTestRatio);
    
    testIdx = randsample(numTrials, numTrialsTest );
    trainIdx = 1:numTrials;
    trainIdx(testIdx) = [];
    
    trainToolLabels = toolClass(trainIdx); 
    testToolLabels= toolClass(testIdx); 
    trainToolDataAff = dataAff{i}(trainIdx,:);
    testToolDataAff = dataAff{i}(testIdx,:);
    trainToolDataFeat = dataFeats{i}(trainIdx,:);
    testToolDataFeat = dataFeats{i}(testIdx,:);
    
    
    trainDataAff = [trainDataAff; trainToolDataAff];
    testDataAff = [testDataAff; testToolDataAff];
    trainDataFeat = [trainDataFeat; trainToolDataFeat];
    testDataFeat = [testDataFeat; testToolDataFeat];
    trainLabels = [trainLabels; trainToolLabels];
    testLabels = [testLabels; testToolLabels];
    classLabels = [classLabels; toolClass];
    
end
