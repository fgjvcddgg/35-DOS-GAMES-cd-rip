
	LOCALS			;; Enable local labels

        IDEAL                   ;; Use Turbo Assembler's IDEAL mode
	JUMPS

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

CPROC	Left8
	ARG	SOURCE:DWORD,DEST:DWORD,SLEN:WORD
	PENTER	0
	PushCREGS

	lds	si,[SOURCE]
	les	di,[DEST]
	mov	cx,[SLEN]
	xor	ax,ax
@@GO:	movsb
	stosb
	loop	@@GO

	PopCREGS
	PLEAVE
	ret
	endp

CPROC	Right8
	ARG	SOURCE:DWORD,DEST:DWORD,SLEN:WORD
	PENTER	0
	PushCREGS

	lds	si,[SOURCE]
	les	di,[DEST]
	mov	cx,[SLEN]
	xor	ax,ax
@@GO:	stosb
	movsb
	loop	@@GO

	PopCREGS
	PLEAVE
	ret
	endp

CPROC	Mono16
	ARG	SOURCE:DWORD,DEST:DWORD,SLEN:WORD
	PENTER	0
	PushCREGS

	lds	si,[SOURCE]
	les	di,[DEST]
	mov	cx,[SLEN]
@@GO:	lodsb
	mov	ah,al
	xor	al,al
	sub	ax,32767
	stosw
	loop	@@GO

	PopCREGS
	PLEAVE
	ret
	endp

CPROC	Left16
	ARG	SOURCE:DWORD,DEST:DWORD,SLEN:WORD
	PENTER	0
	PushCREGS

	lds	si,[SOURCE]
	les	di,[DEST]
	mov	cx,[SLEN]

@@GO:	lodsb
	mov	ah,al
	xor	al,al
	sub	ax,32767
	stosw
	mov	ax,8000h
	stosw
	loop	@@GO

	PopCREGS
	PLEAVE
	ret
	endp

CPROC	Right16
	ARG	SOURCE:DWORD,DEST:DWORD,SLEN:WORD
	PENTER	0
	PushCREGS

	lds	si,[SOURCE]
	les	di,[DEST]
	mov	cx,[SLEN]

@@GO:	mov	ax,8000h
	stosw
	lodsb
	mov	ah,al
	xor	al,al
	sub	ax,32767
	stosw
	loop	@@GO

	PopCREGS
	PLEAVE
	ret
	endp

	ends
	end
