% sampleScript.m
% This file is meant to be invoked by igafemgui to test
% The export of variables

printf('\nThis a sample script to demonstrate\nwhat igafemgui can do.\n')
printf('Here we just output some variables\nand count to five\n\n');

printf('Within the script\n * foo=%f\n * param1=%f\n', foo, param1)

disp('Here is a loop:')
for i=0:5
	printf('%d', i)
end

disp('')

printf('plotting a curve...\n')

x=[-10:0.1:10];
y=sin(x);
plot(x, y)
title('Test curve')
legend('Test graph', 'location', 'SouthEast')
