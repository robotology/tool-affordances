clear
close all
readDataFolders
[trainDataAff, trainDataFeat, trainLabels, testDataAff, testDataFeat, testLabels, classLabels] = splitData(dataAff, dataFeat, toolLabels);

[model, bestSpread] = gnnRegAff(trainDataFeat, trainDataAff, 1);
outAll = model(testDataFeat');
totalErr = mse(model,testDataAff',outAll);
figure;
for tN=unique(testLabels)'
    toolTestDataFeat = testDataFeat(testLabels==tN,:)';
    toolTestDataAff = testDataAff(testLabels==tN,:)';
    out = model(toolTestDataFeat);
    toolErr(tN) = mse(model,toolTestDataAff,out);

    subplot(Npose,Ntools,mod((tN-1),Npose)*Ntools+ceil(tN/Npose));
    y= -5:1:5;
    plot(y,mean(toolTestDataAff,2), 'b');hold on;
    plot(y,mean(out,2), 'r');hold off;
    
    title( ['Pred. Eff. ', toolLabels{tN}(1,:)], 'interpreter','none');
    xlabel('approach [cm]');
    ylabel('effect [m]');
    
end
figure;plot(toolErr);
title( 'Mean square error per tool pose');
ylabel('MSE [m]');
xlabel('toolpose index');
% 
% %% Test with a completely new unseen tool 
% newToolIdx = [19 20 21];
% for poseCnt = 1:length(newToolIdx)
%     n = newToolIdx(poseCnt);
%     newToolPose = dataFeat{n};
%     outNew{poseCnt} = model(newToolPose');
%     subplot(1,3,poseCnt);
%     y= -5:1:5;
%     plot(y,mean(outNew{poseCnt},2));
%     %title( ['Pred. Eff. for new Tool ', toolLabels{n}(1,:)], 'interpreter','none');
%     xlabel('approach [cm]');
%     ylabel('effect [m]');
% end
