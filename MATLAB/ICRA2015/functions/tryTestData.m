function [predClass,testData] = tryTestData(data,model,  muNorm, sigNorm)

N = size(data,1);
%normalize data
testData = (data-repmat(muNorm,N,1))./repmat(sigNorm,N,1);
testData(isnan(testData)) = 0;

% predict the tool affordance from its feature vectors
%testLabel=0;
%[predict_label, accuracy, decis_values] = ovrpredictBot(testLabel, testData, model);
%[decis_value_winner, affClass] = max(decis_values,[],2);            
predClass = testSVMaff(model,testData);
%fprintf('Affordance cluster: %i \n\n', predClass);
