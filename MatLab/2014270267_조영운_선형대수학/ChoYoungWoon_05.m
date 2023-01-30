for i = 2:1:15
    x = 1;
    for j = 2:1:i-1
        if mod(i,j) == 0
            x =  0;
            break;
        end
    end
    if x == 1
        fprintf('%d\t',i);
    end
end