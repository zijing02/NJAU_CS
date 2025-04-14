;程序接受用户从键盘输入的一行字符（字符个数不超过80个字符，该字符串以回车符结束），
;并按字母、数字及其他字符分类统计个数，然后将相应的结果存放于letter、digit和other中
;0->48(30H),9->57(39H),A->65(41H),Z->90(5aH),a->97(61H),z->122(7aH)
;只需要统计字符和数字的数量即可，其他字符的数量用总数减去
data segment
    buf db 10,'1qq123',3 dup(0)        ;预定义字符串
data ends
code segment
    assume ds:data,cs:code 
begin:
    mov ax,data
    mov ds,ax
    mov cx,10           ;总共的数量，用于循环
    mov bx,0            ;视为数组下标
    mov dx,0            ;
do:
    inc bx              ;此处先自增说明数组从1开始
    mov al,buf[bx]
    cmp al,61H
    jge letter_l
    cmp al,41H           ;41H->A
    jge letter_U         ;大于等于跳转
    cmp al,30H           ;30h->0
    jge digit             
    dec cx              
    jnz do              ;判断cx是否为0，不为0继续循环
    jz done             ;为0跳到done
letter_l:
    cmp al,7aH          ;7aH->z
    jle count_letter    ;小于等于跳转
    dec cx
    jnz do
    jz done
letter_U:
    cmp al,5a           ;5a->Z
    jle count_letter
    dec cx
    jnz do
    jz done
count_letter:
    add dl,1
    dec cx 
    jnz do
    jz done
digit:
    cmp al,39H          ;39H->9
    jle count_digit
    dec cx
    jnz do
    jz done
count_digit:
    add dh,1
    dec cx 
    jnz do
    jz done
done:
    mov byte ptr buf[11],dh
    mov byte ptr buf[12],dl
    mov al,buf
    sub al,dh
    sub al,dl
    mov byte ptr buf[13],al
code ends
    end begin