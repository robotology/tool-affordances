clear
close all
readDataFolders3D


% Perform Clustering and train SVM based on clustering labels  
K = 3;
kN = 3;
[~, ~, ~, ~, ~, ~, classLabels] = splitData(dataAff, dataFeat, toolLabels);
bestDB = 1000000;
bestInd = 1;
bestKhits = zeros(1, max(kN));
numKit = 100;
DB = zeros(length(kN), numKit);
for kI=1:numKit;
    disp(['Clustering round: ',num2str(kI)]);
    [trainKMlabels, centroids, dbInd] = KMclustering(dataAffsAll,classLabels, kN,0);
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
figure(40);
pie(histc(bestLabels, unique(bestLabels)));
drawnow;

poseErr = zeros(1, length(dataAff));
acc = zeros(1, length(dataAff));
confusionMatrixTot = zeros(K);
for p=1:length(dataAff)
    % Split the data on a leave-one-out fashion
    [trainDataAff, trainDataFeat, trainLabels, testDataAff, testDataFeat, testLabels] = splitPoses(dataAff, dataFeat,p);
    trainKMlabels{p} = findCluster(trainDataAff,bestCentroid);    
    [model{p}, bestParam] = trainSVMaff(trainDataFeat, trainKMlabels{p}); 
    
    % Find test data clusters and evaulate model
    testKMlabels = findCluster(testDataAff,bestCentroid);
    [predClass,confusionMatrixP, acc(p)] = testSVMaff(model{p}, testDataFeat, testKMlabels);
    confusionMatrixTot = confusionMatrixTot + confusionMatrixP;
    
    
    predAff{p} = zeros(length(predClass), size(dataAff{p},2));
    for v =1:length(predClass)
        predAff{p}(v,:) = bestCentroid(predClass(v),:);
    end
    poseErr(p)= sqrt(mean(mean((testDataAff-predAff{p}).^2)));
    avgPred  = mean(predAff{p});
    
    figure(30)
    subplot(Npose,Ntools,mod((p-1),Npose)*Ntools+ceil(p/Npose));
    y= -5:1:5;
    plot(y,mean(testDataAff,1), 'b');hold on;
    plot(y,mean(predAff{p},1), 'r');hold off;    
    title( ['Pred. Eff. ', toolLabels{p}(1,:)], 'interpreter','none');
    xlabel('approach [cm]');
    ylabel('effect [m]');
    
    
    figure(31)
    subplot(Npose,Ntools,mod((p-1),Npose)*Ntools+ceil(p/Npose));
    y= -5:1:5;
    plot(y,testDataAff', 'b');hold on;
    plot(y,predAff{p}', 'r');
    plot(y,avgPred, 'g');hold off;   
    title( ['Pred. Eff. ', toolLabels{p}(1,:)], 'interpreter','none');
    xlabel('approach [cm]');
    ylabel('effect [m]');
    
end
figure;plot(poseErr);
title( 'Mean square error per tool pose');
xlabel('MSE [m]');
ylabel('toolpose index');
figure;plot(acc);
title( 'Mean Accuracy');
xlabel('Acc %');
ylabel('toolpose index');
meanErr= mean(poseErr);
meanAcc= mean(acc);

confusionMatrixTot
figure;
imagesc(confusionMatrixTot');

