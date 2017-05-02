% If workspace/variables are loaded, comment until "TRAIN"
%% Running mode variables
% Script to run all the necessary functions
clear;
close all;
canonF = 0; % 1 to work with canonical features, 0 for oriented ones
sourceF = 2; % 0 for REAL data, 1 for SIM data, 2 for both
featsF = 0;  % 0 for OMSEGI, 1 for OCCUP
dataSplit = 1; % 0 for random tool-pose split for train and test, 1 for tool-wise split

plotF = 0;  % 0 minimal plot, 1 plot everything
singleModel = 0;

Kmin = 5;
Kmax = 18;
runs = 20;

%% Read and prepare data
% Read the feats and affordance data.
if (sourceF ==0)    
    [feats3D, actDataPose, effDataPose, graspDataPose, toolNames, numPoses] = readAffData3D(featsF, canonF); % REAL data
elseif (sourceF ==1)
    [feats3D, actDataPose, effDataPose, graspDataPose, toolNames, numPoses] = readAffData3D(10 + featsF, canonF); % SIM data
else
    [feats3D_real, actDataPose_real, effDataPose_real, graspDataPose_real, toolNames_real, numPoses_real] = readAffData3D(featsF, canonF); % REAL data
    [feats3D_sim, actDataPose_sim, effDataPose_sim, graspDataPose_sim , toolNames_sim, numPoses_sim] = readAffData3D(10 + featsF, canonF); % SIM data
    feats3D = cat (1, feats3D_real, feats3D_sim);
    actDataPose = cat (3, actDataPose_real, actDataPose_sim);
    effDataPose= cat (3, effDataPose_real, effDataPose_sim);
    graspDataPose = cat (3, graspDataPose_real, graspDataPose_sim);
    toolNames = [ toolNames_real; toolNames_sim];
    numPoses = numPoses_real +  numPoses_sim;

end

%% =========================== FIND BEST K =========================================
% Find the best number of K based on the predicive power of the models.
meanMAEtrain = nan(Kmax,runs);
meanMAEmeansTrain = nan(Kmax,runs);
weightedMAEmeansTrain = nan(Kmax,runs);
meanMAPEtrain = nan(Kmax,runs);
meanMAPEmeansTrain = nan(Kmax,runs);
weightedMAPEmeansTrain = nan(Kmax,runs);
predTrainErrorAllAvg = nan(Kmax,runs);

%% Run this 'runs' times and select the K for which the average error measure is lower.
for Krun = 1:runs
    
    % Split data into training and testing samples
    if dataSplit ==0
        fprintf('\n Splitting data at 80 / 20 random\n');
        splitToolAffData;
    elseif dataSplit == 1
        fprintf('\n Splitting data by tool\n');
        divideDataByTool;
    end
    
    % Make SOM from training data
    [sMap,sData] = makeSOM(feats3Dtrain,toolNamesTrain, 20);

%% Train classifier sets for each considered K
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
        figure(51); clf;
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
        % Plot prediction errors on training        
        predTrainErrorAll = [];
        figure(120);clf;
        %% For each considered K, train all the models with data from each cluster.
        for modelI = 1:nClusters
            % Get the predictions from the learnt models and denormalize.
            sizeM = length(trainInReal{modelI});
            if (sizeM ~= 0)
                trainOutPredNorm = net{nClusters}{modelI}(affInputNorm{modelI}')';
                trainOutPred{nClusters}{modelI} = trainOutPredNorm.*repmat(sigNormOut{modelI},sizeM,1) + repmat(muNormOut{modelI},sizeM,1);

                % Reshape wrt. to action parameter for visualization
                trainInputM = reshape(trainInReal{modelI}(:,1), 8, sizeM/8);    % input
                trainOutRealM = reshape(trainOutReal{modelI}(:,1), 8, sizeM/8); % real data
                trainOutPredM = reshape(trainOutPred{nClusters}{modelI}(:,1), 8, sizeM/8); % predicted data

                meanTrainInputM = mean(trainInputM,2);          % input
                meanTrainRealM = mean(trainOutRealM,2);         % real
                meanTrainPredM = mean(trainOutPredM,2);         % predicted

                % Plot prediction against real data (TRAIN)
                if (plotF)
                    figure(120);
                    set(120,'name','Prediction: TRAIN');
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
                predTrainErrorVec{modelI} = abs(trainOutPred{nClusters}{modelI}-trainOutReal{modelI});  % sample by sample absolute error of predicted data wrt to recorded one.
                MAEtrain(modelI) = mean(predTrainErrorVec{modelI});                           % Mean Absolute Error for each model
                MAEmeansTrain(modelI) = mean(abs(meanTrainPredM-meanTrainRealM));                  % mean of the absolute difference between the means of real and predicted data
                MAPEtrain(modelI) = mean(predTrainErrorVec{modelI}./trainOutPred{nClusters}{modelI});         % Mean Absolute Percentage Error for each model
                MAPEmeansTrain(modelI) = mean(abs(meanTrainPredM-meanTrainRealM)./meanTrainPredM);      % mean of the relative difference between the means of real and predicted data

                predTrainErrorAll = [predTrainErrorAll;predTrainErrorVec{modelI}];       % Simple concatenation of absolute error at each point between real and predicted
            end
        end
        
        % Compute average errors among all models, for clustering with K clusters
        meanMAEtrain(nClusters,Krun) = mean(MAEtrain(MAEtrain ~= 0));
        meanMAEmeansTrain(nClusters,Krun) = mean(MAEmeansTrain(MAEmeansTrain ~= 0));
        meanMAPEtrain(nClusters,Krun) = mean(MAPEtrain(MAPEtrain ~= 0));
        meanMAPEmeansTrain(nClusters,Krun) = mean(MAPEmeansTrain (MAPEmeansTrain ~=0));
        
        % For the previously averaged, weight by samples on each cluser
        HH = hist(clusterIdx, max(clusterIdx));
        weightedMAEmeansTrain(nClusters,Krun) = sum(MAEmeansTrain.*HH)/sum(HH);%
        weightedMAPEmeansTrain(nClusters,Krun) = sum(MAPEmeansTrain.*HH)/sum(HH);% weighted mean of error between averages, weighted by the number of
        
        predTrainErrorAllAvg(nClusters,Krun) = mean(predTrainErrorAll);
        
        fprintf('\n ================== \n MEAN ERROR MEASURES FOR N = %d \n', nClusters);
        fprintf('mean MAE = %g\n', mean(MAEtrain))
        fprintf('mean MAE means = %g\n', mean(MAEmeansTrain))
        fprintf('mean MAPE = %g\n', mean(MAPEtrain))
        fprintf('mean MAPE means = %g \n', mean(MAPEmeansTrain))
        fprintf('mean Abs Error = %g \n\n', mean(predTrainErrorAll))
        
        drawnow;
        clear MAEmeansTrain MAEtrain MAPEtrain MAPEmeansTrain
        
    end;
    
    % Plot the error measurments for each K on each run
    figure(130)
    set(130,'name','Error - K plots');
    title('Average error among models w.r.t. K')
    subplot(2,4,1);plot(meanMAEtrain(:,Krun));title('MAE train')
    subplot(2,4,2);plot(meanMAEmeansTrain(:,Krun));title('MAE means train')
    subplot(2,4,3);plot(weightedMAEmeansTrain(:,Krun));title('weighted MAE means train')
    subplot(2,4,4);plot(meanMAPEtrain(:,Krun));title('MAPE train');
    subplot(2,4,5);plot(meanMAPEmeansTrain(:,Krun));title('MAPE means train')
    subplot(2,4,6);plot(weightedMAPEmeansTrain(:,Krun));title('weighted MAPE means train')
    subplot(2,4,7);plot(predTrainErrorAllAvg(:,Krun));title('Avg abs pred error')
end
 
%% save average among all runs of error measures for all possible Ks 
save errorByK_simreal_tool_OMSEGI meanMAEtrain meanMAEmeansTrain weightedMAEmeansTrain meanMAPEtrain meanMAPEmeansTrain  weightedMAPEmeansTrain predTrainErrorAllAvg
% remember that at IIT, the file should be renamed errorByK_simreal_tool_OCCUP

% plot them
figure(170);
set(170,'name','Error - K plots');
title('Average error among models w.r.t. K')
subplot(4,2,1);plot(meanMAEtrain);hold on; plot(mean(meanMAEtrain,2), 'r', 'Linewidth' , 2);title('MAE train');hold off;
subplot(4,2,2);plot(meanMAEmeansTrain);hold on;plot(mean(meanMAEmeansTrain,2), 'r', 'Linewidth' , 2);title('MAE means train');hold off;
subplot(4,2,3);plot(weightedMAEmeansTrain);hold on;plot(mean(weightedMAEmeansTrain,2), 'r', 'Linewidth' , 2);title('weighted MAE means train');hold off;
subplot(4,2,4);plot(meanMAPEtrain);hold on;plot(mean(meanMAPEtrain,2), 'r', 'Linewidth' , 2);title('MAPE train');hold off;
subplot(4,2,5);plot(meanMAPEmeansTrain);hold on;plot(mean(meanMAPEmeansTrain,2), 'r', 'Linewidth' , 2);title('MAPE means train');hold off;
subplot(4,2,6);plot(weightedMAPEmeansTrain);hold on;plot(mean(weightedMAPEmeansTrain,2), 'r', 'Linewidth' , 2);title('weighted MAPE means train');hold off;
subplot(4,2,7);plot(predTrainErrorAllAvg);hold on;plot(mean(predTrainErrorAllAvg,2), 'r', 'Linewidth' , 2);title('Avg abs pred error');hold off;


[bestError,bestK] = min(mean(meanMAEtrain,2));   

fprintf('Search for best K finished with source %d, featsF %d and dataSplit method %d\n', sourceF,featsF, dataSplit);

fprintf('Best K number is %d, with min mean MAE error %g\n', bestK, bestError);

