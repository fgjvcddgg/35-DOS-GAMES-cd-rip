

	LOCALS			;; Enable local labels

        IDEAL                   ;; Use Turbo Assembler's IDEAL mode
	JUMPS

	; Driver load and unload calls.  Requires that the application provide
	; memory allocation functions and access to DOSCALLS.OBJ.

SMALL_MODEL	equ	0   ;: True if declaring C procedures as near.
			; It is false here because all procedures are
			; far, so that you can link any memory model
			; to theme. (They are prototyped as well.)


        INCLUDE "PROLOGUE.MAC"          ;; common prologue


SEGMENT  _TEXT BYTE PUBLIC 'CODE'               ;; Set up _TEXT segment
        ENDS

	ASSUME	CS: _TEXT, DS: _TEXT, SS: NOTHING, ES: NOTHING


SEGMENT _TEXT

Macro	CPROC	name		; Macro to establish a C callable procedure.
	public	_&name
IF	SMALL_MODEL
Proc	_&name	near
ELSE
Proc	_&name	far
ENDIF
	endm


CPROC	VidOn
	mov	ax,13h
	int	10h
	ret
	endp

CPROC	VidOff
	mov	ax,03
	int	10h
	ret
	endp

CPROC	DrawSound
	ARG	SOUND:DWORD,SWID:WORD,COLOR:WORD
	PENTER	0
	PushCREGS

	mov	ax,0A000h
	mov	es,ax		; VGA screen ram segment.
	lds	si,[SOUND]
	mov	bx,320
	xor	cx,cx		; Xlocation.

@@GO:   mov     al,[ds:si]      ; Get sound value.
	xor	ah,ah		; Zero high byte.
	shr	ax,1
	add	ax,36		; Adjust.
	mul	bx
	add	ax,cx		; Plus current xlocation.
	mov	di,ax
	mov	ax,[COLOR]
	mov	[byte es:di],al
	inc	cx
	inc	si
	cmp	cx,[SWID]
	jne	@@GO

	PopCREGS
	PLEAVE
	ret
	endp


	ends
	end
