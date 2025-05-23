bits 16
org 0x7C00

start:
    ; Configuração básica
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00

    ; Modo de vídeo texto 80x25
    mov ax, 0x0003
    int 0x10

    ; Carrega o kernel do disco
    mov ah, 0x02
    mov al, 4       ; Número de setores
    mov ch, 0       ; Cilindro
    mov cl, 2       ; Setor
    mov dh, 0       ; Cabeçote
    mov bx, 0x7E00  ; Carregar após o bootloader
    int 0x13

    ; Chama a função kernel_main em C
    extern kernel_main
    call kernel_main

    ; Se retornar (não deveria), fica em loop
    cli
    hlt

times 510-($-$$) db 0
dw 0xAA55
