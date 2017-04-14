% plot old data in format as new: 
[feats3D, actDataPose, effDataPose,  graspDataPose, toolNames, numPoses] = readAffData3D(10,0);

[affsMeans, affsStds] = old2newEffData(effDataPose);

for i = 1:length(toolNames)
    if (mod(i,3)==0)
        lab = char(toolNames{i});
        tpLabels{i/3} = lab(1:8);
    end
end


% Plot the data by tool type:
plotTPaffs(affsMeans(1:27,:),affsStds(1:27,:), tpLabels(1:27))
plotTPaffs(affsMeans(28:54,:),affsStds(28:54,:), tpLabels(28:54))
plotTPaffs(affsMeans(55:81,:),affsStds(55:81,:), tpLabels(55:81))
plotTPaffs(affsMeans(82:111,:),affsStds(82:111,:), tpLabels(82:111))
plotTPaffs(affsMeans(112:132,:),affsStds(112:132,:), tpLabels(112:132))