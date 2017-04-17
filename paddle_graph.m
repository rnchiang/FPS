function paddle_graph(data,info)


%% Graph Data

[n,c] = size(data);
t = 1:n;

figure;
plot(t,data);
legend('Paddler');
xlabel('Time'), ylabel('Relative Voltage');
ylim([-5 inf])
title(['Paddle Session: ',info])



end