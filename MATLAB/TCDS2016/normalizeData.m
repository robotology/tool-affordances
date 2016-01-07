% normalized data matrix (n samples x m features) to zero mean and unit variance.
function [normData,mu, sig] = normalizeData(data, mu, sig)
featN=size(data,2);
if (nargin<2)
    mu = zeros(1,featN);
    sig = zeros(1,featN);
    extValF =0;
else
    extValF =1;
end
% clean outliers (value > median(feat) + 10*std(feat))
normData = zeros(size(data));


for feat=1:featN
    outlier = find(data(:,feat)>median(data(:,feat))+10*std(data(:,feat)));
    data(outlier,feat)= median(data(:,feat));
    if (~isempty(outlier))
        for  o = outlier
            fprintf('outlier found on feat %i, sample %i\n', feat, o)
        end
    end
end
if extValF ==0
    [normData, mu, sig] = zscore(data);
else
    N= size(data,1);
    normData = (data - repmat(mu,N,1))./repmat(sig,N,1);
    normData(isnan(normData)) = 0;
end


%normData(:,sum(data).^2==0)=[];         % remove empty columns

%imagesc(normData); title('features');