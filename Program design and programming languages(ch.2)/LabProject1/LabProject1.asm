DSEG SEGMENT
        BUF DB "0123406789"
DSEG ENDS

CSEG SEGMENT
        ASSUME CS:CSEG, DS:DSEG, SS:SSEG
START:
        MOV BX,OFFSET BUF    
        MOV CX,10            
        MOV SI,0
L1:
        MOV DL,[BX+SI]       
        MOV AH,2;
        INT 21H              
        INC SI
        CMP CX, 0
        JE BREAK               
        LOOP L1
BREAK:              
CSEG ENDS
END START