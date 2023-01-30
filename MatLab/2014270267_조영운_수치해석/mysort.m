% my_sort
function [x,avg] = mysort(A)
x = sort(A);
avg = mean(A,'all');
end