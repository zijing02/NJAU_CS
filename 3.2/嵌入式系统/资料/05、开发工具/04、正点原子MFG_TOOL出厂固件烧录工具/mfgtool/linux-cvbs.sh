#!/bin/bash
cd ..
find . -type f -iname "*.vbs" -exec sed -i "s/Set wshShell = CreateObject\(\"WScript\.shell\"\)\n//g" {} \;
find . -type f -iname "*.vbs" -exec sed -i "s/wshShell //g" {} \;
find . -type f -iname "*.vbs" -exec sed -i "s/Set\ =\ //g" {} \;
find . -type f -iname "*.vbs" -exec sed -i "s/CreateObject\(.*\)//g" {} \;
find . -type f -iname "*.vbs" -exec sed -i "s/wshShell\.run\ \"//g" {} \;
find . -type f -iname "*.vbs" -exec sed -i "s/-c\ \"\"linux\"\"//g" {} \;
find . -type f -iname "*.vbs" -exec sed -i "s/\"\"//g" {} \;
find . -type f -iname "*.vbs" -exec sed -i "s/\"//g" {} \;
find . -type f -iname "*.vbs" -exec sed -i "s/Nothing//g" {} \;
find . -type f -iname "*.vbs" -exec sed -i "s/mfgtool2.exe/\.\/mfgtoolcli/g" {} \;
find . -type f -iname "*.vbs" -exec sed -i "s/Quad\ Nor\ Flash/\"Quad\ Nor\ Flash\"/g" {} \;
find . -type f -iname "*.vbs" -exec sed -i "s/l\ Nor\ Flash/l\ \"Nor\ Flash\"/g" {} \;
find . -type f -iname "*.vbs" -exec sed -i "s/l\ NAND\ Flash/l\ \"NAND\ Flash\"/g" {} \;
find . -type f -iname "*.vbs" -exec sed -i "s/^\'/#/g" {} \;
find . -type f -iname "*.vbs" -exec mv {} {}.sh \;

chmod +x *.sh
