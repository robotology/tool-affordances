function [bmus, bmu_coords, som_coords] = get_bmu_coords(sM, sD)

%% Find each sample's best matching unit coordinates (BMU, index of cell in SOM)
bmus = som_bmus(sM,sD);                     % get samples bmu
som_coords = som_unit_coords(sM);           % get neuron coordinates

bmu_coords = zeros(length(bmus),2);
for n = 1 : length(bmus)
    bmu_coords(n,:) =  som_coords(bmus(n),:);
end
bmu_coords(:,2) = - bmu_coords(:,2) + sM.topol.msize(1); % We need to invert Y to match the SOM plot
som_coords(:,2) = - som_coords(:,2) + sM.topol.msize(1);