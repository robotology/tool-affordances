% If workspace/variables are loaded, comment until "TRAIN"
%% Running mode variables
% Script to run all the necessary functions
clear;
close all;
canonF = 0; % 1 to work with canonical features, 0 for oriented ones
plotF = 1;
singleModel = 0;
Kmin = 10;
Kmax = 20;

% XXX possibly, run this like 20 times and select the K for which the
% average error measure is lower.

%% Read and prepare data
% Read the feats and affordance data. 
readDataFolders_exp3Daff

% Split data into training and testing samples
splitToolAffData

% Make SOM from training data
[sMap,sData] = makeSOM(feats3Dtrain, 20);

%% =========================== FIND BEST K =========================================
% Find the best number of K based on the predicive power of the models.
meanMAEtrain = nan(1,Kmax);
meanMAEmeansTrain = nan(1,Kmax);
weightedMAEmeansTrain = nan(1,Kmax);
meanMAPEtrain = nan(1,Kmax);
meanMAPEmeansTrain = nan(1,Kmax);
weightedMAPEmeansTrain = nan(1,Kmax);
predTrainErrorAllAvg = nan(1,Kmax);
for nClusters = Kmin:Kmax
    %% +++++++++++++++++++++++++++++ CLUSTER ++++++++++++++++++++++++++++++++++++
    % Cluster tools by their features, and divide data according to which tool type generated it
    % Show data clustering and get cluster indices    
    [clusterIdx,somClusterIdx,clusterStructure] = clusterSOMhierarch(sMap,sData,nClusters,'ward',50);
    % Do a comparison between 'ward' and complete. ward seems to render more
    % balanced clusters, but maybe that is not all good.


    %% ++++++++++++++++++++++++++ ORGANIZE DATA ++++++++++++++++++++++++++++++++++++
    % XXX Randomize cluster index to check baseline
    % clusterIdx = randi(nClusters, length(clusterIdx),1);

    % Separe the affData (grasp/action/effect) according to the clusters
    [trainInReal, trainOutReal] = organizeByCluster(graspDataTrain,actDataTrain,effDataTrain,toolNamesTrain,clusterIdx, canonF, singleModel, 100);
    plotClusterDistribution; % Script to visualize data distribution (train and test) among clusters.


    %% +++++++++++++++++++++++++++++ TRAIN ++++++++++++++++++++++++++++++++++++
    % Learn an affordance model for each of the tool types
    for modelI = 1:nClusters
        % normalize train data to mean 0 and variance 1, and keep them values to norm test data
        [affInputNorm{modelI},muNormIn{modelI}, sigNormIn{modelI}] = normalizeData(trainInReal{modelI});
        [affOutputNorm{modelI},muNormOut{modelI}, sigNormOut{modelI}] = normalizeData(trainOutReal{modelI});  
        fprintf('\n Training model %i\n',modelI);
        [net{nClusters}{modelI}, bestSpread{nClusters}{modelI}] = gnnRegAff(affInputNorm{modelI}, affOutputNorm{modelI},[nClusters modelI]);  % gnn regresion
        %[model{modelI}, bestParam{modelI}] = svmRegAff(affInputNorm{modelI}, affOutputNorm{modelI}, [], plotF);        % svm Regression
    end
    fprintf('Finished training all models\n');

    %% +++++++++++++++++++++++++++++ PREDICT ++++++++++++++++++++++++++++++++++++
    %% Plot prediction errors on training

    predTrainErrorAll = [];
    for modelI = 1:nClusters
        % Get the predictions from the learnt models and denormalize.
        sizeM = length(trainInReal{modelI});
        trainOutPredNorm = net{nClusters}{modelI}(affInputNorm{modelI}')';
        trainOutPred{modelI} = trainOutPredNorm.*repmat(sigNormOut{modelI},sizeM,1) + repmat(muNormOut{modelI},sizeM,1);

        % Reshape wrt. to action parameter for visualization
        trainInputM = reshape(trainInReal{modelI}(:,1), 8, sizeM/8);    % input        
        trainOutRealM = reshape(trainOutReal{modelI}(:,1), 8, sizeM/8); % real data  
        trainOutPredM = reshape(trainOutPred{modelI}(:,1), 8, sizeM/8); % predicted data
        
        meanTrainInputM = mean(trainInputM,2);          % input        
        meanTrainRealM = mean(trainOutRealM,2);         % real
        meanTrainPredM = mean(trainOutPredM,2);         % predicted
        
        % Plot prediction against real data (TRAIN)
        if (plotF)            
            figure (120)
            subplot(ceil(sqrt(nClusters)),ceil(sqrt(nClusters)),modelI);    
            errorbar(meanTrainInputM, meanTrainPredM,std(trainOutPredM,1,2),'b','linewidth', 2); hold on;
            errorbar(meanTrainInputM, meanTrainRealM,std(trainOutRealM,1,2),'k','linewidth', 1);
            predErrorGraph = abs(meanTrainRealM-meanTrainPredM);
            plot(meanTrainInputM, predErrorGraph, 'r');hold off;
            axis([-10 360 0 0.25])
            xlabel('Action - Pull Angle', 'FontSize',18);
            ylabel('Effect (cm)', 'FontSize',18);  
        end
       
        % Compute the error in the prediction in  a few different ways:
        predTrainErrorVec{modelI} = abs(trainOutPred{modelI}-trainOutReal{modelI});  % sample by sample absolute error of predicted data wrt to recorded one.
        MAEtrain(modelI) = mean(predTrainErrorVec{modelI});                           % Mean Absolute Error for each model        
        MAEmeansTrain(modelI) = mean(abs(meanTrainPredM-meanTrainRealM));                  % mean of the absolute difference between the means of real and predicted data        
        MAPEtrain(modelI) = mean(predTrainErrorVec{modelI}./trainOutPred{modelI});         % Mean Absolute Percentage Error for each model    
        MAPEmeansTrain(modelI) = mean(abs(meanTrainPredM-meanTrainRealM)./meanTrainPredM);      % mean of the relative difference between the means of real and predicted data        

        predTrainErrorAll = [predTrainErrorAll;predTrainErrorVec{modelI}];       % Simple concatenation of absolute error at each point between real and predicted
    end
    
    % Compute average errors among all models, for clustering with K clusters
    meanMAEtrain(nClusters) = mean(MAEtrain);           
    meanMAEmeansTrain(nClusters) = mean(MAEmeansTrain);
    meanMAPEtrain(nClusters) = mean(MAPEtrain);
    meanMAPEmeansTrain(nClusters) = mean(MAPEmeansTrain);
    
    % For the previously averaged, weight by samples on each cluser
    HH = hist(clusterIdx, max(clusterIdx));
    weightedMAEmeansTrain(nClusters) = sum(MAEmeansTrain.*HH)/sum(HH);% 
    weightedMAPEmeansTrain(nClusters) = sum(MAPEmeansTrain.*HH)/sum(HH);% weighted mean of error between averages, weighted by the number of

    predTrainErrorAllAvg(nClusters) = mean(predTrainErrorAll);
    
    fprintf('\n ================== \n MEAN ERROR MEASURES FOR N = %d \n', nClusters);
    fprintf('mean MAE = %g\n', mean(MAEtrain))
    fprintf('mean MAE means = %g\n', mean(MAEmeansTrain))
    fprintf('mean MAPE = %g\n', mean(MAPEtrain))
    fprintf('mean MAPE means = %g \n', mean(MAPEmeansTrain))
    fprintf('mean  Abs Error = %g \n\n', mean(predTrainErrorAll))
    
    drawnow;
    
end;


figure(130)
title('Average error among models w.r.t. K')
subplot(2,4,1);plot(meanMAEtrain);title('MAE train')
subplot(2,4,2);plot(meanMAEmeansTrain);title('MAE means train')
subplot(2,4,3);plot(weightedMAEmeansTrain);title('weighted MAE means train')
subplot(2,4,4);plot(meanMAPEtrain);title('MAPE train');
subplot(2,4,5);plot(meanMAPEmeansTrain);title('MAPE means train')
subplot(2,4,6);plot(weightedMAPEmeansTrain);title('weighted MAPE means train')
subplot(2,4,7);plot(predTrainErrorAllAvg);title('Avg abs pred error')

[bestError,bestK] = min(meanMAEtrain);
fprintf('Best K number is %d, with error %g\n', bestK, bestError)

%% TEST 
% Retrieve data separation, clustering and models for the best K.
[bestClusterIdx, bestSOMclusterIdx, clustStructure] = clusterSOMhierarch(sMap,sData,bestK,'ward',50);
bestModel = net{bestK};

%% Find the model to apply for each test sample from the tool features
sDataTest = som_data_struct(feats3Dtest,'name','testData');
% Find each sample's best matching unit (BMU, index of cell in SOM)
testBMUs = som_bmus(sMap,sDataTest);
% Find the cluster/model to which each test sample corresponds.
testClusterIdx = bestSOMclusterIdx(testBMUs);

% Obtain the model prediction and compare to real effect.
[trainInReal, trainOutReal] = organizeByCluster(graspDataTrain,actDataTrain,effDataTrain,toolNamesTrain,bestClusterIdx, canonF, singleModel, 100);
[testInput, outReal] = organizeByCluster(graspDataTest,actDataTest,effDataTest,toolNamesTest,testClusterIdx, canonF,singleModel, 200);

%% Get predictions of the effect based on the model and the features. 
predErrorAll = [];
poseI = 1;
for m = unique(testClusterIdx)'

    %% PLOT prediction data on TRAIN SET for comparison
    sizeM = length(trainInReal{m});    
    % Reshape wrt. to action parameter for visualization --- TRAIN DATA
    trainInputM = reshape(trainInReal{m}(:,1), 8, sizeM/8);    % input        
    trainOutRealM = reshape(trainOutReal{m}(:,1), 8, sizeM/8); % real data  
    trainOutPredM = reshape(trainOutPred{m}(:,1), 8, sizeM/8); % predicted data
    meanTrainInputM = mean(trainInputM,2);          % input        
    meanTrainRealM = mean(trainOutRealM,2);         % real
    meanTrainPredM = mean(trainOutPredM,2);         % predicted

    % Plot prediction against real data (TRAIN)
    if (plotF)  
        figure (120)
        subplot(ceil(sqrt(nClusters)),ceil(sqrt(nClusters)),m);    
        errorbar(meanTrainInputM, meanTrainPredM,std(trainOutPredM,1,2),'b','linewidth', 2); hold on;
        errorbar(meanTrainInputM, meanTrainRealM,std(trainOutRealM,1,2),'k','linewidth', 1);
        predErrorGraph = abs(meanTrainRealM-meanTrainPredM);
        plot(meanTrainInputM, predErrorGraph, 'r');hold off;
        axis([-10 360 0 0.25])
        xlabel('Action - Pull Angle', 'FontSize',18);
        ylabel('Effect (cm)', 'FontSize',18);  
    end
    
    %% COMPUTE and PLOT prediction on TEST SET for evaluation
    
    sizeM = length(testInput{m});    
    % Normalize the data with the norm variables of its respective model.
    [testInputNorm] = normalizeData(testInput{m},muNormIn{m}, sigNormIn{m});
    [outRealNorm] = normalizeData(outReal{m},muNormOut{m}, sigNormOut{m});
  
    % Get the predictions from the learnt models and denormalize.
    outPredNorm = bestModel{m}(testInputNorm')';
    outPred{m} = outPredNorm.*repmat(sigNormOut{m},sizeM,1) + repmat(muNormOut{m},sizeM,1);
    
    
    % Reshape wrt. to action parameter for visualization --- TEST DATA
    testInputM = reshape(testInput{m}(:,1), 8, sizeM/8);   % input        
    outRealM = reshape(outReal{m}, 8, sizeM/8);            % real output
    outPredM = reshape(outPred{m}, 8, sizeM/8);            % predicted output
    
    meanTestinputM = mean(testInputM,2);                   % input
    meanRealM = mean(outRealM,2);                          % real output
    meanPredM = mean(outPredM,2);                          % predicted output

    % Plot prediction against real data (TEST)    
    if plotF
        figure(220)
        subplot(ceil(sqrt(bestK)),ceil(sqrt(bestK)),m);    
        errorbar(meanTestinputM,meanPredM,std(outPredM,1,2),'b','linewidth', 2); hold on;
        errorbar(meanTestinputM,meanRealM,std(outRealM,1,2),'k','linewidth', 1);
        predErrorGraph = abs(meanRealM-meanPredM);
        plot(meanTestinputM, predErrorGraph, 'r');hold off;
        axis([-10 360 0 0.25])
        xlabel('Action - Pull Angle', 'FontSize',18);
        ylabel('Effect (cm)', 'FontSize',18);  
    end
    
    % Compare predictions to the real data to get the error
    RMSEmeans(m)= sqrt(mean((meanPredM-meanRealM).^2));            % Root Mean Square Error between the average pred and the average real output
    RMSE(m)= sqrt(mean((outPred{m}-outReal{m}).^2));              % Average Root Mean Square Error of pair-wise prediction to each real output

    predErrorVec{m} = abs(outPred{m}-outReal{m});    
    MAE(m) = mean(predErrorVec{m});                                  % Mean Absolute error for each model
    MAEmeans(m) = mean(abs(meanPredM-meanRealM));
    MAPE(m) = mean(predErrorVec{m}./outPred{m});
    MAPEmeans(m) = mean(abs(meanPredM-meanRealM)./meanPredM);

    predErrorAll = [predErrorAll;predErrorVec{m}];

%     predErrorModelNorm{m} = abs(outPredNorm{m}-outReal{m});            
end
%imagineAffordance(net,graspDataTest,actDataTest,effDataTest,testClusterIdx, muNormIn, muNormOut, sigNormIn, sigNormOut, canonF, 250);

%RMSE
%RMSEmeans

% MAE = MAE*100 %(to put it in cm)
% MAEmeans = MAEmeans*100 %(to put it in %)
% MAPE = MAPE*100 %(to put it in cm)
% MAPEmeans = MAPEmeans*100 %(to put it in %)

fprintf('\n =================== \nERROR MEASURES ON TEST, WITH N = %d \n', bestK);
figure(230)
title('Error measures for each model')
subplot(1,5,1);plot(MAE);title('MAE')
subplot(1,5,2);plot(MAEmeans);title('MAE means')
subplot(1,5,3);plot(MAPE);title('MAPE');
subplot(1,5,4);plot(MAPEmeans);title('MAPE means')

HH = hist(testClusterIdx, max(testClusterIdx));
weightedMAEmeans = sum(MAEmeans.*HH)/sum(HH);% 
weightedMAPEmeans = sum(MAPEmeans.*HH)/sum(HH);% weighted mean of error between averages, weighted by the number of

fprintf('mean MAE = %g\n', mean(MAE))
fprintf('mean MAE means= %g\n', mean(MAEmeans))
fprintf('weighted MAE means = %g\n',weightedMAEmeans)
fprintf('mean MAPE = %g\n', mean(MAPE))
fprintf('mean MAPE means= %g\n', mean(MAPEmeans))
fprintf('weighted MAPE means = %g\n', weightedMAPEmeans)
fprintf('mean Abs Error = %g\n', mean(predErrorAll))
predErrorAllAvg = mean(predErrorAll)
