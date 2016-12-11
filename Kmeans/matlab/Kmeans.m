function [ label,center ] = Kmeans( data, K )
%	Input: data,K
%   Output:Label,center

%��ȡ����������ά��
[n,d]=size(data);
label = zeros(n,1);
%eta = 0.00001;

%��ʼ����������
center=zeros(K,d);
for k=1:K
    center(k,:)=data(k,:);
end

osse = 1.7977e+308;
%old_label=zeros(n,1);
cnt = 1;
%��ʼ����
while 1
    P = zeros(n,K);
    for k=1:K
        P(:,k)=sum((data-repmat(center(k,:),n,1)).^2, 2);
    end
    [~,label] = min(P,[],2);

%     if label == old_label
%         break;
%     end
%     old_label = label;
    %�ж�SSE��
    sse = 0;
    for k=1:K
        rows = label == k;
        sse = sse + sum(sqrt(sum((data(rows,:)-repmat(center(k,:),size(data(rows,:), 1),1)).^2,2)));
    end;
    
    %���¾�������
    for k=1:K
        rows = label==k;
        center(k,:) = mean(data(rows,:));
    end
    fprintf('��\n%d�ε���,DSSE=%d,SSE=%d,�����������£�\n',cnt, osse-sse, sse);
    for k=1:K
        num = length(find(label==k));
        fprintf('%d,',num);
    end;
    
    cnt = cnt+1;
    if abs(osse-sse) < 10
        break;
    end;
    osse = sse;
end
end

