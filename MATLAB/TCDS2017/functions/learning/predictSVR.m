function [pred_target, errors] = predictSVR(SVR, input, plotF)
% Predict some values

pred_target = Predict(SVR, input);

errors = Margin(SVR, input, pred_target);
if plotF >0
    % Build plot
    BuildPlot(SVR);
end
