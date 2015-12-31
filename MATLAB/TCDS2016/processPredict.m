%dataDir = 'C:\ProgramsTanis\MATLAB\data\dataICRA\dataPredict\predictSim';
dataDir = 'C:\ProgramsTanis\MATLAB\data\dataICRA\dataPredict\predictRobot';
[dataEff,dataPred,dC,or,orI,toolpose] = readPredict(dataDir);

confusionMatrix = confusionmat(dC,orI)
figure ;imagesc(confusionMatrix');

%rMSE= sqrt(mean((dataPred-dataEff).^2));
predErr = mean(abs(dataEff-dataPred));
figure;plot(predErr);mean(predErr)

%we= [toolpose(orI ~= dC ) or(orI ~= dC ) ]'
%ww= [toolpose(orI == dC ) or(orI == dC ) ]'

moveAcc = length(dataEff(dataEff>0.05))/length(dataEff);