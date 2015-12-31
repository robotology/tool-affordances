clear
close all

t =1;

kN = 3;
kernI = 0;

readDataFolders3D 
[trainDataAff, trainDataFeat, trainLabels, testDataAff, testDataFeat, testLabels, classLabels] = splitData(dataAff, dataFeat, toolLabels);
% [~, ~, ~, ~, ~, ~, classLabels] = splitData(dataAff, dataFeat, toolLabels);
%  trainDataAff= dataAffsAll;
%  testDataAff = dataAffsAll;
%  trainDataFeat= dataFeatsAll;
%  testDataFeat = dataFeatsAll;
%  trainLabels = classLabels;
%  testLabels = classLabels;


bestDB = 1000000;
bestInd = 1;
bestKhits = zeros(1, max(kN));
numKit = 100;
DB = zeros(length(kN), numKit);
for kI=1:numKit;
    disp(['Clustering round: ',num2str(kI)]);
    [trainKMlabels, centroids, dbInd] = KMclustering(trainDataAff,trainLabels, kN,0);
    [bestDBtrial,bestIndTrial] = min(dbInd); % select the one with smallest index
    bestKhits(kN(bestIndTrial)) = bestKhits(kN(bestIndTrial))+1; % populate the hit histogram
    DB(:,kI) = dbInd;
    if (bestDBtrial < bestDB)
        bestDB = bestDBtrial;
        bestInd = bestIndTrial;
        bestK = kN(bestInd);
        bestCentroid = centroids{bestInd};
        bestLabels = trainKMlabels{bestInd};
    end
end
DBavg = mean(DB,2);
figure;
subplot(2, 3, 1);bar(bestKhits);
subplot(2, 3, 2);bar(kN,DBavg);
subplot(2, 3, 3);plot(DB);
for bi=1:bestK
    subplot(2, bestK, bestK+bi);
    plot(-5:5,bestCentroid(bi,:)');    
    ylim([0,0.25])    
    xlabel('approach [cm]');
    ylabel('effect [m]');
end
drawnow;

for p=1:length(dataAff)
    figure(50)
    toolposeKMlabels = findCluster(dataAff{p},bestCentroid);    
    tpClasses = unique(toolposeKMlabels);
    subplot(Npose,Ntools,mod((p-1),Npose)*Ntools+ceil(p/Npose));
    y= -5:1:5;
    colorList ={'b','r','g','y','m','c','k','w'};
    hold on;
    for c=tpClasses'
        plot(y,dataAff{p}(toolposeKMlabels==c,:)', colorList{c});
    end
    hold off;
end

if (t == 1) % Select the number of clusters from the DB index
    
    %[dummy,bestK_ind] = min(dbInd); % select the one with smallest index
    disp(['Best number of clusters: ',num2str(bestK), ', with DB Index of ', num2str(bestDB)]);
    %clustNumBest = kN(bestK);
    [model, bestParam] = trainSVMaff(trainDataFeat, bestLabels);        
    testKMlabels = findCluster(testDataAff,bestCentroid);
    [predClass,confusionMatrix, finalAcc] = testSVMaff(model, testDataFeat, testKMlabels,2);   
    
    predAff = zeros(length(predClass), size(testDataAff,2));
    for v =1:length(predClass)
        predAff(v,:) = bestCentroid(predClass(v),:);
    end
    poseErr= sqrt(mean(mean((testDataAff-predAff).^2)));
    disp(['Sq. MSE: ',num2str(poseErr)]);
elseif (t == 2) % Select the number of clusters from the SCM accuracty on training set
    for k =1:length(kN)        
        [models{k}, bestParam] = trainSVMaff(trainDataFeat, trainKMlabels{k});
        [out, ~, acc(k)] = testSVMaff(models{k},trainDataFeat, trainKMlabels{k}) ;
    end
    figure;plot(kN,acc);title('Evaluation classification accuracy for N clusters');xlabel('N');
    [dummy,bestK_ind] = max(acc); % select the one with smallest index
    testKMlabels = findCluster(testDataAff,centroids{bestK_ind});    
    model = models{bestK_ind}; 
    [predClass,confusionMatrix, finalAcc] = testSVMaff(model, testDataFeat, testKMlabels,2);
end
