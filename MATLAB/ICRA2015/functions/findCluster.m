function kIdx = findCluster(data, centroids)

nD = size(data,1);
nC = size(centroids,1);

kIdx= zeros(nD,1);

for d=1:nD
    dist= zeros(nC,1);
    for c=1:nC
        dist(c) = norm(data(d,:)-centroids(c,:));
    end
    [dummy,kIdx(d)] =min(dist);
end