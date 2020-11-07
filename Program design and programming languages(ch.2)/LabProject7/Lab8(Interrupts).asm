.model tiny                
.code                      
org 100h 

start:
jmp main
key db "keyLab"
sizeOfCMD db 0
textOfCMD db 126 dup (0)
delayString db 50 dup (0)
delayNumber dw 0
commandLineErrorStr db "Incorrect number of parameters", 10, 13, '$'
strOff db "New interrupts are off", 10, 13, '$'
strOn db "New interrupts are on", 10, 13, '$'
endl db 10, 13, '$'
countComWord dw 0
sizeWord dw 0
screen dw 2000 dup (0)
oldTimerOffset dw 0  
oldTimerSegment dw 0
oldKeyboardOffset dw 0  
oldKeyboardSegment dw 0
count dw 0
countM dw 0
flag dw 0

newTimerHundler proc far
    pusha
    push ds
    push cs
    pop ds
    cmp flag, 1
    je endTimeHundler
    inc countM
    cmp countM, 18
    jne endTimeHundler
    mov countM, 0
    inc count
    mov ax, delayNumber
    cmp ax, count
    jne endTimeHundler
    mov ax, 0B800h
    mov es, ax
    xor di, di
    loopSaveScreen:
        mov ax, es:[di]
	    mov [screen + di], ax
	    add di, 2
	    cmp di, 4000    
    jl loopSaveScreen
    xor di, di
    loopClearScreen:
        mov es:[di], 0F20h
	    add di, 2
	    cmp di, 4000    
    jl loopClearScreen
    mov ah, 01h
    mov cx, 2000h
    int 10h;             
    mov count, 0
    mov countM, 0
    mov flag, 1
    endTimeHundler:
    pushf
    call cs:dword ptr oldTimerOffset 
    pop ds   
    popa
    iret
newTimerHundler endp

newKeyboardHundler proc far
    pusha
    push ds
    push cs
    pop ds
    cmp flag, 0
    je endKeyboardHundler     
    mov ax, 0B800h
    mov es, ax
    xor di, di
    loopShowScreen:        
	    mov ax, [screen + di]
	    mov es:[di], ax
	    add di, 2
	    cmp di, 4000    
    jl loopShowScreen
    mov ah, 01h
    mov cx, 0607h
    int 10h
    mov flag, 0
    endKeyboardHundler:
    mov count, 0
    mov countM, 0
    pushf
    call cs:dword ptr oldKeyboardOffset
    pop ds
    popa
    iret 
newKeyboardHundler endp    

programInst:

print proc     
    mov ah, 09h
    int 21h 
    ret
print endp
	
checkCMD proc
    pusha                                                                      
    xor cx, cx              
	mov cl, ds:[80h]
    cmp cl, 0		 
	je endMainProc
	mov sizeOfCMD, cl
	mov si, 81h             
	lea di, textOfCMD       
	rep movsb
    mov cl, sizeOfCMD
    lea di, textOfCMD
    dec di
    loopCheckCMD:
        cmp cl, 0
        je continue
        dec cl
        inc di
        cmp [di], ' '
        je loopCheckCMD
        cmp [di], 9
        je loopCheckCMD
        cmp [di], '9'
        ja endMainProc
        cmp [di], '0'
        jb endMainProc 
    jmp loopCheckCMD
    continue:
    lea si, textOfCMD
    dec si
	skeepSpace:
	    inc si              
	    mov al, ds:[si]         
	    cmp al, ' '                                                                       
	    je skeepSpace
	    cmp al, 9
	    je skeepSpace 
	lea di, delayString
	loopReadingDelayString:              
	    mov al, ds:[si]         
	    cmp al, ' '       
        je endReadingCMD
        cmp al, 9
        je endReadingCMD
        cmp al, 0
        je endReadingCMD                      
	    mov es:[di], al
	    inc sizeWord                                  
	    inc di                     
	    inc si                               
	jmp loopReadingDelayString        
	endReadingCMD:
	cmp sizeWord, 0
	je endRead
	inc countComWord
	endRead:
	cmp countComWord, 1
	jne endMainProc
	cmp sizeWord, 3
	ja endMainProc
    popa
    ret
    endMainProc:
    lea dx, commandLineErrorStr
    call print
    popa
    .exit
checkCMD endp 

stringToNumber proc
    pusha
    mov cx, 10
    mov di, sizeWord
    dec di
    mov bx, 1
    loopCTN:
        xor ax, ax
        mov al, delayString[di]
        sub al, 48    
        mul bx
        add delayNumber, ax
        cmp di, 0
        je endLoopCTN
        mov ax, bx        
        mul cx
        mov bx, ax
        dec di
    jmp loopCTN
    endLoopCTN:
    popa
    ret
stringToNumber endp

setNewInterrupt proc
    pusha
    push cs
    pop ds
    mov ax, 3508h 
    int 21h
    mov word ptr oldTimerOffset, bx 
    mov word ptr oldTimerSegment, es
    lea di, key
    lea si, key
    mov cx, 6
    repe cmpsb
    je endProc    
    mov ax, 2508h 
    lea dx, newTimerHundler    
    int 21h      
    mov ax, 3509h
    int 21h
    mov word ptr oldKeyboardOffset, bx
    mov word ptr oldKeyboardSegment, es    
    mov ax, 2509h
    lea dx, newKeyboardHundler
    int 21h
    lea dx, strOn
    call print
    popa
    ret
    endProc:
    push ds
    mov ax, 2508h 
    mov ds, es:oldTimerSegment
    mov dx, es:oldTimerOffset    
    int 21h
    mov ax, 2509h    
    mov ds, es:oldKeyboardSegment
    mov dx, es:oldKeyboardOffset
    int 21h
    pop ds
    lea dx, strOff
    call print
    .exit    
setNewInterrupt endp    

main:    
	call checkCMD
	call stringToNumber
	call setNewInterrupt
	mov ax, 3100h                                                 	                            
	mov dx, (programInst - start + 100h) / 16 + 1 	                                  
	int 21h 
end main                                                                                                                                                                                      