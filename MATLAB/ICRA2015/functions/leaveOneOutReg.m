clear
close all
readDataFolders

poseErr = zeros(1, length(dataAff));
for p=1:length(dataAff)
    % Split the data on a leave-one-out fashion
    [trainDataAff, trainDataFeat, trainLabels, testDataAff, testDataFeat, testLabels] = splitPoses(dataAff, dataFeat,p);
    [net, bestSpread] = gnnRegAff(trainDataFeat, trainDataAff, 0);
    
    % Test on the remainig pose 
    out = net(testDataFeat');
    poseErr(p) = sqrt(mse(net,testDataAff',out));
    
    figure(30)
    subplot(Npose,Ntools,mod((p-1),Npose)*Ntools+ceil(p/Npose));
    y= -5:1:5;
    plot(y,mean(testDataAff,1), 'b');hold on;
    plot(y,mean(out,2), 'r');hold off;    
    title( ['Pred. Eff. ', toolLabels{p}(1,:)], 'interpreter','none');
    xlabel('approach [cm]');
    ylabel('effect [m]');
    
    
    figure(31)
    subplot(Npose,Ntools,mod((p-1),Npose)*Ntools+ceil(p/Npose));
    y= -5:1:5;
    plot(y,testDataAff', 'b');hold on;
    plot(y,out, 'r');hold off;    
    title( ['Pred. Eff. ', toolLabels{p}(1,:)], 'interpreter','none');
    xlabel('approach [cm]');
    ylabel('effect [m]');
    
end
figure;plot(poseErr);
title( 'Mean square error per tool pose');
ylabel('MSE [m]');
xlabel('toolpose index');
