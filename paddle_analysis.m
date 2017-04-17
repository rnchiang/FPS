function paddle_analysis(inputFile)

%% Input Data

data = importdata(inputFile);

%% Find a Baseline for Zero Work

% Assuming no paddling at beginning,
% Find average between 5th input and 15th input
% (First input is usually incorrect, voltage spike)

baseline = mean(data(5:15));
adjusted_data = data(2:1:length(data)) - baseline;
for i = 1:length(adjusted_data)
    if (adjusted_data(i) < 0)
        adjusted_data(i) = 0;
    end
end
    
    
    

%% Graph Data

paddle_graph(adjusted_data, 'Adjusted Data');

%% Parse data for individual strokes

% Find peaks

[peaks, location] = findpeaks(adjusted_data, 'MinPeakHeight', 5, ...
    'MinPeakDistance', 25);

% Find %20 of the peak on both sides. These are boundaries of stroke
bounds = zeros(2*length(peaks),2);
for i = 1:length(peaks)
    min_limit = peaks(i)*0.20;
    limit_left = false;
    limit_right = false;
    j = location(i);
    while (~limit_left)
        if (adjusted_data(j) <= min_limit)
            limit_left = true;
            bounds(2*i-1,:) = [j,adjusted_data(j)];
            
        else
            j = j - 1;
        end  
    end
    j = location(i);
    while (~limit_right)
        if (adjusted_data(j) <= min_limit)
            limit_right = true;
            bounds(2*i,:) = [j,adjusted_data(j)];
        else
            j = j + 1;
        end  
    end
    
    
end



% Graph peaks

hold on;
plot(location, peaks, 'rx');
plot(bounds(:,1), bounds(:,2), 'o');

%% Calculate Force per Stroke and Impulse (momentum)

stroke_impulse = zeros(length(adjusted_data),1);
for i = 1:length(bounds)-1
    
    % Sum voltages(force)
    vsum(i) = sum(adjusted_data(bounds(i,1):bounds(i+1,1)));
    stroke_time(i) = bounds(i+1,1) - bounds(i,1);
    % Calculate Impulse
    stroke_impulse(bounds(i,1):bounds(i+1,1)) = vsum(i) / stroke_time(i);
    
end

%Plot
plot(1:length(stroke_impulse),stroke_impulse,'m');


hold off;
end