function ori = deg2ori(deg)
    
    ori = cell(1,length(deg));
    for i = 1:length(deg)
        if ((deg(i) > -135 ) && (deg(i) < -45))  
            ori{i} = 'rght';
        end
        if ((deg(i) >= -45 ) && (deg(i) <= 45))  
            ori{i} = 'frnt';
        end
        if ((deg(i) > 45 ) && (deg(i) <= 135))  
            ori{i} = 'left';
        end
    end
end