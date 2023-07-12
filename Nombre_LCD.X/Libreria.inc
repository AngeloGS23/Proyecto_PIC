CBLOCK 0x20
GUARDAR_DATO
COMANDO
DATA_GUARDAR
DATAS
CUENTA
ENDC

#define RS  PORTC,0
#define E   PORTC,1
#define RW  PORTC,2

LCD_INIT
    BSF	    STATUS,RP0
    CLRF    TRISD ; Configurar puerto D como salida
    CLRF    TRISC ; Configurar puerto C como salida
    BCF	    STATUS,RP0
    CALL    Retardo_1ms
    BCF	    RS ; Modo Comando
    MOVLW   0x30 ; RD4 RD5 RD6 RD7 -> RD3 AL RD0
    CALL    LCD_CONFIG
    CALL    Retardo_1ms
    MOVLW   0x30
    CALL    LCD_CONFIG
    CALL    Retardo_1ms
    MOVLW   0x30
    CALL    LCD_CONFIG
    CALL    Retardo_1ms
    MOVLW   0x20 ; 4 bits
    CALL    LCD_CONFIG
    CALL    LCD_2LINEAS ; 0x28
    CALL    LCD_DISPLAY ; 0x0C
    CALL    LCD_BORRAR ; 0x01
    CALL    LCD_INCR ; 0x06
    RETURN

LCD_LINEA2
    MOVLW   0xC0 ; Dirección de la segunda línea
    CALL    LCD_COMANDO
    RETURN

LCD_LINEA1
    MOVLW   0x80 ; Dirección de la primera línea
    CALL    LCD_COMANDO
    RETURN

LCD_DISPLAY
    MOVLW   0x0C ; Encender display sin cursor ni parpadeo
    CALL    LCD_COMANDO
    RETURN

LCD_BORRAR
    MOVLW   0x01 ; Borrar pantalla
    CALL    LCD_COMANDO
    RETURN

LCD_INCR
    MOVLW   0x06 ; Incrementar posición del cursor
    CALL    LCD_COMANDO
    RETURN

LCD_2LINEAS
    MOVLW   0x28 ; Configurar display de 2 líneas
    CALL    LCD_COMANDO
    RETURN

LCD_COMANDO
    MOVWF   COMANDO ; Guardar comando
    MOVF    COMANDO,W
    CALL    LCD_CONFIG ; Configurar puerto para enviar comando
    SWAPF   COMANDO,F ; Intercambiar nibbles
    MOVF    COMANDO,W
    CALL    LCD_CONFIG ; Configurar puerto para enviar comando
    RETURN

LCD_CARACTER
    MOVWF   DATA_GUARDAR ; Guardar dato/caracter
    BSF	    RS ; Modo Caracter
    CALL    LCD_DATOS ; Enviar dato
    SWAPF   DATA_GUARDAR,F ; Intercambiar nibbles
    MOVF    DATA_GUARDAR,W
    CALL    LCD_DATOS ; Enviar dato
    RETURN

LCD_DATOS
    ANDLW   0xF0 ; Máscara para obtener los 4 bits más significativos
    MOVWF   DATAS ; Guardar los 4 bits más significativos
    BSF	    RS ; Modo Caracter
    MOVF    PORTD,W
    ANDLW   0x0F ; Máscara para obtener los 4 bits menos significativos
    IORWF   DATAS ; Combinar con los 4 bits más significativos
    MOVF    DATAS,W
    MOVWF   PORTD ; Enviar dato al puerto D
    CALL    ENABLE ; Generar pulso en pin E
    RETURN

LCD_CONFIG
    ANDLW   0xF0 ; Máscara para obtener los 4 bits más significativos
    MOVWF   GUARDAR_DATO ; Guardar los 4 bits más significativos
    BCF	    RS ; Modo Comando
    MOVF    PORTD,W
    ANDLW   0x0F ; Máscara para obtener los 4 bits menos significativos
    IORWF   GUARDAR_DATO ; Combinar con los 4 bits más significativos
    MOVF    GUARDAR_DATO,W
    MOVWF   PORTD ; Enviar dato al puerto D
    CALL    ENABLE ; Generar pulso en pin E
    RETURN

ENABLE
    CALL    Retardo_1ms
    CALL    Retardo_1ms
    BSF	    E ; Pulso en pin E
    CALL    Retardo_1ms
    BCF	    E ; Fin de pulso en pin E
    RETURN

Retardo_1ms
    MOVLW   .250 ; Retardo de 1 ms
    MOVWF   CUENTA ; Inicializar contador
BUCLE
    NOP	    ; No operation (sin operación)
    DECFSZ  CUENTA,F ; Decrementar contador y saltar si es cero
    GOTO    BUCLE ; Repetir bucle
    MOVF    GUARDAR_DATO,W ; Recuperar valor guardado
    RETURN
