function [sM,sD, bmu_coords]  = initSOM(data,labels, name, plotF)
sM = som_make(data, 'msize', [15 25], 'shape', 'sheet', 'name',name, 'tracking', 0);
sD = som_data_struct(data,'name',name, 'labels', labels');
[~, bmu_coords, som_coords] = get_bmu_coords(sM, sD);

if plotF > 0
        figure(plotF);               
        sMapClass = som_autolabel(sM,sD,'add',1);
        som_show(sMapClass,'umat','all','bar','none', 'colormap', 1-gray); % visualize        
        h(:,i) = som_hits(sM,sD.data(clust_ids==i,:));
end