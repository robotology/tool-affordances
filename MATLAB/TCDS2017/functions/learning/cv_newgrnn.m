function cv_mse = cv_newgrnn(x, y, spread, nr_fold)

len=size(y,2);
cv_mse = 0;
rand_ind = randperm(len);
for i=1:nr_fold % Cross training : folding
  test_ind=rand_ind([floor((i-1)*len/nr_fold)+1:floor(i*len/nr_fold)]');
  train_ind = [1:len]';
  train_ind(test_ind) = [];
  net = newgrnn(x(:,train_ind),y(:,train_ind),spread);
  pred = net(x(:,test_ind));
  se = mse(net,y(:,test_ind),pred);
  cv_mse = cv_mse + se;
end
cv_mse = cv_mse / len;
fprintf('.');
%fprintf('Cross-validation MSE = %g\n', cv_mse);