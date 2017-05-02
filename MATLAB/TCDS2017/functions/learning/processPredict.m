function processPredict(simF, tool_res)

if (nargin < 2)
    tool_resF = 0;
else
    tool_resF = 1;
end

%Load recorded data
[aff_data_rec, tp_labels_rec]  = readAffData(simF, 0); % XXX star i smisisng
tp_labels_rec = tp_labels_rec';

% Load data from action selection experiment
[tool_labels_sel,deg, eff_sel]= readActSelectAffData(simF);

if (tool_resF)
    tools_rec = cell_subindex(tp_labels_rec, (1:3));
    tools_sel = cell_subindex(tool_labels_sel, (1:3));    

    tool_ind_sel = find(~cellfun('isempty',strfind(tools_sel, tool_res)));        
    tool_ind_rec = find(~cellfun('isempty',strfind(tools_rec, tool_res)));       
    
    aff_data_rec = aff_data_rec(tool_ind_rec, :);
    tp_labels_rec = tp_labels_rec(tool_ind_rec);
    
    eff_sel = eff_sel(tool_ind_sel);
    tool_labels_sel = tool_labels_sel(tool_ind_sel);
    deg = deg(tool_ind_sel);    
end



N_sel = size(tool_labels_sel,1);
ori = deg2ori(deg);
tp_labels_sel = cell(N_sel,1);
for tp = 1:N_sel
    tp_labels_sel{tp} = [tool_labels_sel{tp} '_' ori{tp}];
end

tp_labels = unique(tp_labels_rec, 'stable');
N = length(tp_labels);
for tp = 1:N
    tool_labels{tp} = [tp_labels{tp}(1:4)];
end
    
% Compare the effects of the selected actions, to the max effect obtained
% by each of the tested tool-poses.
median_eff_sel = zeros(1,N);

max_eff = zeros(1,N);
mean_eff = zeros(1,N);
median_eff = zeros(1,N);
mean_max_eff = zeros(1,N);
mean_mean_eff = zeros(1,N);
mean_median_eff = zeros(1,N);



for tp_i=1:N
    fprintf('Tool-pose: %s \n', tp_labels{tp_i});
    tp_ind_sel = find(~cellfun('isempty',strfind(tp_labels_sel, tp_labels{tp_i})));
    tp_ind_rec = find(~cellfun('isempty',strfind(tp_labels_rec, tp_labels{tp_i})));

    eff_sel_toolpose = eff_sel(tp_ind_sel);
    median_eff_sel(tp_i) = median(eff_sel_toolpose);
    
    aff_data_rec_toolpose = aff_data_rec(tp_ind_rec,:); 
    
    aff_mean_toolpose = mean(aff_data_rec_toolpose); 
    mean_max_eff(tp_i) = max(aff_mean_toolpose);
    mean_mean_eff(tp_i)= mean(aff_mean_toolpose);
    mean_median_eff(tp_i)= median(aff_mean_toolpose); 
end

dist_to_max = mean_max_eff-median_eff_sel;

%plot the min, max and mean of recorded data, and the eff of selected action
figure(1)
stairs(mean_max_eff,'g','LineWidth',2);hold on
stairs(mean_median_eff,'r','LineWidth',2);
stairs(median_eff_sel,'b','LineWidth',2);
plot(dist_to_max, 'ko');hold off

axis([0 size(median_eff_sel,2)+1 0 0.25])
ax = gca;
set(ax,'YTick',[0;0.1;0.2;0.3])
set(ax,'YTickLabel',['   0';' 0.1';' 0.2';' 0.3'])
set(ax,'XTick',1:3:N)
set(ax,'XTickLabel',tool_labels(1:3:end))
ax.XTickLabelRotation=45;
colormap(lines(4)); %nice combination of colors
    
%Compute number of times over the mean.
overMeanPer = sum(median_eff_sel>mean_mean_eff)/N
gambScore_mean = (sum(median_eff_sel>mean_mean_eff) - sum(median_eff_sel<mean_mean_eff))/N
overMedianPer = sum(median_eff_sel>mean_median_eff)/N
gambScore_med = (sum(median_eff_sel>mean_mean_eff) - sum(median_eff_sel<mean_median_eff))/N

avg_dist_to_max = mean(dist_to_max)