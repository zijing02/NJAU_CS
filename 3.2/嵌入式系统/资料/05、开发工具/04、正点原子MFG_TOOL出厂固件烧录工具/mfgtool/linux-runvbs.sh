#!/bin/bash
file=$1
echo $file
filedata=$(cat $file | grep -i mfgtool2.exe)
filedata=$(echo $filedata | sed "s/Set wshShell = CreateObject\(\"WScript\.shell\"\)\n//g")
filedata=$(echo $filedata | sed "s/wshShell //g")
filedata=$(echo $filedata | sed "s/Set\ =\ //g")
filedata=$(echo $filedata | sed "s/CreateObject\(.*\)//g")
filedata=$(echo $filedata | sed "s/wshShell\.run\ \"//g")
filedata=$(echo $filedata | sed "s/-c\ \"\"linux\"\"//g")
filedata=$(echo $filedata | sed "s/\"\"//g")
filedata=$(echo $filedata | sed "s/\"//g")
filedata=$(echo $filedata | sed "s/Nothing//g")
filedata=$(echo $filedata | sed "s/mfgtool2.exe/\.\/mfgtoolcli/g")
filedata=$(echo $filedata | sed "s/Quad\ Nor\ Flash/\"Quad\ Nor\ Flash\"/g")
filedata=$(echo $filedata | sed "s/l\ Nor\ Flash/l\ \"Nor\ Flash\"/g")
filedata=$(echo $filedata | sed "s/l\ NAND\ Flash/l\ \"NAND\ Flash\"/g")
filedata=$(echo $filedata | sed "s/^\'/#/g")
echo "Executing: $filedata"
$(echo $filedata)
