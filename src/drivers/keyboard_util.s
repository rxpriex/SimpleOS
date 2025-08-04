# src\drivers\keyboard.s
# basic keyboard utilities written in assembly

.code32
.text

.global read_key

read_key:
    #prepeare stack and registers for execution
    push %ebp
    mov %esp, %ebp
    push %esi

    mov 8(%ebp), %esi

    movb $0x1, 1(%esi)

Idle:

    #check the port for new data
    inb $0x64, %al
    test $0x01, %al
    jz Idle

    #read from port
    inb $0x60, %al

    #check keystate
    test $0x80, %al
    jz write_value

    and $0x7F, %al
    movb $0x0, 1(%esi)
    jmp write_value

write_value:

    movb %al, 0(%esi)

    pop %esi
    pop %ebp

    ret
    