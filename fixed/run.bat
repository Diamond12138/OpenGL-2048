@echo off

echo ----------Input File----------

set input=main
echo %input%.exe

echo ----------Compling----------
g++ -finput-charset=UTF-8 -fexec-charset=gbk %input%.cpp -o %input%.exe -lgdi32 -lopengl32 -lfreeglut -lglu32
echo ----------Running----------
%input%.exe

echo ----------End----------
pause