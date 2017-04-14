% We need to avoid taking in account the models which do not appear on the
% test set when computing the averages
for m = 1:bestK
    meanMAEruns(m) = mean(MAEtest(MAEtest(:,m) ~= 0,m));    
    meanMAPEruns(m) = mean(MAPEtest(MAPEtest(:,m) ~= 0,m));
    meanMAEmeansRuns(m) = mean(MAEmeansTest(MAEmeansTest(:,m) ~= 0,m));   
    meanMAPEmeansRuns(m) = mean(MAPEmeansTest(MAPEmeansTest(:,m) ~= 0,m));   
end
meanMAEruns*100
meanMAPEruns*100

for r = 1:runs
    meanMAEmodels(r) = mean(MAEtest(r,MAEtest(r,:) ~= 0));
    meanMAPEmodels(r) = mean(MAPEtest(r,MAPEtest(r,:) ~= 0));
    meanMAEmeansModels(r) = mean(MAEmeansTest(r,MAEmeansTest(r,:) ~= 0));
    meanMAPEmeansModels(r) = mean(MAPEmeansTest(r,MAPEmeansTest(r,:) ~= 0));
end

% For the previously averaged, weight by samples on each cluser
%HH = hist(testClusterIdx,bestK);
%weightedMAEmeansTest = sum(MAEmeansTest.*HH)/sum(HH);%
%weightedMAPEmeansTest = sum(MAPEmeansTest.*HH)/sum(HH);% weighted mean of error between averages, weighted by the number of samples per cluster
%predErrorTestAllAvg = mean(predErrorTestAll);

%% GET some statistics and plots among runs
if (plotF)    
    figure(320);
    set(320,'name','TEST ERROR measurements plots');
    title('TEST error by model model')    
    subplot(2,2,1);plot(MAEtest);hold on; plot(meanMAEmodels, 'r', 'Linewidth' , 2);title('MAE test');hold off;
    subplot(2,2,2);plot(MAEmeansTest);hold on;plot(meanMAEmeansModels, 'r', 'Linewidth' , 2);title('MAE means test');hold off;    
    subplot(2,2,3);plot(MAPEtest);hold on;plot(meanMAPEmodels, 'r', 'Linewidth' , 2);title('MAPE test');hold off;
    subplot(2,2,4);plot(MAPEmeansTest);hold on;plot(meanMAPEmeansModels, 'r', 'Linewidth' , 2);title('MAPE means test');hold off;
end
