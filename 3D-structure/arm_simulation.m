% 2D PEN PLOTTER MANIPULATOR SIMULATION

l1 = 10;                % length of the first link
l2 = 11.5;                % length of the second link

theta1 = -pi/2: 0.1: pi/2;    % array declaration with starting point in 0 and final point in pi/2 with a delta of 0.1
theta2 = -pi: 0.1: 0;

[THETA1, THETA2] = meshgrid(theta1, theta2);

X = l1 * cos(THETA1) + l2 * cos(THETA1 + THETA2);
Y = l1 * sin(THETA1) + l2 * sin(THETA1 + THETA2);

data1 = [X(:) Y(:) THETA1(:)];  % create x-y-theta1 dataset
data2 = [X(:) Y(:) THETA2(:)];  % create x-y-theta2 dataset

%PLOT
plot(X(:), Y(:), 'r.');
axis equal;
xlabel('X', 'FontSize', 10);
ylabel('Y', 'FontSize', 10);
title('Plotter workspace');