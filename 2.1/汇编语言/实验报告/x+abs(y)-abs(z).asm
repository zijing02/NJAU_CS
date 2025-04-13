data segment 
    input db 'x=20,y=24,z=-42',0dh,0ah,'x+|y|-|z|=$'
    x db 20h
    y db 24h
    z db -42h
data ends

; stack segment
;     db 200 dup(0)
; stack ends

code segment
    assume ds:data,cs:code;,ss:stack     ;假定每个段的段寄存器，但是需要自己赋值
begin:
    mov ax,data
	mov ds,ax
    ; mov ax,stack
    ; mov ss,ax
    lea dx,input
    mov ah,09h                  ;显示要计算的内容
    int 21H                     
    mov al,x                    ;将x赋给ax
    cmp y,0             
    jge Ly                  ;将y与0比较，大于等于0就跳转到ly，直接相加
    mov bl,0                   
    sub bl,y                    ;bl为0，减去y就是|y|
    mov y,bl                    ;将bl的值返回给y，继续执行ly
ly:
    add al,y
    cmp z,0
    jge lz                      ;将z与0比较，大于等于0就跳转到lz，直接相加
    mov bl,0
    sub bl,z
    mov z,bl
lz:
    sub al,z
    mov dl,al                   ;将al中的值转移到dl中准备输出
    add dl,30H                  ;dl中的数字加上48，转化为ascii输出
    mov ah,2                    ;显示命令
    int 21H                     ;终止命令    
    mov ah,4CH                  ;停止命令
    int 21H
code ends
    end begin
 