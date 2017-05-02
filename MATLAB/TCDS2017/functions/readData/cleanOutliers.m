function cleanData = cleanOutliers(data)
   
featN=size(data,2);
cleanData = data;
for feat=1:featN
    outlier = find(data(:,feat)>median(data(:,feat))+5*std(data(:,feat)));
    cleanData(outlier,feat)= median(data(:,feat));
    if (~isempty(outlier))
        for  o = outlier'
            fprintf('outlier found on feat %i, sample %i\n', feat, o)
        end
    end
end