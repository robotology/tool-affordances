function [sampleIndices,cellIndices,sC] = clusterSOMhierarch(sMap, sData, K, mode, plotF)

if ((nargin < 3)||isempty(K))
   K = 10;
end
if ((nargin < 4)||isempty(mode))
    mode = 'average';
end
if ((nargin < 5)||isempty(plotF))
    plotF = 0;
end

fprintf('Clustering set to %d clusters using %s mode\n', K, mode);
sC = som_cllinkage(sMap, mode);
cellIndices = cluster(sC.tree,'MaxClust', K);
bmus = som_bmus(sMap,sData);
sampleIndices = cellIndices(bmus);        % Gets the clusteer for each data sample 

if any(plotF)
    for i = 1:K
        h(:,i) = som_hits(sMap,sData.data(sampleIndices==i,:));    
    end
    
    sMapClass = som_autolabel(sMap,sData,'add',1);
    figure(plotF);    
    cc = parula(length(unique(sampleIndices)));    
    somH = som_show(sMapClass,'bar','none', 'colormap', 1-gray, 'color',{cellIndices,sprintf('Clustering set to  %d clusters',K)}); % visualize
    hitH = som_show_add('hit',h,'MarkerColor',cc,'Subplot','all');
end