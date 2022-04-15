clc;clear;close all;

I = imread("checkerboardOne.jpg");
% imshow(I)
[imagePoints1,boardSize] = detectCheckerboardPoints(I);
I = insertMarker(I, imagePoints1, 'o', 'Color', 'red', 'Size', 1);

imagePoints1Int = round(imagePoints1);
err1Mat = abs(imagePoints1-imagePoints1Int);
err1  =sum(sum(err1Mat))/sum(sum(imagePoints1))
imagePoints1 = round(imagePoints1);
imagePoints1Out = 1000*1000*imagePoints1;
imagePoints1Out = round(imagePoints1Out);
% imshow(I)

fid=fopen(['.\','1.txt'],'w');		% 写入文件路径
[m,n] = size(imagePoints1Out);
for i=1:m
    fprintf(fid,'%d,%d\n',imagePoints1Out(i,1),imagePoints1Out(i,2));		% 按行输出
end
fclose(fid);		% a为要保存的变量名，记得更改!!!


I = imread("checkerboardTwo.jpg");
% imshow(I)
[imagePoints2,boardSize] = detectCheckerboardPoints(I);
I = insertMarker(I, imagePoints2, 'o', 'Color', 'red', 'Size', 1);

imagePoints2Int = round(imagePoints2);
err2Mat = abs(imagePoints2-imagePoints2Int);
err2  =sum(sum(err2Mat))/sum(sum(imagePoints2))
imagePoints2 = round(imagePoints2);
imagePoints2Out = 1000*1000*imagePoints2;
imagePoints2Out = round(imagePoints2Out);
% imshow(I)

fid=fopen(['.\','2.txt'],'w');		% 写入文件路径
[m,n] = size(imagePoints2Out);
for i=1:m
    fprintf(fid,'%d,%d\n',imagePoints2Out(i,1),imagePoints2Out(i,2));		% 按行输出
end
fclose(fid);		% a为要保存的变量名，记得更改!!!

A = [];
for i=1:88
A = [A;
    [0,0,0],                                    -1*[imagePoints1(i,1),imagePoints1(i,2),1],   imagePoints2(i,2)*[imagePoints1(i,1),imagePoints1(i,2),1];
     1*[imagePoints1(i,1),imagePoints1(i,2),1],  [0,0,0],                                     -imagePoints2(i,1)*[imagePoints1(i,1),imagePoints1(i,2),1];  
    ];
end
[u,s,v] = svd(A);

H = [v(1,9),v(2,9),v(3,9);
    v(4,9),v(5,9),v(6,9);
    v(7,9),v(8,9),v(9,9);
    ];
H = 1/H(3,3)*H
fid=fopen(['.\','h.txt'],'w');		% 写入文件路径
[m,n] = size(imagePoints2);
Hout = 1000*1000*H;
Hout = round(Hout);
for i=1:3
    for j=1:3
        fprintf(fid,'%d \n',Hout(i,j));		% 按行输出
    end
end
fclose(fid);		% a为要保存的变量名，记得更改!!!

Hinv = inv(H);
Hinv = 1/Hinv(3,3)*Hinv
Hinvout = 1000*1000*Hinv;
Hinvout = round(Hinvout);
fid=fopen(['.\','hInv.txt'],'w');		% 写入文件路径
for i=1:3
    for j=1:3
        fprintf(fid,'%d \n',Hout(i,j));		% 按行输出
    end
end
fclose(fid);		% a为要保存的变量名，记得更改!!!