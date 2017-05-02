function colorIds = clusterColor(clustIds)

K = max(clustIds);
%cc = hsv(K);
cc = jet(K);
for i = 1:K
    colorIds(clustIds == i,:) = repmat(cc(i,:), length(clustIds(clustIds == i)),1);
end
