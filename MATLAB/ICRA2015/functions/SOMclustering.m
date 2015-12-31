function [sMap,sData, centroids, indices] = SOMclustering(data, labels, toolNames)
%SOM_DEMO2 Basic usage of the SOM Toolbox.

if ((nargin == 1)||isempty(labels))
    labels = ones(size(data,1),1);
end

if (nargin <= 2)
    for l = 1:size(data,1)
        toolNames{l} = num2str(l);    
    end
end

% Get data
D = data;%(:,featMask); % 1000 samples from unit cube

sData = som_data_struct(D,'name','tools');

for i = unique(labels)'
    sData = som_label(sData,'add',find(labels==i),num2str(i));    
end

for l = 1:size(data,1)
    sData = som_label(sData,'add',l,toolNames{l});    
end

    
%    STEP 2: DATA NORMALIZATION

%sData = som_normalize(sData,'var');

%    STEP 3: MAP TRAINING
sMap = som_make(sData, 'msize', [20 15], 'shape', 'sheet');
sMapClass = som_autolabel(sMap,sData,'add',1);
sMapNames = som_autolabel(sMap,sData,'add',2);

for i = unique(labels)'
    h(:,i) = som_hits(sMap,sData.data(labels==i,:));
end

%    STEP 4: VISUALIZING THE SELF-ORGANIZING MAP: SOM_SHOW + SOM_SHOW_ADD
%    =========================================================
figure;
[centroids, indices, err, db_ind] = kmeans_clusters(sMapClass, 10, 20, 1); % find clusterings
[dummy,clustNumOpt] = min(db_ind); % select the one with smallest index
figure;
cc = prism(length(unique(labels)));
somH = som_show(sMapClass,'umat','all', 'bar','none', 'colormap', 1-gray, 'color',{indices{clustNumOpt},sprintf('K-Means %d clusters',clustNumOpt)}); % visualize
hitH = som_show_add('hit',h,'MarkerColor',cc,'Subplot','all');
labelsH = som_show_add('label',sMapNames,'Textsize',8,'TextColor','k','Subplot',1);
uistack(labelsH,'top');
% plot the prototype vectors
%for nClust = [clustNumOpt]
%    figure
%    for k=1:nClust
%        subplot(ceil(sqrt(nClust)),ceil(sqrt(nClust)),k);
%        prototypes = som_denormalize(centroids{nClust},sMap.comp_norm);
%        plot(prototypes(k,:))
%    end
%end




%    STEP 5: ANALYSIS OF RESULTS
%    ===========================

%    The purpose of this step highly depends on the purpose of the
%    whole data analysis: is it segmentation, modeling, novelty
%    detection, classification, or something else? For this reason, 
%    there is not a single general-purpose analysis function, but 
%    a number of individual functions which may, or may not, prove 
%    useful in any specific case.

%    Visualization is of course part of the analysis of
%    results. Examination of labels and hit histograms is another
%    part. Yet another is validation of the quality of the SOM (see
%    the use of SOM_QUALITY in SOM_DEMO1).

%[qe,te] = som_quality(sMap,sData)

%    People have contributed a number of functions to the Toolbox
%    which can be used for the analysis. These include functions for 
%    vector projection, clustering, pdf-estimation, modeling,
%    classification, etc. However, ultimately the use of these
%    tools is up to you.

%    More about visualization is presented in SOM_DEMO3.
%    More about data analysis is presented in SOM_DEMO4.

echo off
%warning on

