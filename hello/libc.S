#    SPDX-FileCopyrightText: 2025 TeOSobrino <teosobrinogit@gmail.com>
#
#    SPDX-License-Identifier: GPL-3.0-or-later
#
#  This file is a derivative of SYSeg (https://gitlab.com/monaco/syseg)
#  and includes modifications made by the following author(s):
#  TeOSobrino <teosobrinogit@gmail.com>

	/* Suppose we receive the argument in %cx. */

	.code16gcc
	.global printf
    .global get_time
	.section .text

printf:

	mov   %cx, %bx

        mov   $0x0e,%ah
        mov   $0x0, %si
loop:
        mov   (%bx, %si), %al
        cmp   $0x0, %al
        je    end
        int   $0x10
        add   $0x1, %si
        jmp   loop
end:
        mov %si, %ax
        ret


get_time:

        mov   $0x02,%ah #determina leitura do RTC
        int   $0x1A     #interrupção para serviços do RTC
        #%ch = horas; %cl = minutos;
        movb  %ch, %ah #horas no byte alto do ax
        movb  %cl, %al #minutos no byte baixo do ax
        ret            #retornamos em %ax [0xHHMM]

