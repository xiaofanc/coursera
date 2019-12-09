


theta1 = [1 1 2.4;1 1.7 3.2];
theta2 = [1 0.3 -1.2];
x = [1;2;2];

a2 = sigmoid(theta1*x);
a2 = [1; a2];
sigmoid(theta2 * a2)


theta1ud = [1 1.7 3.2; 1 1 2.4];    % flip up down
theta2lr = [1 -1.2 0.3 ];           % flip left right
a2p = sigmoid(theta1ud*x);
a2p = [1; a2p];
sigmoid(theta2lr * a2p)