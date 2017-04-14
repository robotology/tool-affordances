function [sMap_online,sData] = makeSOM_incremental(data,labels, name,num_stops, plotF)

if (nargin < 4) || isempty(plotF)
    plotF = 0;
end

%% make SOM
%num_stops = 30;
batch_size = floor(size(data,1)/num_stops);
sMap_online = som_make(data(1:batch_size,:), 'msize', [15 25], 'shape', 'sheet', 'name',name, 'tracking', 0);
figure(plotF-1);
som_show(sMap_online,'umat','all','bar','none', 'colormap', 1-gray); % visualize
for stop_i = 1:num_stops-1
    data_batch = data(batch_size*stop_i:batch_size*(stop_i+1),:);
    %labs_batch = labels(batch_size*stop_i:batch_size*(stop_i+1));
    figure(plotF+1);
    sMap_online = som_seqtrain(sMap_online,data_batch,'tracking',3);
    
    if any(plotF)
        figure(plotF-1);
        som_show(sMap_online,'umat','all','bar','none', 'colormap', 1-gray); % visualize
    end
end



sData = som_data_struct(data,'name',name, 'labels', labels);
sMap = som_make(sData, 'msize', [15 25], 'shape', 'sheet', 'name',name, 'tracking', 0);

%% plot U-Matrix
if any(plotF)
    figure(plotF + 1);
    sMapClass = som_autolabel(sMap,sData,'add',1);
    som_show(sMapClass,'umat','all','bar','none', 'colormap', 1-gray); % visualize
end