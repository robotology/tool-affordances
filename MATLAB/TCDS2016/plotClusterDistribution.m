
figure(110);
sDataTest = som_data_struct(feats3Dtest,'name','testData');
testBMUs = som_bmus(sMap,sDataTest);
testClusterIdx = somClusterIdx(testBMUs);
subplot(121)
trainClustDist = hist(clusterIdx, max(clusterIdx));
%pie(trainClustDist)
bar([trainClustDist;zeros(1,length(trainClustDist))], 'stack');
subplot(122)
testClustDist = hist(testClusterIdx, max(testClusterIdx));
%pie(hist(testClusterIdx, max(testClusterIdx)))
bar([testClustDist;zeros(1,length(testClustDist))], 'stack');
