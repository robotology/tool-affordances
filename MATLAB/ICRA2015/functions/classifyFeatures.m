clear
close all
readDataFolders

[trainDataAff, trainDataFeat, trainLabels, testDataAff, testDataFeat, testLabels, classLabels] = splitData(dataAff, dataFeat, toolLabels);

kN = 4;
[trainKMlabelsFeat, centroidsFeat, dbIndFeat] = KMclustering(trainDataFeat,trainLabels, kN,100);
[trainKMlabelsAff, centroidsAff, dbIndAff] = KMclustering(trainDataAff,trainLabels, kN,200);
[dummy,bestKFeat] = min(dbIndFeat); % select the one with smallest index
[dummy,bestKAff] = min(dbIndAff);
labelsFeatKM = trainKMlabelsFeat{bestKFeat};

labelsAffKM = trainKMlabelsAff{bestKAff};

[confusionMatrix,order] = confusionmat(labelsFeatKM,labelsAffKM);
confMat= confusionMatrix'; % Taspose to generate the conventional confMat: column=actual, row = predicted.
totalAccuracy = trace(confusionMatrix)/length(trainLabels);
disp(['Similarity between Feat and Aff clustering ',num2str(totalAccuracy*100),'%']);

%[model, bestParam] = trainSVMaff(trainDataFeat,trainLabels);
%[predClass,confusionMatrix, finalAcc] = testSVMaff(model, testDataFeat, testLabels,2);


if (plotF == 1)

    figure; 
    % plot predictions  
    subplot(2,4,1); imagesc(labelsFeatKM); title('Feat KM labels'); ylabel('observations'); 
    subplot(2,4,2); imagesc(labelsAffKM); title('Aff KM labels');ylabel('observations');

    % plot confusion matrix and class relevance
    subplot(2,2,2);
    imagesc(confusionMatrix');
    xlabel('Feat KM label');
    ylabel('Aff KM label');
    colorbar;

    subplot(2,2,3);
    % plot sample labeling in 2D space projection 
    colorList = hsv(length(unique(trainLabels)));
    distanceMatrix = pdist(trainDataFeat,'euclidean'); % compute distance betweeen all points
    newCoor = cmdscale(distanceMatrix); % 
    colorClass = colorList(labelsFeatKM,:);% true (ground truth) class

    % Plot the test data
    x = newCoor(:,1);
    y = newCoor(:,2);
    patchSize = 20; %80*max(decis_values,[],2);
    scatter(x,y,patchSize,colorClass,'o','filled');
    title('2D projection of feature data')
    
    subplot(2,2,4);
    % plot sample labeling in 2D space projection 
    colorList = hsv(length(unique(trainLabels)));
    distanceMatrix = pdist(trainDataAff,'euclidean'); % compute distance betweeen all points
    newCoor = cmdscale(distanceMatrix); % 
    colorClass = colorList(labelsAffKM,:);% true (ground truth) class

    % Plot the test data
    x = newCoor(:,1);
    y = newCoor(:,2);
    patchSize = 20; %80*max(decis_values,[],2);
    scatter(x,y,patchSize,colorClass,'o','filled');
    title('2D projection of effect data')
 
end