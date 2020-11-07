.model small
.stack 100h
.data
msgString db "Enter string:", 0Ah, 0Dh, "$"
msgSubstring1 db 0Ah, 0Dh, "Replaceable word: $"
msgSubstring2 db 0Ah, 0Dh, "Replacement word: $"
msgResult db 0Ah, 0Dh, "Result string:", 0Ah, 0Dh, "$" 
msgError db 0Ah, 0Dh, "Incorrect input! Substring is empty or contains more than 1 words or spaces!$" 

enterStr db 0Ah, 0Dh, '$'
max_length equ 200   
 
mainStrLength db '$'                    
Strl db max_length                                  
String  db max_length + 1 dup('$')                         
                               
SubStrLength1 db '$'                 
SubStr1 db max_length               
SubString1 db max_length dup('$') 

SubStrLength2 db '$'                
SubStr2 db max_length               
SubString2 db max_length dup('$')    

exitIfNotSingleWord MACRO Temp_str, Size_temp_str, Point
    push cx
    push di
                       
    lea di, Temp_str 
    dec di     
    xor cx, cx       
    mov cl, [Size_temp_str]
    Point:                                              ;Этот цикл проверяет наличие пробелов в строке
        inc di            
        push bx 
        mov bl, [di]
        cmp bl,' '        
        pop bx      
        je error_exit 
    loop Point 
                                          
    pop cx                    
    pop di                                
ENDM

.code                          
start:                         
    mov ax, @data              
    mov ds, ax                                               
                 
    call enterString                                    ;Ввод основной строки
    call enterSubstring1                                ;Ввод заменяемого слова
    exitIfNotSingleWord SubString1, Substr1, Point1     ;Проверка на правильность ввода заменяемого слова
    call enterSubstring2                                ;Ввод замещающего слова 
    exitIfNotSingleWord SubString2, Substr2, Point2     ;Проверка на правильность ввода замещающего слова

    xor cx, cx                                          ;обнуление cx
    lea si, String                                      
    dec si                     
    jmp skip_spaces                                     ;цикл пропуска пробелов
    
    find:                                               ;Ищем начало следующего слова
        inc si                 
        push bx 
        mov bl, [si]               
        cmp bl, ' '                                     ;Сравниваем с ' '
        pop bx
        je skip_spaces                                  ;Если ' ' - начать новый поиск       
       
        push bx 
        mov bl, [si]
        cmp bl, '$'                                     ;Сравниваем с концом строки    
        pop bx
        je exit                                         ;Если конец строки - выходим 
        
        jmp find                                        ;если текущий элемент символ - пропускаем
        
        skip_spaces:                                    ;Пропуск пробелов
            inc si   
            push bx 
            mov bl, [si]
            cmp bl, ' '
            pop bx
           je skip_spaces      
           
        lea di, SubString1                              ;Указатель на начало заменяемого слова
        call searchSubString1                           ;Процедура поиска слова в строке 
        jmp find                                        ;Ищем дальше
        
    error_exit:
        call outputErrorResult

    exit:  
        call outputResult                                  
     
enterString proc                                        
    lea dx, msgString                                   
    call outputString                                   
    lea dx, mainStrLength                                
    mov mainStrLength[0], max_length
    call inputString                                    
    ret                                                 
enterString endp                                        

enterSubstring1 proc            
    lea dx, msgSubstring1                              
    call outputString          
    lea dx, SubStrLength1                               
    mov SubStrLength1[0], max_length
    call inputString           
    ret                        
enterSubstring1 endp            

enterSubstring2 proc            
    lea dx, msgSubstring2                               
    call outputString          
    lea dx, SubStrLength2                                
    mov SubStrLength2[0], max_length
    call inputString           
    ret                        
enterSubstring2 endp           

outputString proc              
    push ax                    
    mov ah, 09h               
    int 21h                    
    pop ax                     
    ret                        
outputString endp              

inputString proc               
    push ax                     
    mov ah, 0Ah               
    int 21h                   
    pop ax                     
    ret                      
inputString endp               

outputErrorResult proc              
    lea dx, msgError           
    call outputString                    
    mov ax, 4ch                
    int 21h  
    ret                  
outputErrorResult endp   

outputResult proc              
    lea dx, msgResult         
    call outputString          
    lea dx, String            
    call outputString          
           
    lea dx, enterStr
    call outputString       
           
    xor ax, ax    
    mov ah,4ch               
    int 21h  
    ret                  
outputResult endp   
 
searchSubString1 proc             
    push ax                      
    push cx                      
    push di                      
    push si                      
                                 
    xor cx, cx                   
    mov cl, [SubStr1]         
    comparestr:                  
        mov ah,[si]              
        dec cx                  
        cmp ah,[di]              
        je  compare              
        jne NotEqual            
        compare:                
            inc si              
            inc di             
            cmp cx,0            
            je check             
            jne comparestr                                      
        check:      
            push bx 
            mov bl, [si]                
            cmp bl, ' '          
            pop bx
            je Equal
            push bx 
            mov bl, [si]                
            cmp bl, 0dh          
            pop bx           
            je Equal 
            jne NotEqual                                         
        Equal:
            push bx
            mov bl, SubStr1
            cmp bl, SubStr2
            pop bx        
            je swapStr1
            ja swapStr2
            jna swapStr3
        a1:      
            call searchSubString1                           
        NotEqual:                       
            pop si               
            pop di               
            pop cx               
            pop ax               
            ret                  
        swapStr1:
            call swapStr1p
            jmp a1
        swapStr2:
            call swapStr2p
            jmp a1
        swapStr3:
            call swapStr3p
            jmp find                                       
searchSubString1 endp             

swapStr1p proc
    push si
    push di
    push cx
    push ax
      
    mov ax, si
    sub al, [SubStr1]
    mov si, ax 
    lea di, SubString2
    xor cx, cx
    mov cl, [SubStr1]  
    
    cycle1:
        mov al, [di]
        mov [si], al
        inc si
        inc di
    loop cycle1
           
    pop ax       
    pop cx
    pop di
    pop si
    ret
swapStr1p endp

swapStr2p proc
    push si
    push di
    push cx
    push ax
      
    mov ax, si
    sub al, [SubStr1]
    mov si, ax 
    lea di, SubString2
    xor cx, cx
    mov cl, [SubStr2]  
    
    cycle2:
        mov al, [di]
        mov [si], al
        inc si
        inc di
    loop cycle2
    
    xor bx, bx
    mov bl, [SubStr1]
    sub bl, [SubStr2]
    add si, bx
    
    
    lea ax, String
    push bx
    mov bl, [Strl]             
    add ax, bx
    pop bx             
    sub ax,si                 
    mov cx,ax                   
    inc cx                      
    
    shift_left:                  
        mov ah,[si]             
        sub si, bx              
        mov [si], ah            
        add si, bx             
        inc si                  
    loop shift_left  
          
    pop ax       
    pop cx
    pop di
    pop si
    ret
swapStr2p endp

swapStr3p proc
    push si
    push di
    push cx
    push ax
    push dx
    
    xor bx, bx
    xor dx, dx
    mov bl, [SubStr2]
    sub bl, [SubStr1]
    mov dx, bx
    
    mov bl, [SubStr2]
    sub bl, [SubStr1]
    add [Strl], bl 
     
    lea bx, String
    push ax
    mov ax, 0
    mov al, [Strl]
    add bx, ax
    pop ax
    mov di, bx
    sub bx, si
    mov cx, bx
    inc cx 
      
    push bx
    lea bx, String
    add bx, 00c5h
    push si      
    shift_right:                  
        mov ah,[di]
        mov si, di              
        add di, dx
        cmp di, bx
        jae end_str       
    cont:              
        mov [di], ah              
        mov di, si               
        dec di                   
    loop shift_right
    pop si
    pop bx
       
    mov ax, si
    sub al, [SubStr1]
    mov si, ax 
    lea di, SubString2
    xor cx, cx
    mov cl, [SubStr2]  

    cycle3:
        mov al, [di]
        mov [si], al
        inc si
        inc di
    loop cycle3
    
    mov String[199], '$'
    
    pop dx      
    pop ax       
    pop cx
    pop di
    pop si
    ret
swapStr3p endp

end_str:
    lea di, String
    add di, 00c5h
    jmp cont 

end start                       