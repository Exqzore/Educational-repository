.model small
.stack 100h
.data
    isLoss db 0 
    roadPosition db	15
    gameScreenSize db 59
    blockFrame equ 06DBh
    blockPanel equ 07DBh
    blockBarrier equ 04DBh
    block equ 072DBh               
    carPosition dw 3420
    carSize equ 3
    temp dw 7100h
    delayTime dw 0FFFFh   
    leftBorderPosition db 15
    rightBorderPosition db 15    
    car dw 01DBh, 01DBh, 01DBh
    voidStr dw 00DBh, 00DBh, 00DBh  
    scoreMessage db "Score:"
    speedMessage db "Speed:"
    score dw 0
    speed dw 1
    flag dw 0
    gameOverMessage db "Game Over"
              
.code
begin:
    mov ax, @data
    mov ds, ax  
    mov ax, 0003
    int 10h
    in al, 61h
	or al, 1
	out 61h, al 
    mov ax, 0B800h
    mov es, ax
    mov ax, 0305h
    mov bx, 0000h
    int 16h
    call drawInterface 
    mainLoop:
        call nextFrameDelay                       
        call moveScreen
        call keyHundler
        call drawCar
        call drawRoad
        call drawBarriers
        call showScore
        call showSpeed
        call checkLoss
        cmp isLoss, 1
		je end 
    jmp mainLoop    
    end:
        call endOfTheGame 	

keyHundler proc
    push ax
    push bx
    mov ax, 0100h
    int 16h
    jz noBuffer
    mov ax, 0000h
    int 16h
    cmp al, '+'
    jne minusCheck
    inc speed
    cmp speed, 4
    jb skip
    mov speed, 3
    jmp skip
    minusCheck:
    cmp al, '-'
    jne skip
    dec speed
    cmp speed, 0
    ja skip
    mov speed, 1 
    skip:
    cmp al, 'a'
    jne notLeft
    mov bx, carPosition
    cmp bx, 3362
    je noBuffer
    sub carPosition, 2
    jmp noBuffer
    notLeft:
    cmp al, 'd'
    jne noBuffer
    mov bx, carPosition
    cmp bx, 3474
    je noBuffer
    add carPosition, 2
    noBuffer:
    pop bx
    pop ax           
    ret               
keyHundler endp

drawInterface proc
    push bx
    push cx
    push ax
        mov cx, 80 
    xor bx, bx
    loop_1:
        mov es:[bx], blockFrame
        add bx, 2    
    loop loop_1
    mov cx, 80
    mov bx, 80 * 48
    loop_2:
        mov es:[bx], blockFrame
        add bx, 2    
    loop loop_2
    mov cx, 24
    mov bx, 0
    loop_3:
        mov es:[bx], blockFrame
        add bx, 80 * 2
    loop loop_3
    mov cx, 24
    mov bx, 79 * 2
    loop_4:
        mov es:[bx], blockFrame
        add bx, 80 * 2
    loop loop_4
    mov cx, 24
    xor bx, bx
    mov bl, gameScreenSize
    inc bx
    shl bx, 1
    loop_5:
        mov es:[bx], blockFrame
        add bx, 80 * 2
    loop loop_5
    mov cx, 23
    mov bx, 80 * 2
    add bx, 61 * 2
    loop_6:
        push cx
        mov cx, 18
        loop_7:
            mov es:[bx], blockPanel
            add bx, 2
        loop loop_7
        add bx, 62 * 2
        pop cx
    loop loop_6
    push es
    mov ax, @data
    mov es, ax
    mov ax, 1300h
    mov bx, 0
    mov bl, 71h
    mov cx, 6
    mov dx, 023Eh
    lea bp, scoreMessage
    int 10h
    mov cx, 6
    mov dx, 043Eh
    lea bp, speedMessage
    int 10h
    pop es
    pop ax
    pop cx 
    pop bx
    ret  
drawInterface endp

drawCar proc   
    mov di, carPosition
    mov cx, carSize
    mov si, offset car
    rep movsw
    ret    
drawCar endp

moveScreen proc
    mov di, carPosition
    mov si, offset voidStr
    mov cx, carSize
    rep movsw
    mov ah, 07h
    mov al, 1
    xor bh, bh
    mov ch, 1
    mov cl, 1
    mov dh, 23
    mov dl, 59
    int 10h
    ret    
moveScreen endp

showSpeed proc
    mov bx, 4 * 160 + 69 * 2
    mov ax, speed
    add ax, '0'
    add ax, temp
    mov es:[bx], ax
    add bx, 2
    mov es:[bx], 7178h  
    ret    
showSpeed endp

showScore proc
    inc score
    mov ax, score
    mov cx, 5
    mov di, 2 * 160 + 73 * 2
    showScoreLoop:
        mov bx, 10
        xor dx, dx
        div bx    
        add dl, '0'
        add temp, dx        
        mov si, offset temp
        movsw
        mov temp, 7100h
        sub di, 4
    loop showScoreLoop  
    ret
showScore endp

checkLoss proc
	push ax
	push bx	
	mov bx, carPosition
	sub bx, 160	
	mov ax, es:[bx]
	cmp ax, block
	je isLosed
	cmp ax, blockBarrier
	je isLosed	
	mov ax, es:[bx] + 4
	cmp ax, block
	je isLosed
	cmp ax, blockBarrier
	jne checkLossEnd	
	isLosed:
		mov isLoss, 1	
	checkLossEnd:
	pop bx
	pop ax
	ret
checkLoss endp     

nextFrameDelay proc
    mov bx, delayTime
    cmp speed, 2
    je boost1
    ja boost2
    continue:
    mov ax, 8600h
	mov dx, bx 
	mov cx, 0
	int 15h
	jmp endk
	boost1:
	    mov bx, 7FFFh
	jmp continue
	boost2:
	    mov bx, 5555h
	jmp continue
	endk:
	ret   
nextFrameDelay endp   

randomGeneratorAX proc
    mov ah, 2ch
    int 21h
	ret
randomGeneratorAX endp

drawRoad proc
	push ax
	push bx
	call randomGeneratorAX
	xor ax, ax
	mov al, dl
	mov bl, 3
	div bl
	sub ah, 1
	add roadPosition, ah
	cmp roadPosition, 6
	jge checkLeftBorder
		mov roadPosition, 6
	checkLeftBorder:
	cmp roadPosition, 31
	jl checkRightBorder
		mov roadPosition, 30
	checkRightBorder:
	mov ax, block
	mov di, 162
    xor cx, cx
	mov cl, roadPosition
	dec cl
	drawLeftBorderOfTheRoadLoop:
	    mov es:[di], ax
	    add di, 2
	loop drawLeftBorderOfTheRoadLoop
	mov cl, 60
	sub cl, roadPosition
	sub cl, 31
	add di, 62
	drawRightBorderOfTheRoadLoop:
	    mov es:[di], ax
	    add di, 2
	loop drawRightBorderOfTheRoadLoop
	pop bx
	pop ax
	ret
drawRoad endp

drawBarriers proc
	push ax
	push bx
	push cx
	push dx
	xor bx, bx
	mov bl, roadPosition	 
	add bx, 80
	shl bx, 1
	call randomGeneratorAX
	xor ax, ax
	add al, dl
    add al, dl
    cmp flag, 3
    jne changeFlag
    push bx
    xor dx, dx
    mov bx, 50
    div bx
    pop bx
    add bx, dx
	mov es:[bx], blockBarrier
	mov es:[bx] + 2, blockBarrier
	mov flag, 0
	jmp noDrawBarriers
	changeFlag:
	add flag, 1
	noDrawBarriers:
	pop dx
	pop cx
	pop bx
	pop ax
	ret
drawBarriers endp

endOfTheGame proc
    mov ax, @data
    mov es, ax
    mov ax, 1300h
    mov bx, 0
    mov bl, 71h
    mov cx, 9
    mov dx, 0B1Ah
    lea bp, gameOverMessage
    int 10h
    mov ah, 4Ch
    int 21h
    ret
endOfTheGame endp

end begin