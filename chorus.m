clear all
close all
clc
[x,Fs] = audioread('acoustic.wav'); % señal de impulso unitario de longitud 100
g=0.5;
Delayline=zeros(10,1); % asignación de memoria para la longitud 10
for n=1:length(x);
	y(n)=x(n)+g*Delayline(10);
	Delayline=[y(n);Delayline(1:10-1)];
end;

figure(1)
hold on
plot(x(1:100),'r');
plot(y(1:100),'b');
title('Chorus Primeras 100 Muestras');
audiowrite('out_chorus.wav',y,Fs);