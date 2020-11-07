.model small
.stack 100h

.data
msgString1 db "Current number system: $"
msgString2 db 0Ah, 0Dh, "Desired number system: $"
msgString3 db 0Ah, 0Dh, "Number: $"
msgResult db 0Ah, 0Dh, "Result number: $" 
msgError1 db 0Ah, 0Dh, "Incorrect input!$" 

NumSystem db "0123456789ABCDEF"
 
CurNumSys db 3
CurNumSysLenght db '$'
CurNumSysStr db 3 dup('$')

DesNumSys db 3
DesNumSysLenght db '$'
DesNumSysStr db 3 dup('$')

NumberSought db 18
NumberSoughtLenght db '$'
NumberSoughtStr db 18 dup('$')

CurNumSysNum dw 0
DesNumSysNum dw 0
NumberSoughtNum dw 0 

DecimalNumSys dw 10
Size dw 0    
FlagMinus dw 0

ValidationOfInput macro Temp_str, Size_temp_str, Point
    mov si, offset Temp_str                          
    xor cx, cx       
    mov cl, [Size_temp_str]
    cmp cl, 0
    je error_exit
    Point:                                              
	    lodsb			
	    cmp al,'9'	
	    jnbe error_exit
	    cmp al,'0'      
	    jb error_exit 
    loop Point                                                                         
endm

ValidationRange macro Number
    xor ax, ax
    mov ax, Number			
	cmp ax, 16	
	jnbe error_exit
	cmp al, 2      
    jb error_exit                                                                         
endm

.code                          
start:                         
    mov ax, @data              
    mov ds, ax                                         
        
    push offset msgString1             
    call OutputString 
                                     
    push offset CurNumSys                                 
    call InputString
    ValidationOfInput CurNumSysStr, CurNumSysLenght, Point1
        
    lea si, CurNumSysStr
    mov al, CurNumSysLenght
    mov bx, DecimalNumSys
    push bx  
    call Atoi
    mov CurNumSysNum, dx
    ValidationRange CurNumSysNum
    
    push offset msgString2             
    call OutputString
    
    push offset DesNumSys                                 
    call InputString
    ValidationOfInput DesNumSysStr, DesNumSysLenght, Point2
        
    lea si, DesNumSysStr
    mov al, DesNumSysLenght
    mov bx, DecimalNumSys
    push bx  
    call Atoi
    mov DesNumSysNum, dx
    ValidationRange DesNumSysNum
    
    push offset msgString3             
    call OutputString
    
    push offset NumberSought                                 
    call InputString
        
    lea si, NumberSoughtStr
    xor ax, ax
    mov al, NumberSoughtLenght
    cmp [si], '-'
    jne continue
    push ax
    mov ax, FlagMinus
    inc ax
    mov FlagMinus, ax
    pop ax
    inc si
    dec al
    continue:
    call ValidationNumber 
    mov bx, CurNumSysNum
    push bx 
    call Atoi
    mov NumberSoughtNum, dx
    
    push offset msgResult             
    call OutputString
    
    xor di, di
    mov ax, NumberSoughtNum
    call Itoa
    
    push offset NumberSoughtStr             
    call OutputString  
                
    call Exit                                   

    error_exit:
        push offset msgError1
        call OutputString
        call Exit
                                         
OutputString proc    
    mov bp, sp      
    mov dx, [bp + 2]                                 
    mov ax, 0900h               
    int 21h                                    
    ret 2                       
OutputString endp 

InputString proc    
    mov bp, sp      
    mov dx, [bp + 2]                                 
    mov ax, 0A00h               
    int 21h                                                                          
    ret 2                        
InputString endp

Atoi proc
    mov bp, sp
    xor cx, cx
    mov cl, al
    xor bx, bx
	xor dx,dx	
    cycle:	
        xor ax,ax
    	lodsb
	    cmp	ax, '9'
	    jbe temp_point1		    
	    cmp ax, 'F'
	    jbe temp_point2
	    return_temp1:
	    mov bx, dx
	    push ax
	    mov ax, [bp + 2]
	    mul bx
	    cmp dx, 0 
	    jne exit_point1
	    mov bx, ax
	    pop ax		
	    add bx, ax
	    mov dx, bx
    loop cycle
exit_point2:
	ret 2
temp_point1:
    sub ax, '0'
    jmp return_temp1
temp_point2:
    sub ax, 55
    jmp return_temp1
exit_point1:
    pop ax
    mov dx, bx
    jmp error_exit     
Atoi endp

Itoa proc    
    xor dx, dx
    cmp ax, 0
    je temp_point3
    inc di
    push ax
    div DesNumSysNum     
    call Itoa
    pop ax
    lea si, NumberSoughtStr
    push ax
    mov ax, FlagMinus
    cmp ax, 1
    jne continue2
    inc si             
    continue2:
    pop ax
    add si, Size
    sub si, di
    xor dx, dx
    idiv DesNumSysNum
    push si
    lea si, NumSystem
    add si, dx     
    mov al, [si]
    pop si
    mov [si], al  
    dec di
    jmp exit_point
    temp_point3:
    push ax
    mov ax, FlagMinus
    cmp ax, 1
    jne continue1
    inc di             
    continue1:
    pop ax             
    add [Size], di
    lea si, NumberSoughtStr
    add si, Size
    mov [si], '$'
    exit_point: 
    ret 
Itoa endp

ValidationNumber proc
    push si
    push ax
    xor cx, cx
    mov cx, ax
    dec si
    Cycle3:
        inc si
        mov al, [si]
        xor bx, bx
        lea di, NumSystem
        mov bx, CurNumSysNum
        Cycle5:
            cmp [di], al
            je cont1
            inc di
            dec bx
            cmp bx, 0
            jne Cycle5
            jmp error_exit
        cont1:          
        loop Cycle3
    pop ax
    pop si
    ret
ValidationNumber endp

Exit proc
    xor ax, ax    
    mov ah,4ch               
    int 21h  
    ret   
Exit endp

end start



;Atoi proc
;    xor cx, cx
;    mov cl, al
;	xor dx,dx	
;    cycle:	
;        xor ax,ax
;    	lodsb		
;	    sub ax,'0'	
;	    shl dx, 1	
;	    add ax, dx
;	    shl dx, 2
;	    add dx, ax	
;    loop cycle	
;	ret
;Atoi endp



;CLEAR PROC 
;    PUSHA
;    MOV CX, 64000       
;    MOV AX, 0A000h     
;    MOV ES, AX          
;    MOV AL, 00010100b
;    XOR DI, DI
;    CLD         
;    REP STOSB       
;    POPA
;    RET
;CLEAR ENDP 