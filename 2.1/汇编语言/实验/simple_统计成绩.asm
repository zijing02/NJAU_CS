DATA SEGMENT
	BUF DB 89,76,91,93,58,64,83,81,45,72,84
	TJ  DB 5 DUP(0)           ;0位置放100到90的人数，1位置放90到80的人数
	INFO1 DB 0DH,0AH,'THE numberof people between100-90 90-80 80-70 70-60 59-0 is:$'
	INFO2 DB 0DH,0AH,'cong gao dao di pai xu hou ,11 ge ren de cheng ji xu lie wei:$'
DATA ENDS                     ;2位置放80到70的人数，3位置放70到60的人数
CODE SEGMENT				  ;4位置放60以下到人数
ASSUME CS:CODE,DS:DATA
START:MOV AX,DATA
	  MOV DS,AX
	  MOV CX,11
	  LEA SI,BUF
	  LEA DI,TJ
RS:	  MOV AL,[SI]
	  CMP AL,90           ;大于等于90分，跳转到S1
	  JGE S1			
	  CMP AL,80	      ;大于等于80分，跳转到S2
	  JGE S2
	  CMP AL,70           ;大于等于70分，跳转到S3
	  JGE S3
	  CMP AL,60           ;大于等于60分，跳转到S4
	  JGE S4
	  MOV BL,[DI+4]       ;小于60分,对应位置加一
	  INC BL
	  MOV [DI+4],BL
	  INC SI
	  LOOP RS
	  JMP TJWB            ;CX=0，统计结束
S1:  MOV BL,[DI]          ;存放90分以上的人数的存储单元加一
	 INC BL
	 MOV [DI],BL
	 INC SI
	 LOOP RS
	 JMP TJWB
S2:  MOV BL,[DI+1]
	 INC BL
	 MOV [DI+1],BL
	 INC SI
	 LOOP RS
	 JMP TJWB
S3:  MOV BL,[DI+2]
	 INC BL
	 MOV [DI+2],BL
	 INC SI
	 LOOP RS
	 JMP TJWB
S4:  MOV BL,[DI+3]
	 INC BL
	 MOV [DI+3],BL
	 INC SI
	 LOOP RS
	 JMP TJWB
TJWB:
     LEA DX,INFO1
	 MOV AH,9
	 INT 21H
	 MOV DL,0DH
	 MOV AH,2
	 INT 21H
	 MOV DL,0AH
	 MOV AH,2
	 INT 21H
	 LEA DI,TJ
	 MOV CX,5
CTJ: MOV DL,[DI]              ;输出统计的结果，各分数段到人数
     INC DI
	 ADD DL,30H
	 MOV AH,2
     INT 21H
	 MOV DL,20H
	 MOV AH,2
	 INT 21H
	 LOOP CTJ
	 MOV DL,0DH
	 MOV AH,2
	 INT 21H
	 MOV DL,0AH
	 MOV AH,2
	 INT 21H 
	 MOV CX,11    ;统计完毕，开始排序，CX赋值为11
WXH: MOV SI,11    ;采用选择排序，
	 SUB SI,CX    ;SI每次从11-CX开始做循环，当SI<=11时，内循环继续
	 MOV BL,BUF[SI]    ;将每次内循环开始的第一个数放在BL中，当作目前最大数
	 MOV DX,SI         ;DX用来记录找到的最大数的位置
NXH:
	 INC SI            ;内循环每次SI加一
	 CMP BL,BUF[SI]    ;比较SI所指向的数与当前的最大数的大小
	 JL  GJL           ;若SI所指向的数大于当前的最大数，则将当前最大数换为SI所指向的数
	 CMP SI,11         ;SI小于11时内循环继续
	 JL  NXH           ;继续执行内循环
	 JMP CHUNEI        ;内循环结束，出内循环
	 

GJL: MOV BL,BUF[SI]    ;更改当前最大数
     MOV DX,SI         ;DX用以记录找到最大数的位置
	 CMP SI,11         ;若SI小于等于11则继续执行内循环
	 JLE NXH
	 JMP CHUNEI        ;否则，出内循环
	 
	 
	 
CHUNEI:MOV SI,11        ;找到本次内循环开始的位置
       SUB SI,CX
	   XCHG BUF[SI],BL  ;将本次找到的最大数放到本次内循环的开始位置
	   MOV DI,DX        ;同时更改原来最大数位置的内容，换为本次循环开始的位置中的数
	   XCHG BL,BUF[DI]
	   LOOP WXH         ;到外循环
FINISH:LEA SI,BUF
	   MOV CX,11
	   MOV BX,0AH
	   LEA DX,INFO2           ;输出排序后到成绩
	   MOV AH,9
	   INT 21H
	   MOV DL,0DH
	   MOV AH,2
	   INT 21H
	   MOV DL,0AH
	   MOV AH,2
	   INT 21H
PRINT: MOV AL,BUF[SI]
	   MOV AH,00H
	   INC SI
	   DIV BL
	   MOV DH,AH
	   MOV DL,AL
	   ADD DL,30H
	   MOV AH,2
	   INT 21H
	   MOV DL,DH
	   ADD DL,30H
	   MOV AH,2
	   INT 21H
	   MOV DL,20H
	   MOV AH,2
       INT 21H
	   LOOP PRINT
	   MOV AH,4CH
	   INT 21H
CODE ENDS
END START
	  