function imagineAffordance(model,graspDataPose,actDataPose,effDataPose,clusterIdx, muNormIn, muNormOut, sigNormIn, sigNormOut, canonF, plotF)

nClusters = max(clusterIdx);
figure(plotF)
for c = 1:nClusters
    posesClust = find(clusterIdx==c);
    numPosesClust = length(posesClust);
    if canonF == 1
        subplot(ceil(sqrt(nClusters)),ceil(sqrt(nClusters)),c);
        
        inX = (0:15:315)';
        inY = (-90:30:90)'; 
        in = [repmat(inX,length(inY),1) reshape(repmat(inY,1,length(inX))', [length(inY)*length(inX),1])];
        lin = length(in);
        inNorm = (in - repmat(muNormIn{c},lin,1))./repmat(sigNormIn{c},lin,1);
        outNorm = model{c}(inNorm');
        out = outNorm'.*repmat(sigNormOut{c},lin,1) + repmat(muNormOut{c},lin,1);
        %outSurf = reshape(out,length(inY),length(inX));
        %mesh(inX,inY,outSurf); hold on
        scatter3(in(:,1), in(:,2),out,'filler');hold on; 

        gr0id = find(graspDataPose(:,3,1) == 0);
        affVec = [actDataPose(gr0id,1,posesClust) graspDataPose(gr0id,2,posesClust) effDataPose(gr0id,1,posesClust)];
        scatter3(mean(affVec(:,1,:),3), mean(affVec(:,2,:),3),mean(affVec(:,3,:),3),'linewidth', 2);hold off;
        %affMat = reshape(affVec,8,9,size(affVec,2),numPosesClust);        
        %plot3(mean(affMat(:,:,1,:),4), mean(affMat(:,:,2,:),4),mean(affMat(:,:,3,:),4),'linewidth', 2, 'color',[0 0 1]);hold off;   
     if canonF == 2
        subplot(ceil(sqrt(nClusters)),ceil(sqrt(nClusters)),c);
        
        inX = (0:45:315)';
        inY = c; 
        in = [repmat(inX,length(inY),1) reshape(repmat(inY,1,length(inX))', [length(inY)*length(inX),1])];
        lin = length(in);
        inNorm = (in - repmat(muNormIn{c},lin,1))./repmat(sigNormIn{c},lin,1);
        outNorm = model{c}(inNorm');
        out = outNorm'.*repmat(sigNormOut{c},lin,1) + repmat(muNormOut{c},lin,1);
    else       
        in = 0:20:360;
        lin = length(in);
        inNorm = (in - muNormIn{c})./sigNormIn{c};
        outNorm = model{c}(inNorm);
        out = outNorm.*sigNormOut{c} + muNormOut{c};
        subplot(ceil(sqrt(nClusters)),ceil(sqrt(nClusters)),c);
        plot(in,out,'linewidth',2,'color',[1 0 0]);hold on;    
        affVec = permute([actDataPose(:,1,posesClust) effDataPose(:,1,posesClust)],[1 3 2]);
        plot(mean(affVec(:,:,1),2), mean(affVec(:,:,2),2),'linewidth', 2, 'color',[0 0 1]); hold off;    
        axis([-10 360 0 0.25])
        xlabel('Action - Pull Angle');
        ylabel('Effect (cm)');  
    end
end
