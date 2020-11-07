.model tiny
.code
org 100h
start:
	mov ah, 06h
	mov al, 0
	xor bh, bh
	xor cx, cx
	mov dh, 23
	mov dl, 78
	int 10h
	in al, 61h
	or al, 1
	out 61h, al
	mov ax, 3509h
	int 21h
	mov word ptr old_keyboard_handler + 2, es
	mov word ptr old_keyboard_handler, bx
	lea dx, keyboard_handler
	mov ah, 25h
	int 21h
	mov ax, 0B800h
	mov es, ax
	main_loop:
		call drawRoad
		call drawBarriers
		call drawCar
		call checkLoss
		call move
		call drawScore
		cmp isLoss, 1
		je car_broken
	jmp main_loop	
	car_broken:	
	mov ax, 2509h
	mov ds, word ptr old_keyboard_handler + 2
	mov dx, word ptr cs:old_keyboard_handler
	int 21h		
	mov ax, 4C00h
	int 21h
	


drawCar proc
	push bx
	cmp moving_l, 1
	jne not_moving_l
		dec carPosition
	not_moving_l:	
	cmp moving_r, 1
	jne not_moving_r
		inc carPosition
	not_moving_r:
	cmp carPosition, 0
	jge draw_car_l_bound
		mov carPosition, 0
	draw_car_l_bound:
	cmp carPosition, 80 - carWidth
	jle draw_car_r_bound
		mov carPosition, 80 - carWidth
	draw_car_r_bound:
	mov bx, 80 * carRow
	add bx, carPosition
	shl bx, 1	
	mov es:[bx], 0F4FEh
	mov es:[bx] + 2, 07FFEh
	mov es:[bx] + 4, 0F3FEh	
	pop bx
	ret
drawCar endp

move proc
	push ax
	push bx
	push cx
	push dx
	mov ah, 86h
	mov dx, 0A2C2h
	mov cx, 0
	int 15h
	mov ax, 00FEh
	mov bx, 80 * carRow
	add bx, carPosition
	shl bx, 1
	mov es:[bx], ax
	mov es:[bx] + 2, ax
	mov es:[bx] + 4, ax
	mov ah, 07h
	mov al, 1
	xor bh, bh
	xor cx, cx
	mov dh, 24
	mov dl, 79
	int 10h
	pop dx
	pop cx
	pop bx
	pop ax
	ret
move endp

keyboard_handler proc
	push ax	
	A_KEY EQU 1Eh
	D_KEY EQU 20h	
	in al, 60h	
	cmp al, A_KEY
	jne not_a
		mov moving_l, 1
		jmp keyboard_handler_end
	not_a:	
	cmp al, D_KEY
	jne not_d
		mov moving_r, 1
		jmp keyboard_handler_end
	not_d:	
	cmp al, A_KEY + 80h
	jne not_a_released
		mov moving_l, 0
		jmp keyboard_handler_end
	not_a_released:	
	cmp al, D_KEY + 80h
	jne not_d_released
		mov moving_r, 0
		jmp keyboard_handler_end
	not_d_released:	
	keyboard_handler_end:
	mov al, 20h
	out 20h, al
	pop ax
	iret
keyboard_handler endp

checkLoss proc
	push ax
	push bx	
	mov bx, 80 * (carRow - 1)
	add bx, carPosition
	shl bx, 1	
	mov ax, es:[bx]
	cmp ax, block
	je check_loss_ouch	
	mov ax, es:[bx] + 4
	cmp ax, block
	jne check_loss_end	
	check_loss_ouch:
		mov isLoss, 1	
	check_loss_end:
	pop bx
	pop ax
	ret
checkLoss endp

drawBarriers proc
	push ax
	push bx
	push cx
	xor bx, bx
	mov bl, roadPosition
	add bx, 2
	shl bx, 1
	mov cx, roadWidth - 3
	drawBarriersLoop:
		call rand
		and ax, 01111111b
		jnz noDrawBarriers
			mov es:[bx], block
			mov es:[bx] + 2, block
		noDrawBarriers:
		add bx, 4
	loop drawBarriersLoop
	pop cx
	pop bx
	pop ax
	ret
drawBarriers endp

drawScore proc
	push ax
	push bx
	push cx
	push di
	
	mov bx, 80 * (carRow + 1) * 2
	mov cx, 80
	draw_score_loop_0:
		cmp word ptr es:[bx], block
		jne no_cost
			inc score
		no_cost:
		add bx, 2
	loop draw_score_loop_0
	mov ax, score
	mov bx, 10
	mov cx, 5
	mov di, (80 * 25 - 1) * 2
	draw_score_loop_1:
		xor dx, dx
		div bx
		add dl, '0'                                                                    
		mov dh, 074h
		mov es:[di], dx
		sub di, 2
	loop draw_score_loop_1
	lea bx, score_msg
	mov ah, 074h
	draw_score_loop_2:
	cmp BYTE PTR [bx], 0
	je draw_score_end
		mov al, [bx]
		mov es:[di], ax
		sub di, 2
		dec bx
	jmp draw_score_loop_2
	draw_score_end:
	pop di
	pop cx
	pop bx
	pop ax
	ret
drawScore endp

block equ 07DBh                
roadPosition db	24
roadWidth EQU 32
carPosition	dw 40
carWidth EQU 3
carRow	EQU	20
old_keyboard_handler dd	(?)
moving_l db	0
moving_r db	0
isLoss db	0
score dw 0
db 0, "Score: "
score_msg EQU $ - 1

end start