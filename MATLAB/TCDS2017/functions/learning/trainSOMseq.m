function [sM, sD, bmu_coords] = trainSOMseq(sM, data,labels, name, plotF, toolIms)

if isempty(sM)
    sM = som_make(data, 'msize', [15 25], 'shape', 'sheet', 'name',name, 'tracking', 0);
end
if nargin < 6
    toolIms = [];
end
figure(plotF+1)
sM = som_seqtrain(sM,data,'tracking',1);
sD = som_data_struct(data,'name',name, 'labels', labels');
[~, bmu_coords, som_coords] = get_bmu_coords(sM, sD);

if any(plotF)
    %Find activation color by tool category and pose
    clust_ids = clustByLabel(labels, [1 3]); % tool names are between ids 1 and 3        
    color_ids = clusterColor(clust_ids);
    
    % plot hexagonal SOM with activations
    figure(plotF);
    sMapClass = som_autolabel(sM,sD,'add',1);
    som_show(sMapClass,'umat','all','bar','none', 'colormap', 1-gray); % visualize
    K = max(clust_ids);
    for i = 1:K
        h(:,i) = som_hits(sM,sD.data(clust_ids==i,:));    
    end
    cc = jet(K);
    som_show_add('hit',h,'MarkerColor',cc,'Subplot','all');  
    
    if (plotF > 100)
        %plot scatter version for miniPlot
        ht = figure(plotF+5); ht.Position = [0 0 1000 700];
        scatter(som_coords(:,1),som_coords(:,2),10, '.k'); axis off; hold on;
        scatter(bmu_coords(:,1),bmu_coords(:,2),30,color_ids, 'filled' ); hold off;drawnow;   
    
        if strcmp(name, 'tool')            
            ht.Name = 'SOM tools';
            addMiniPlot(data, bmu_coords,labels, 2, toolIms, plotF+5, 15);    
        elseif strcmp(name, 'aff')
            ht.Name = 'SOM aff';
            addMiniPlot(data, bmu_coords,labels, 1, color_ids, plotF+5,1);
        end
    end
    drawnow
end