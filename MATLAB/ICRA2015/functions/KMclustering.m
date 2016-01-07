function [kmLabels, centroids,dbInd] = KMclustering(data,trueLabels, kN, fL)

if (nargin < 3 )|| isempty(kN)
    kN = [3, 4, 5, 6, 9];
end
if (nargin < 4 )|| isempty(fL)
    fL =100;
elseif (fL == 0)
    plotF = 0;
else 
    plotF=1;
end

if (plotF)
    colorList = hsv(max(unique(trueLabels)));
    colorTrueClass = colorList(trueLabels,:);
    figure(fL);
    title('Cluster Prototypes');
    % figure(fL+1);
    % title('2D projection');
    % figure(fL+2);
    % title('3D projection');
end

kL =length(kN);
dbInd = zeros(kL,1);
for i=1:kL
    N = kN(i);
    
    [kmLabels{i}, centroids{i}, err] = kmeans(data,N, 'replicates', 10);
    errors(i)= sum(err);
    % Compute Davies-Bouldin clustering index.
    dbInd(i) = db_index(data,kmLabels{i}, centroids{i}, 2);
    
    % plot the prototype vectors
    if (plotF)
        figure(fL);
        for k=1:N
            subplot(kL,max(kN),(i-1)*max(kN) + k);
            plot(-0.05:0.01:0.05,centroids{i}(k,:))
            %plot(centroids{i}(k,:))
            %axis([-0.05 0.05 0 0.3])
            if (k==1)            
                ylab = sprintf('Prot. N=%d ', kN(i)); ylabel(ylab);  
            end
        end
    end
        
%     % Reduce the dimension from ND to 2D
%     distanceMatrix = pdist(data,'euclidean');    
%     newCoor = cmdscale(distanceMatrix); % 
%     newCoor3 = mdscale(distanceMatrix,3,'Criterion','strain'); % take longer time, but more beautiful
%     % Plot the whole data set
%     colorClusterClass = colorList(kmLabels{i},:);
%     x = newCoor(:,1);
%     y = newCoor(:,2);
%     
%     x3 = newCoor3(:,1);
%     y3 = newCoor3(:,2);
%     z3 = newCoor3(:,3);
%     patchSize = 30; %max(decis_values,[],2);
% 
%     figure(fL+1)
%     subplot(ceil(sqrt(kL)),ceil(sqrt(kL)),i);
%     scatter(x,y,patchSize,colorClusterClass,'filled');
%     tit = sprintf('kmeans clustering %d clusters', kN(i));  title(tit);
%     
%     figure(fL+2)
%     subplot(ceil(sqrt(kL)),ceil(sqrt(kL)),i);   
%     scatter3(x3,y3,z3,patchSize,colorClusterClass,'filled');
%     tit = sprintf('kmeans clustering %d clusters', kN(i));  title(tit);  
%     
%     figure(fL+3)
%     subplot(ceil(sqrt(kL)),ceil(sqrt(kL)),i);   
%     silhouette(data,kmLabels{i});
%     tit = sprintf('Silhouette for %d clusters', kN(i));  title(tit);
end

%  figure (fL+1)
%  subplot(ceil(sqrt(kL)),ceil(sqrt(kL)),i+1); 
%  scatter(x,y,patchSize,colorTrueClass,'filled');
%  title('True classes');
%  figure (fL+2)
%  subplot(ceil(sqrt(kL)),ceil(sqrt(kL)),i+1); 
%  scatter3(x3,y3,z3,patchSize,colorTrueClass,'filled');
%  title('True classes');
 if (plotF)
     if (kL>1)
         figure(fL)
         subplot(2,3,3)
         plot(kN,dbInd)
         title('Davies-Bouldin index for clusterizatoin with N clusters');
         xlabel('N');
         ylabel('D-B index');
     end
 end

 
