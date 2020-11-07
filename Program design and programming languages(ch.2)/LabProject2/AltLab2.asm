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
    pop di                    
    pop cx                                
ENDM

.code                          
start:                         
    mov ax, @data              
    mov ds, ax
    mov es, ax                                               
                 
    call enterString                                    ;Ввод основной строки
    mov bl, Strl 
    mov String[bx], '$'
       
    call enterSubstring1                                ;Ввод заменяемого слова
    ;exitIfNotSingleWord SubString1, Substr1, Point1     ;Проверка на правильность ввода заменяемого слова
    mov bl, SubStr1 
    mov SubString1[bx], '$'
    
    call enterSubstring2                                ;Ввод замещающего слова 
    ;exitIfNotSingleWord SubString2, Substr2, Point2     ;Проверка на правильность ввода замещающего слова
    mov bl, SubStr2 
    mov SubString2[bx], '$'
      
    lea di, String                                      
    dec di                     
    jmp skip_spaces                                     ;цикл пропуска пробелов    
    find:                                               ;Ищем начало следующего слова
        inc di                  
        mov bl, [di]               
        cmp bl, ' '                                     ;Сравниваем с ' '
        je skip_spaces                                  ;Если ' ' - начать новый поиск              
        mov bl, [di]
        cmp bl, '$'                                     ;Сравниваем с концом строки    
        je exit                                         ;Если конец строки - выходим         
        jmp find                                        ;если текущий элемент символ - пропускаем     
        skip_spaces:                                    ;Пропуск пробелов
            inc di   
            mov bl, [di]
            cmp bl, ' '
            je skip_spaces           
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
    lea si, SubString1
    xor cx, cx
    mov cl, SubStr1
    repe cmpsb
    jne temp
    mov bl, [di]
    cmp bl, ' '
    jne checkEnd
    continue:
    mov bl, SubStr1
    cmp bl, SubStr2        
    je swapStr1
    ja swapStr2
    jna swapStr3
    swapStr1:
        call swapStr1p
        jmp skip_spaces
    swapStr2:
        call swapStr2p
        jmp skip_spaces
    swapStr3:
        call swapStr3p
        jmp skip_spaces
    checkEnd:
        cmp bl, [si]
        jne temp
        je continue
    temp:
        jmp find                              
searchSubString1 endp      
             
swapStr1p proc
    mov ax, di
    sub al, [SubStr1]
    mov di, ax 
    lea si, SubString2       
    xor cx, cx
    mov cl, [SubStr1]
    rep movsb  
    ret
swapStr1p endp

swapStr2p proc      
    mov ax, di
    sub al, [SubStr1]
    mov di, ax 
    lea si, SubString2       
    xor cx, cx
    mov cl, [SubStr2]
    rep movsb     
    push di 
    xor bx, bx
    mov bl, [SubStr1]
    sub bl, [SubStr2]    
    add di, bx        
    lea ax, String
    push bx
    mov bl, [Strl]             
    add ax, bx
    pop bx             
    sub ax,di                 
    mov cx,ax                   
    inc cx                          
    shift_left:                  
        mov ah,[di]             
        sub di, bx              
        mov [di], ah            
        add di, bx             
        inc di                  
    loop shift_left     
    sub di, bx
    dec di
    mov al, '$'
    xor cx, cx
    add cx, bx
    rep stosb
    sub [Strl], bl
    pop di              
    ret
swapStr2p endp

swapStr3p proc        
    xor dx, dx
    mov dl, [SubStr2]
    sub dl, [SubStr1]
                         
    lea bx, String
    xor ax, ax
    mov al, [Strl]
    add bx, ax
    mov si, bx
    sub bx, di
    mov cx, bx
    inc cx
        
    add [Strl], dl
           
    lea bx, String
    add bx, max_length
    dec bx
          
    shift_right:                  
        mov ah,[si]
        push si              
        add si, dx
        cmp si, bx
        jae end_str       
    cont:              
        mov [si], ah              
        pop si               
        dec si                   
    loop shift_right
    
    mov String[199], '$'
        
    mov ax, di
    sub al, [SubStr1]
    mov di, ax 
    lea si, SubString2       
    xor cx, cx
    mov cl, [SubStr2]
    rep movsb
                     
    ret
swapStr3p endp

end_str:
    lea si, String
    add si, max_length
    dec si
    jmp cont 
end start              