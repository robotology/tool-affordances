function [sampleIndices, mapping] = clusterTSNEhierarch(X, K, mode, plotF)

if ((nargin < 3)||isempty(K))
   %K = 10;
   K = round(intrinsic_dim(X))
end
if ((nargin < 4)||isempty(mode))
    mode = 'ward';
end
if ((nargin < 5)||isempty(plotF))
    plotF = 1;
end



fprintf('Clustering set to %d clusters using %s mode\n', K, mode);
[mappedX, mapping] = compute_mapping(X, 'tSNE');
distX = pdist(mappedX);
tree = linkage(distX,'ward');
sampleIndices = cluster(tree,'maxclust', K);

if any(plotF)
    for i = 1:K
        h(:,i) = som_hits(sMap,sData.data(sampleIndices==i,:));    
    end
    
    sMapClass = som_autolabel(sMap,sData,'add',1);
    figure(plotF);    
    cc = parula(K);    colorInd = zeros(length(clustIds), 3);
    for i = 1:c
        colorInd(clustIds == i,:) = repmat(cc(i,:), length(clustIds(clustIds == i)),1);
    end
    scatter(mappedX(:,1),mappedX(:,2),[],colorInd );
end