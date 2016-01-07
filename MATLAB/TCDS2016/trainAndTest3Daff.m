% If workspace/variables are loaded, comment until "TRAIN"
%% Running mode variables
% Script to run all the necessary functions
clear;
close all;
canonF = 0; % 1 to work with canonical features, 0 for oriented ones
plotF = 1;
singleModel = 0;
Kmax = 20;
%% Read and prepare data
% Read the feats and affordance data. 
readDataFolders_exp3Daff

% Split data into training and testing samples
splitToolAffData

% Make SOM from training data
[sMap,sData] = makeSOM(feats3Dtrain, 20);

%% =========================== FIND BEST K =========================================
% Find the best number of K based on the predicive power of the models.


predTrainErrorAllAvg = zeros(1,Kmax);
for k = 2:Kmax
    %% +++++++++++++++++++++++++++++ CLUSTER ++++++++++++++++++++++++++++++++++++
    % Cluster tools by their features, and divide data according to which tool type generated it
    % Show data clustering and get cluster indices
    nClusters=k;
    [clusterIdx, somClusterIdx, clustStructure] = clusterSOMhierarch(sMap,sData,nClusters,'ward',50);
    % Do a comparison between 'ward' and complete. ward seems to render more
    % balanced clusters, but maybe that is not all good.


    %% %% +++++++++++++++++++++++++++++ TRAIN ++++++++++++++++++++++++++++++++++++
    % XXX Randomize cluster index to check baseline
    % clusterIdx = randi(nClusters, length(clusterIdx),1);

    % Separe the affData (grasp/action/effect) according to the clusters
    [trainInReal, trainOutReal] = organizeByCluster(graspDataTrain,actDataTrain,effDataTrain,toolNamesTrain,clusterIdx{nClusters}, canonF, singleModel, 100);
    plotClusterDistribution; % Script to visualize data distribution (train and test) among clusters.


    %% Learn an affordance model for each of the tool types
    if (singleModel ==1)
        affInputSingle = [];
        affOutputSingle = [];
        for modelI = 1:nClusters
            affInputSingle = [affInputSingle;trainInReal{modelI}];
            affOutputSingle = [affOutputSingle;trainOutReal{modelI}];
        end
        [affInputNormSingle,muNormInSingle, sigNormInSingle] = normalizeData(affInputSingle);
        [affOutputNormSingle,muNormOutSingle, sigNormOutSingle] = normalizeData(affOutputSingle);   
        fprintf('\n Training single model \n');
        [netSingle, bestSpreadSingle] = gnnRegAff(affInputNormSingle, affOutputNormSingle,[nClusters modelI]);  % gnn regresion
    end

    for modelI = 1:nClusters
        if (singleModel ==1)
            net{modelI} = netSingle;
            [affInputNorm{modelI}] = normalizeData(trainInReal{modelI}, muNormInSingle, sigNormInSingle);
            [affOutputNorm{modelI}] = normalizeData(trainOutReal{modelI}, muNormOutSingle, sigNormOutSingle); 
            muNormIn{modelI} = muNormInSingle;
            sigNormIn{modelI} = sigNormInSingle;
            muNormOut{modelI} = muNormOutSingle; 
            sigNormOut{modelI}= sigNormOutSingle;
        else
            % normalize train data to mean 0 and variance 1, and keep them values to norm test data
            [affInputNorm{modelI},muNormIn{modelI}, sigNormIn{modelI}] = normalizeData(trainInReal{modelI});
            [affOutputNorm{modelI},muNormOut{modelI}, sigNormOut{modelI}] = normalizeData(trainOutReal{modelI});  
            fprintf('\n Training model %i\n',modelI);
            [net{modelI}, bestSpread{modelI}] = gnnRegAff(affInputNorm{modelI}, affOutputNorm{modelI},[nClusters modelI]);  % gnn regresion
            %[model{modelI}, bestParam{modelI}] = svmRegAff(affInputNorm{modelI}, affOutputNorm{modelI}, [], plotF);        % svm Regression
        end
    end
    fprintf('Finished training all models\n');

    %% Plot prediction errors on training
    nClusters = max(clusterIdx);
    predTrainErrorAll = [];
    for modelI = 1:nClusters
        % Get the predictions from the learnt models and denormalize.
        sizeM = length(trainInReal{modelI});
        trainOutPredNorm = net{modelI}(affInputNorm{modelI}')';
        trainOutPred{modelI} = trainOutPredNorm.*repmat(sigNormOut{modelI},sizeM,1) + repmat(muNormOut{modelI},sizeM,1);

        trainOutPredM = reshape(trainOutPred{modelI}(:,1), 8, sizeM/8);
        trainOutRealM = reshape(trainOutReal{modelI}(:,1), 8, sizeM/8);   
        meanTrainRealM = mean(trainOutRealM,2);
        meanTrainPredM = mean(trainOutPredM,2);         

        if canonF == 1
            trainInputActM = reshape(trainInReal{modelI}(:,1), 8, sizeM/8);
            trainInputGrM = reshape(trainInReal{modelI}(:,2), 8, sizeM/8);

            spI = 1;
            numC = ceil(sqrt(nClusters));
            posGrasp = unique(trainInputGrM(1,:));
            for g = posGrasp
                figure (120)
                subplot(nClusters,length(posGrasp),(modelI-1)*length(posGrasp)+spI)         
                grId = find(trainInputGrM(1,:) == g);

                meanTrainInputM = mean(trainInputActM,2);
                meanTrainRealGrM = mean(trainOutRealM(:,grId),2);
                meanTrainPredGrM = mean(trainOutPredM(:,grId),2); 

                errorbar(meanTrainInputM,meanTrainPredGrM,std(trainOutPredM(:,grId),1,2),'b','linewidth', 2); hold on;
                errorbar(meanTrainInputM,meanTrainRealGrM,std(trainOutRealM(:,grId),1,2),'k','linewidth', 1);
                predErrorGraph = abs(meanTrainRealGrM-meanTrainPredGrM);
                plot(meanTrainInputM, predErrorGraph, 'r');hold off;

                % plot(squeeze(affVecGr(:,1,:)),squeeze(affVecGr(:,2,:)));hold on;
                %plot(mean(affVecGr(:,1,:),3),mean(affVecGr(:,2,:),3),'linewidth', 2);hold off;
                axis([-10 360 0 0.25])
                xlabel(['Action - Pull Angle- Grasp ', num2str(g)], 'FontSize',18);
                ylabel('Effect (cm)', 'FontSize',18); 
                spI = spI + 1;
            end       
        else        
            % Reshape wrt. to action parameter for visualization
            trainInputM = reshape(trainInReal{modelI}(:,1), 8, sizeM/8); 
            meanTrainInputM = mean(trainInputM,2);

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
        predTrainErrorVec{m} = abs(trainOutPred{modelI}-trainOutReal{modelI});  % sample by sample absolute error of predicted data wrt to recorded one.
        MAE(m) = mean(predErrorVec{m});                  % Mean Absolute Error for each model
        MAEmeans(m) = mean(abs(meanPredM-meanRealM));    % mean of the absolute difference between the means of real and predicted data
        MAPE(m) = mean(predErrorVec{m}./outPred{m});     % Mean Absolute Percentage Error for each model
        MAPEmeans(m) = mean(abs(meanPredM-meanRealM)./meanPredM); % mean of the relative difference between the means of real and predicted data

        predTrainErrorAll = [predTrainErrorAll;predTrainErrorVec{m}];  % Simple concatenation of absolute error at each point between real and predicted
    end

%     HH = hist(clusterIdx, max(clusterIdx));
%     if canonF ==1
%        HH= reshape(repmat(HH, 3,1), length(HH)*3,1)';
%     end
% 
%     weightedMAEmeans(K) = sum(MAEmeans.*HH)/sum(HH)% 
%     weightedMAPEmeans(K) = sum(MAPEmeans.*HH)/sum(HH)% weighted mean of error between averages, weighted by the number of

    predTrainErrorAllAvg(K) = mean(predTrainErrorAll)
    
end;

[bestError,bestK] = min(predTrainErrorAllAvg);
fprintf('Best K number is %d, with error %g\n', bestK, bestError)

% Generate fake output data with the possible actions to assess model quality
%imagineAffordance(net,graspDataTrain,actDataTrain,effDataTrain,clusterIdx, muNormIn, muNormOut, sigNormIn, sigNormOut, canonF, 150);

%% TEST
%% Find the model to apply for each test sample from the tool features
sDataTest = som_data_struct(feats3Dtest,'name','testData');
% Find each sample's best matching unit (BMU, index of cell in SOM)
bmus = som_bmus(sMap,sDataTest);
% Find the cluster/model to which weacg test tool corresponds.
testClusterIdx = somClusterIdx(bmus);

% Obtain the model prediction and compare to real effect.
[testInput, outReal] = organizeByCluster(graspDataTest,actDataTest,effDataTest,toolNamesTest,testClusterIdx, canonF,singleModel, 200);

%% Get predictions of the effect based on the model and the features. 
predErrorAll = [];
poseI = 1;
for m = unique(testClusterIdx)'
    sizeM = length(testInput{m});
    % Normalize the data with the norm variable sof its respective model.
    [testInputNorm] = normalizeData(testInput{m},muNormIn{m}, sigNormIn{m});
    [outRealNorm] = normalizeData(outReal{m},muNormOut{m}, sigNormOut{m});

    % Get the predictions from the learnt models and denormalize.
    outPredNorm = net{m}(testInputNorm')';
    outPred{m} = outPredNorm.*repmat(sigNormOut{m},sizeM,1) + repmat(muNormOut{m},sizeM,1);
    
    outPredM = reshape(outPred{m}, 8, sizeM/8);
    outRealM = reshape(outReal{m}, 8, sizeM/8);
    meanRealM = mean(outRealM,2);
    meanPredM = mean(outPredM,2); 
    
    if canonF ==1
        testInputActM = reshape(testInput{m}(:,1), 8, sizeM/8);
        testInputGrM = reshape(testInput{m}(:,2), 8, sizeM/8);
        
        spI = 1;
        numC = ceil(sqrt(nClusters));
        posGrasp = unique(testInputGrM(1,:));
        for g = posGrasp
            figure(220)
            subplot(nClusters,length(posGrasp),(m-1)*length(posGrasp)+spI);            
            grId = find(testInputGrM(1,:) == g);
            
            meanTestInputM = mean(testInputActM,2);
            outRealGrM = outRealM(:,grId);
            outPredGrM = outPredM(:,grId);
            
            meanRealGrM = mean(outRealGrM,2);
            meanPredGrM = mean(outPredGrM,2);                         
            
            errorbar(meanTestInputM,meanPredGrM,std(outPredGrM,1,2),'b','linewidth', 2); hold on;
            errorbar(meanTestInputM,meanRealGrM,std(outRealGrM,1,2),'k','linewidth', 1);
            predErrorGraph = abs(meanRealGrM-meanPredGrM);
            plot(meanTestInputM, predErrorGraph, 'r');hold off;

            % plot(squeeze(affVecGr(:,1,:)),squeeze(affVecGr(:,2,:)));hold on;
            %plot(mean(affVecGr(:,1,:),3),mean(affVecGr(:,2,:),3),'linewidth', 2);hold off;
            axis([-10 360 0 0.25])
            xlabel(['Action - Pull Angle- Grasp ', num2str(g)], 'FontSize',18);
            ylabel('Effect (cm)', 'FontSize',18); 
            spI = spI + 1;
            
            % Compare predictions to the real data to get the error
            RMSE(poseI)= sqrt(mean(mean((outPredGrM-outRealGrM).^2)));      % Average Root Mean Square Error of pair-wise prediction to each real output           
            RMSEmeans(poseI)= sqrt(mean((meanPredGrM-meanRealGrM).^2));               % Root Mean Square Error between the average pred and the average real output
            
            predErrorMat{poseI} = abs(outPredGrM-outRealGrM);    
            MAE(poseI) = mean(mean(predErrorMat{poseI}));                                  % Mean Absolute error for each model 
            MAEmeans(poseI) = mean(abs(meanPredGrM-meanRealGrM));
            MAPE(poseI) =mean(mean(predErrorMat{poseI}./outPredGrM));
            MAPEmeans(poseI) = mean(abs(meanPredGrM-meanRealGrM)./meanPredM);            
            

            poseI = poseI + 1;            
        end
        predErrorVec{m} = abs(outPred{m}-outReal{m});
        predErrorAll = [predErrorAll;predErrorVec{m}];        
    else        
        % Reshape wrt. to action parameter for visualization
        testInputM = reshape(testInput{m}(:,1), 8, sizeM/8);
        meanTestinputM = mean(testInputM,2);

        figure(220)
        subplot(ceil(sqrt(nClusters)),ceil(sqrt(nClusters)),m);    
        errorbar(meanTestinputM,meanPredM,std(outPredM,1,2),'b','linewidth', 2); hold on;
        errorbar(meanTestinputM,meanRealM,std(outRealM,1,2),'k','linewidth', 1);
        predErrorGraph = abs(meanRealM-meanPredM);
        plot(meanTestinputM, predErrorGraph, 'r');hold off;
        axis([-10 360 0 0.25])
        xlabel('Action - Pull Angle', 'FontSize',18);
        ylabel('Effect (cm)', 'FontSize',18);  
        
        % Compare predictions to the real data to get the error
        RMSEmeans(m)= sqrt(mean((meanPredM-meanRealM).^2));            % Root Mean Square Error between the average pred and the average real output
        RMSE(m)= sqrt(mean((outPred{m}-outReal{m}).^2));              % Average Root Mean Square Error of pair-wise prediction to each real output
        
        predErrorVec{m} = abs(outPred{m}-outReal{m});    
        MAE(m) = mean(predErrorVec{m});                                  % Mean Absolute error for each model
        MAEmeans(m) = mean(abs(meanPredM-meanRealM));
        MAPE(m) = mean(predErrorVec{m}./outPred{m});
        MAPEmeans(m) = mean(abs(meanPredM-meanRealM)./meanPredM);

        predErrorAll = [predErrorAll;predErrorVec{m}];

    end
 
%     predErrorModelNorm{m} = abs(outPredNorm{m}-outReal{m});            
end
%imagineAffordance(net,graspDataTest,actDataTest,effDataTest,testClusterIdx, muNormIn, muNormOut, sigNormIn, sigNormOut, canonF, 250);

%RMSE
%RMSEmeans

MAE = MAE*100 %(to put it in cm)
MAEmeans = MAEmeans*100 %(to put it in %)
MAPE = MAPE*100 %(to put it in cm)
MAPEmeans = MAPEmeans*100 %(to put it in %)

HH = hist(testClusterIdx, max(testClusterIdx));
if canonF ==1
   HH= reshape(repmat(HH, 3,1), length(HH)*3,1)';
end

weightedMAEmeans = sum(MAEmeans.*HH)/sum(HH)% 
weightedMAPEmeans = sum(MAPEmeans.*HH)/sum(HH)% weighted mean of error between averages, weighted by the number of

%predErrorAllAvg = mean(predErrorAll)

