.model small
.stack 100h
.data
beginData:
fileName db 50 dup (0)    
textOfCMD db 126 dup (0)
descriptor dw 0
readBytes dw 0
sizeTempBuffer dw 0
tempBuffer db 500 dup ('$')
curPosH dw 0 
curPosL dw 0
endl db 10, 13, '$'
endFlag db 0
countComWord dw 0
sizeWord dw 0
commandLineErrorStr db "Incorrect number of parameters$"
errorFileOpenStr db "Could not open file$"
errorReadingFileStr db "Error reading file$"
ddd db "program1.exe", 0
tempChar db 0
sizeNameProg dw 0
nameProg db 50 dup(0)
EPB dw 0
dw 0, 0
dw 005Ch, 0, 006Ch, 0
endData:

.code
closeFile macro descriptorFile               
	pusha                                 
	mov ah, 3Eh               
	mov bx, descriptorFile          
	int 21h                                    		          
	popa                                   
closeFile endm 

print macro str     
    pusha
    mov ah, 09h
    lea dx, str
    int 21h 
    popa
print endm

main:
	mov bx, ((endCode - main) / 16 + 1) + ((endData - beginData) / 16 + 1) + 32
    mov ah, 4ah
    int 21h
	mov ax, @data          
	mov es, ax                                    
	xor cx, cx              
	mov cl, ds:[80h]
	dec cl		 		
	mov si, 82h             
	lea di, textOfCMD       
	rep movsb              
	mov ds, ax                           
	call readingCMD
	cmp countComWord, 1
	jne commandLineError         
	call openFile
	call mainLoop   			                         
	closeFile descriptor
    endMainProc:            
	.exit                                         
    commandLineError:
        print commandLineErrorStr
        print endl
    jmp endMainProc
    errorFileOpen:
        print errorFileOpenStr
        print endl
    jmp endMainProc
    errorReadingFile:
        print errorReadingFileStr                                                           
        print endl                                                                          
    jmp endMainProc

readString proc
    pusha
    mov bx, 0
    loopReadStr:
        call setReadingPosition
        push bx
        mov ah, 3Fh       
        mov bx, descriptor                                                            
        mov cx, 1      
        lea dx, tempChar   
        int 21h
        pop bx
        jc errorReadingFile
        cmp ax, 0
        jne continue1
        mov endFlag, 1
        jmp endReadFile
        continue1:
        cmp tempChar, 13
        je doubleIncCurPosL        
        clc                           
        add curPosL, ax   
        jc incCurPosH1           
        jmp continue2    
        incCurPosH1:                   
        inc curPosH
        continue2:
        mov al, tempChar
        mov tempBuffer[bx], al
        inc bx       
    jmp loopReadStr       
    endReadFile:
    popa
    ret
    doubleIncCurPosL:
    clc
    add curPosL, 2   
    jc incCurPosH2           
    jmp continue3    
    incCurPosH2:                   
    inc curPosH
    continue3:
    jmp endReadFile
readString endp
    
mainLoop proc
    pusha
    processLoop:
        lea di, nameProg
        mov al, '$'
        mov cx, sizeNameProg
        rep stosb
        lea di, tempBuffer
        mov al, '$'
        mov cx, sizeTempBuffer
        rep stosb
        call readString
        lea si, tempBuffer
	    call strlen
	    mov sizeTempBuffer, ax
	    cmp sizeTempBuffer, 0
	    je continue0
	    call readingTempBuffer
	    lea si, nameProg
	    call strlen
	    mov sizeNameProg, ax	    
	    lea dx, nameProg
        lea bx, EPB 
        mov ax, 4B00h 
        int 21h
	    continue0:        
        cmp endFlag, 1
        je endProcessLoop        
    jmp processLoop
    endProcessLoop:
    popa
    ret    
mainLoop endp

readingTempBuffer proc                                
    pusha                                                            
	lea si, tempBuffer
    dec si
	skeepSpaceTempBuffer:
	    inc si              
	    mov al, ds:[si]         
	    cmp al, ' '                                                                       
	    je skeepSpaceTempBuffer
	    cmp al, 9
	    je skeepSpaceTempBuffer 
	lea di, nameProg
	loopReadingTempBuffer:              
	    mov al, ds:[si]         
	    cmp al, ' '       
        je endReadingTempBuffer
        cmp al, 9
        je endReadingTempBuffer
        cmp al, '$'
        je endReadingTempBuffer                      
	    mov es:[di], al                                 
	    inc di                     
	    inc si                               
	jmp loopReadingTempBuffer        
	endReadingTempBuffer:
	mov al, 0        
	mov es:[di], al                                                                                                                                         
    popa                                    
	ret	                                      
readingTempBuffer endp

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

setReadingPosition proc         
    pusha
    mov ax, 4200h     
    mov bx, descriptor                                                      
    mov dx, curPosL   
    mov cx, curPosH              
    int 21h     
    popa   
    ret
setReadingPosition endp

openFile proc                  
    pusha                                           
	mov ax, 3D02h			      			     
	lea dx, fileName        
	mov cx, 0			      
	int 21h
	jc errorFileOpen                  	      
	mov descriptor, ax	       			                       
    popa                    
	ret                        
openFile endp

readingCMD proc                                
    pusha                                                            
	lea si, textOfCMD
    dec si
	skeepSpace:
	    inc si              
	    mov al, ds:[si]         
	    cmp al, ' '                                                                       
	    je skeepSpace
	    cmp al, 9
	    je skeepSpace 
	lea di, fileName
	loopReadingFileName:              
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
	jmp loopReadingFileName        
	endReadingCMD:
	mov al, 0        
	mov es:[di], al
	cmp sizeWord, 0
	je endRead
	inc countComWord
	endRead:                                                                                                                                          
    popa                                    
	ret	                                      
readingCMD endp

endCode:

end main