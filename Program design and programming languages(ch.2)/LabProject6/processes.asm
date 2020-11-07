.model small
.stack 100h

.data    
str1 db 10, 13, "Program 5", 10, 13, '$'
 
.code
 
print macro str     
    pusha
    mov ah, 09h
    lea dx, str
    int 21h 
    popa
print endm

main:
mov	ax, @data                      
mov	ds, ax 
print str1  
.exit
end main