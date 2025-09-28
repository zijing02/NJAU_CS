@echo off
setlocal enabledelayedexpansion

echo 正在处理 *--题目--算法.txt 格式的文件...
echo.

for %%f in (*--*--*.txt) do (
    set "fullname=%%f"
    
    :: 使用分隔符分割文件名
    for /f "tokens=1,* delims=--" %%a in ("%%f") do (
        if not "%%b"=="" (
            echo 重命名: %%f -^> %%b
            ren "%%f" "%%b"
        )
    )
)

echo.
echo 批量重命名完成！
pause