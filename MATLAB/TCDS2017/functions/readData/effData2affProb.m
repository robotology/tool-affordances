% transform effect values into affordance probabilities (heuristically)
% < 5cm means fail:     0
% > 15cm means success: 1
% for  5 < eff < 15, linear 
numRep = 4;
numAct = 4;

aff_prob = aff_data;
aff_prob(aff_prob > 0.15)  = 0.15; % cap at 0.15
aff_prob = aff_prob - 0.05; % min 0.1
aff_prob(aff_prob<0)  = 0; % cap at 0.15

% now all the matrix is between 0.1 and 0.0
aff_prob = aff_prob *10;        % normalize prob to 0 - 1 

% Reshape to have every toolpose in one row
%aff_prob = reshape(aff_prob',size(aff_prob,2)*numRep,size(aff_prob,1)/numRep)';

ff = reshape(aff_prob', numRep,numAct,length(aff_prob)/numRep);

for tp = 1:size(ff,3)
    fprintf('%s \n', aff_labs{tp*numRep});    
    p(tp,:) = mean(ff(:,:,tp),2);
    for a = 1:numAct
        fprintf('%f ', ff(a,:,tp)');
        fprintf('\n ');
    end
    %fprintf('%f \n', p(tp,:)');
end


