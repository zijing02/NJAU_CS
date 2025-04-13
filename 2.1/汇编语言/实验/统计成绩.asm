; 题目：对随机输入的11名学生的成绩进行排序与分数段人数统计，输出最高分，最低分，中间值以及排序后的成绩。
; 内容：定义存储空间存放成绩，输出不及格人数，60~69分数段人数，……，90~100分数段人数，对成绩进行排序，输出排序后的结果，在显示器上显示如下信息：
; 		The score between 90 and 100 ： ×× 
; 							……
; 		The score between 0 and 59 ：×× 
; 		Min is ××
; 		Max is ××
; 		The middle score is ××
; 		Rank ordering : ×× ×× ××……
; 		××表示一个十进制数
data segment
    buf db 89,76,91,93,58,64,83,81,45,72,84
    tj db 5 dup(0)   ;0为100-90,1为90-80,2为80-70,3为70-60,4为60-0
    info1 db 0dH,0ah,'THE numberof people between100-90 90-80 80-70 70-60 59-0 is:$'
	info2 db 0dH,0ah,'cong gao dao di pai xu hou ,11 ge ren de cheng ji xu lie wei:$'
data ends
code segment
assume cs:code,ds:data
begin:
;di存放每个分段的人数，cx判断次数，al存放每个数组元素，bl用于对数组写入数据
;dl,dx存放要输出的内容，ah用于输出
    mov ax,data
    mov ds,ax
    mov cx,11   ;cx为总人数
    lea si,buf
    lea di,tj
range:
    mov al,[si]
    cmp al,90
    jge higher90
    cmp al,80

    jge higher80
    cmp al,70
    jge higher70
    cmp al,60
    jge higher60
    mov bl,[di+4]
    inc si
    loop range
    jmp finishcount
higher90:
    mov bl,[di]
    inc bl
    mov [di],bl
    inc si
    loop range
    jmp finishcount
higher80:
    mov bl,[di+1]
    inc bl
    mov [di+1],bl
    inc si
    loop range
    jmp finishcount
higher70:
    mov bl,[di+2]
    inc bl
    mov [di+2],bl
    inc si
    loop range
    jmp finishcount
higher60:
    mov bl,[di+3]
    inc bl
    mov [di+3],bl
    inc si
    loop range
    jmp finishcount
finishcount:
    lea dx,info1
    mov ah,9
    int 21h
    mov dl,0dH
    mov ah,2
    int 21h
    mov dl,0ah
    mov ah,2
    int 21h
    lea di,tj
    mov cx,5
printnum:
    mov dl,[di]
    inc di
    add dl,30h   ;将数字转换为ascii码才能输出正确的数字(仅限个位数)
    mov ah,2
    int 21h
    mov dl,20h
    mov ah,2
    int 21h
    loop printnum
    mov dl,0dH
    mov ah,2
    int 21h
    mov dl,0ah
    mov ah,2
    int 21h
    mov cx,11   ;计数结束，进行排序，重新赋值cx为数组个数
;选择排序 
;for (i = 0 ; i < len - 1 ; i++){
;     int min = i;
;     for (j = i + 1; j < len; j++)     //走訪未排序的元素
;         if (arr[j] < arr[min])    //找到目前最小值
;             min = j;   //紀錄最小值
;      swap(&arr[min], &arr[i]);    //做交換
; }
outloop:;cx为默认循环的确定条件
    mov si,11      ;选择排序(找到后面最小的数字，拿到前面)
    sub si,cx
    mov bl,buf[si]  ;选定第一个数字为最大数
    mov dx,si       ;dx用于存放最大数的位置
interloop:
    inc si
    cmp bl,buf[si]
    jl changem
    cmp si,11
    jl interloop
    jmp gooutinterloop
changem:
    mov bl,buf[si] 
    mov dx,si
    cmp si,11  
    jl interloop
    jmp gooutinterloop    
gooutinterloop:
    mov si,11
    sub si,cx
    xchg buf[si],bl
    mov di,dx
    xchg bl,buf[di]
    loop outloop
finish:
    lea si,buf
	mov cx,11
    mov bx,0ah
    lea dx,info2
    mov ah,9
    int 21h
    mov dl,0dh
    mov ah,2
    int 21h
    mov dl,0ah
    mov ah,2
    int 21h
print:
    mov al,buf[si]
    mov ah,00h
    inc si
    div bl;字节默认用ax/oprd
    mov dh,ah
    mov dl,al
    add dl,30h
    mov ah,2
    int 21h
    mov dl,20h
    mov ah,2
    int 21h
    loop print
    mov ah,4ch
    int 21h
code ends
end begin


