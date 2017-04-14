function [mapped_data, clust_ids, som_coords, sM] = dimred2map(data, labels, datatype, maptype, plotVar, plotF, clustF)

if (nargin <6) || isempty(plotF)
    plotF = 0;            % for TCDS experiment
end

if (nargin <7) || isempty(clustF)
    clustF = 0;            % for TCDS experiment
end
%% Find best number of clusters

if strcmp(maptype, 'tsne')
    %% Cluster and plot using t-sne
    K = round(intrinsic_dim(data));%, 'EigValue')  
    dim = 2;
    mapped_data = compute_mapping(data, 'tSNE', dim);
    clust_ids  = clustPlot(mapped_data, labels, dim, K, 0, plotF );
    color_ids = clusterColor(clust_ids);
    ht = figure(plotF); ht.Position = [0 0 1000 700];

elseif strcmp(maptype, 'som')    
    %% Map the features onto a SOM and get its coordinates
    [sM,sD] = makeSOM(data, labels', datatype, plotF+3);
    [~, bmu_coords, som_coords] = get_bmu_coords(sM, sD);
    mapped_data = bmu_coords;

    %% Perform and plot clustering based on the SOM
    if (clustF)
        clust_ids = clusterSOMhierarch(sM, sD, K, 'ward', 0);
        color_ids = clusterColor(clust_ids);
    else
        clust_ids = clustByLabel(labels, [1 3]); % tool names are between ids 1 and 3        
        %color_ids = zeros(length(bmu_coords),3); color_ids(:,3) = 1; %%single color, blue        
        color_ids = clusterColor(clust_ids);
    end
    if any(plotF)
        ht = figure(plotF); ht.Position = [0 0 1000 700];
        scatter(som_coords(:,1),som_coords(:,2),10, '.k'); axis off; hold on;
        scatter(mapped_data(:,1),mapped_data(:,2),30,color_ids, 'filled' ); hold off;drawnow;   
    end
    
    if any(plotF)
        K = max(clust_ids);
        for i = 1:K
            h(:,i) = som_hits(sM,sD.data(clust_ids==i,:));    
        end

        sMapClass = som_autolabel(sM,sD,'add',1);
        figure(plotF+3);    
        %cc = hsv(K);
        cc = jet(K);
        %som_labels = clearSOMlabels(sMapClass.labels);
        som_show(sMapClass,'umat','all','bar','none', 'colormap', 1-gray); % visualize
        hitH = som_show_add('hit',h,'MarkerColor',cc,'Subplot','all');        
        %labH = som_show_add('label',som_labels,'Subplot','all');
        
    end

end

if any(plotF)
    if (plotF > 100)
        if strcmp(datatype, 'tool')            
            ht.Name = 'SOM tools';
            addMiniPlot(data, mapped_data,labels, 2, plotVar, plotF, 15);    
        elseif strcmp(datatype, 'aff')
            ht.Name = 'SOM aff';
            addMiniPlot(data, mapped_data,labels, 1, color_ids, plotF,1);
        end
    end
end