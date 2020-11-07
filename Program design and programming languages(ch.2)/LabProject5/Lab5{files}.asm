.model small
.stack 100h
.data    
maxSizeOfCMD equ 126
sizeOfCMD db 0
textOfCMD db 120 dup (0)
filePath db 120 dup (0)
sizeFileL dw 0
sizeFileH dw 0
maxlength equ 30
replaceableWord db 50 dup(0)
replacementWord db 50 dup(0)
sizeReplaceableWord dw 0
sizeReplacementWord dw 0
descriptor dw 0
tempFile db '\', 13 dup(0), 0
tempDescriptor dw 0
readBytes dw 0
allReadBytesL dw 0
allReadBytesH dw 0
sizeTempBuffer dw 0
countBytes dw 32h
tempBuffer db 500 dup ('$')
curPosH dw 0 
curPosL dw 0
tempFileCurPosH dw 0
tempFileCurPosL dw 0
endl db 10, 13, '$'
posWord dw 0
flag db 0
countComWord dw 0
sizeWord dw 0
commandLineErrorStr db "Incorrect number of parameters$"
errorFileOpenStr db "Could not open file$"
errorSetSizeFileStr db "Could not find out file size$"
errorReadingFileStr db "Error reading file$"

.code
closeFile macro descriptorFile               
	push bx                    
	push cx                                 
	mov ah, 3Eh               
	mov bx, descriptorFile          
	int 21h                                    		          
	pop cx                     
	pop bx                                   
closeFile endm 

print macro str     
    pusha
    mov ah, 09h
    lea dx, str
    int 21h 
    popa
print endm

main:
	mov ax, @data           
	mov es, ax                                    
	xor cx, cx              
	mov cl, ds:[80h]
	dec cl		
	add sizeOfCMD, cl 		
	mov si, 82h             
	lea di, textOfCMD       
	rep movsb              
	mov ds, ax                           
	call readingCMD
	cmp countComWord, 3
	jne commandLineError           
	lea si, replaceableWord
	call strlen
	mov sizeReplaceableWord, ax
	lea si, replacementWord
	call strlen
	mov sizeReplacementWord, ax
	call openFile
	call createTempFile					
	call setSizeFile        			                          
	call changeFile    			                         
	closeFile descriptor
	closeFile tempDescriptor
	mov ah, 41h                         
    lea dx, filePath
    int 21h  
    mov ah, 56h                        
    lea dx, tempFile
    lea di, filePath    
    int 21h
    endMainProc:            
	mov ah, 4Ch         
	int 21h                                         
    commandLineError:
    print commandLineErrorStr
    print endl
    jmp endMainProc
    errorFileOpen:
    print errorFileOpenStr
    print endl
    jmp endMainProc
    errorSetSizeFile:
    print errorSetSizeFileStr
    print endl
    jmp endMainProc
    errorReadingFile:
    print errorReadingFileStr
    print endl
    jmp endMainProc
    
changeFile proc
    LoopReadingFile:
        call setReadingPosition
        call readFile
        cmp flag, 1
        je endChange
        call deleteLastWord
        lea si, tempBuffer
        call strlen
        mov sizeTempBuffer, ax
        print tempBuffer
        print endl
        call changeTempBuffer 
        print tempBuffer
        print endl
        call writeTempFile
    jmp LoopReadingFile
    endChange:
    ret
changeFile endp

readFile proc   
    push bx
    push cx
    push dx       
    mov bx, descriptor               
    mov ah, 3Fh                                             
    mov cx, countBytes      
    lea dx, tempBuffer   
    int 21h
    jc errorReadingFile
    cmp ax, 0
    jne continue1
    mov flag, 1
    continue1:     
    mov readBytes, ax          
    clc                           
    add curPosL, ax   
    jc incCurPosH           
    jmp endReadFile    
    incCurPosH:                   
    inc curPosH
    endReadFile:                
    pop dx
    pop cx
    pop bx
    ret
readFile endp

deleteLastWord proc         
    push si
    lea si, tempBuffer             
    add si, readBytes   
    dec si 
    LoopDeletLastWord:
       cmp [si], '$'               
       je notLastword   
       cmp [si], 0                 
       je notLastword   
       cmp [si], ' '                 
       je notLastword   
       mov [si], '$'                 
       dec si
       dec curPosl      
       dec readBytes                
       jmp LoopDeletLastWord                                  
    notLastword:   
    pop si
    ret
    endOfFile:
       dec curPosl      
       dec readBytes
    jmp notLastword
deleteLastWord endp           

setSizeFile proc            
    pusha    
    xor cx, cx                     
    xor dx, dx
    mov ax, 4202h                                       
    mov bx, descriptor              
    int 21h
    jc errorSetSizeFile 
    mov sizeFileL, ax
    mov sizeFileH, dx             
    popa
    ret 
setSizeFile endp

setReadingPosition proc         
    push ax
    push bx     
    mov bx, descriptor               
    mov al, 0                                        
    mov dx, curPosL   
    mov cx, curPosH        
    mov ah, 42h  
    int 21h     
    pop bx
    pop ax    
    ret
setReadingPosition endp

openFile proc                  
	push bx                     
	push dx                                             
	mov ax, 3D02h			      			     
	lea dx, filePath        
	mov cx, 0			      
	int 21h
	jc errorFileOpen                  	      
	mov descriptor, ax	       			                       
	pop dx                     
	pop bx                     
	ret                        
openFile endp  

createTempFile proc                    
    pusha
    mov ah, 5Ah                         
    mov cx, 0h                          
    lea dx, tempFile
    int 21h
    mov tempDescriptor, ax                              
    popa
    ret
createTempFile endp	

writeTempFile proc            
    pusha
    clc 
    mov bx, tempDescriptor
    mov al, 0                    
    mov dx, tempFileCurPosL         
    mov cx, tempFileCurPosH         
    mov ah, 42h                      
    int 21h    
    mov ah, 40h                       
    mov bx, tempDescriptor              
    mov cx, sizeTempBuffer      
    lea dx, tempBuffer             
    int 21h
    push cx    
    lea di, tempBuffer
    mov al, '$'
    mov cx, sizeTempBuffer
    rep stosb
    pop cx  
    clc                            
    add tempFileCurPosL, cx 
    jc incTempFileCurPosH              
    jmp endWriteTempFile    
    incTempFileCurPosH:
    inc tempFileCurPosH      
    endWriteTempFile:                       
    popa
    ret
writeTempFile endp 

strlen proc                   
	push bx                    
	push si                                           
	xor ax, ax                                              
    loop_1:                  
	    mov bl, ds:[si]        
	    cmp bl, 0            
	    je endLoop_1 
	    cmp bl, '$'
	    je endloop_1                                           
	    inc si                  
	    inc ax                                                                    
	    jmp loop_1           
    endLoop_1:                    
	pop si                      
	pop bx                      
	ret                         
strlen endp 

readingCMD proc                                
	push bx                                
	push cx                                
	push dx                                  
    xor cx, cx                                          
	mov cl, sizeOfCMD                          
	lea si, textOfCMD
	dec si
	skeepSpace0:
	    inc si              
	    mov al, ds:[si]         
	    cmp al, ' '                                                                       
	    je skeepSpace0
	    cmp al, 9
	    je skeepSpace0
	lea di, filePath
	loopReadingPath:              
	    mov al, ds:[si]         
	    cmp al, ' '       
        je isEndOfWord1
        cmp al, 9
        je isEndOfWord1
        cmp al, 0
        je endReadingCMD                     
	    mov es:[di], al
	    inc sizeWord                                  
	    inc di                     
	    inc si                               
	jmp loopReadingPath         
    isEndOfWord1:           
	mov al, 0        
	mov es:[di], al
	inc countComWord
	mov sizeWord, 0
	dec si
	skeepSpace1:
	    inc si              
	    mov al, ds:[si]         
	    cmp al, ' '
	    je skeepSpace1
	    cmp al, 9
	    je skeepSpace1                                                                       	
	lea di, replaceableWord
	loopReadingReplaceableWord:              
	    mov al, ds:[si]         
	    cmp al, ' '       
        je isEndOfWord2
        cmp al, 9
        je isEndOfWord2
        cmp al, 0
        je endReadingCMD                      
	    mov es:[di], al
	    inc sizeWord                                  
	    inc di                     
	    inc si                               
	jmp loopReadingReplaceableWord         
    isEndOfWord2:
    mov al, 0        
	mov es:[di], al
    inc countComWord
	mov sizeWord, 0
    dec si
	skeepSpace2:
	    inc si              
	    mov al, ds:[si]         
	    cmp al, ' '                                                                       
	    je skeepSpace2
	    cmp al, 9
	    je skeepSpace2 
	lea di, replacementWord
	loopReadingReplacementWord:              
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
	jmp loopReadingReplacementWord        
	endReadingCMD:
	mov al, 0        
	mov es:[di], al
	cmp sizeWord, 0
	je endRead
	inc countComWord
	endRead:                                                                                                                                          
	pop dx                                  
	pop cx                                  
	pop bx                                    
	ret	                                      
readingCMD endp

changeTempBuffer proc
    mov posWord, 0
    findLoop:
        mov cx, sizeTempBuffer
        sub cx, sizeReplaceableWord
        jc noFind
        cmp cx, posWord
        jb noFind                 
        mov cx, sizeReplaceableWord
        mov bx, posWord
        lea di, replaceableWord
        lea si, tempBuffer[bx] 
        repe cmpsb
        jz continue
        inc posWord
    jnz findLoop
    continue:
    mov bx, sizeReplaceableWord
    cmp bx, sizeReplacementWord        
    je swapStr1
    ja swapStr2
    jna swapStr3
    swapStr1:
        call swapStr1p
        jmp findLoop
    swapStr2:
        call swapStr2p
        jmp findLoop
    swapStr3:
        call swapStr3p
        jmp findLoop     
    noFind:
    ret
changeTempBuffer endp 

swapStr1p proc
    lea di, tempBuffer
    add di, posWord 
    lea si, replacementWord       
    mov cx, sizeReplaceableWord
    rep movsb  
    ret
swapStr1p endp

swapStr2p proc      
    lea di, tempBuffer
    add di, posWord
    lea si, replacementWord       
    xor cx, sizeReplacementWord
    rep movsb     
    push di 
    mov bx, sizeReplaceableWord
    sub bx, sizeReplacementWord    
    add di, bx        
    lea ax, tempBuffer
    push bx
    mov bx, sizeTempBuffer             
    add ax, bx
    pop bx             
    sub ax, di                 
    mov cx, ax                   
    inc cx                          
    shiftLeft:                  
        mov ah, [di]             
        sub di, bx              
        mov [di], ah            
        add di, bx             
        inc di                  
    loop shiftLeft     
    sub di, bx
    dec di
    mov al, '$'
    mov cx, bx
    rep stosb
    sub sizeTempBuffer, bx
    pop di              
    ret
swapStr2p endp

swapStr3p proc
    lea di, tempBuffer
    add di, posWord
    add di, sizeReplaceableWord        
    mov dx, sizeReplacementWord
    sub dx, sizeReplaceableWord                         
    lea bx, tempBuffer
    mov ax, sizeTempBuffer
    add bx, ax
    mov si, bx
    sub bx, di
    mov cx, bx
    inc cx        
    add sizeTempBuffer, dx               
    shiftRight:                  
        mov ah,[si]
        push si              
        add si, dx             
        mov [si], ah              
        pop si               
        dec si                   
    loop shiftRight            
    mov ax, di
    sub ax, sizeReplaceableWord
    mov di, ax 
    lea si, replacementWord       
    mov cx, sizeReplacementWord
    rep movsb                    
    ret
swapStr3p endp 

end main

;strcmp proc
;    cld
;    lea di, replaceableWord
;    lea si, replacementWord
;    mov cx, sizeReplaceableWord
;    cmpsb
;strcmp endp