#************************************************************************
#                            HYPER TERMINAL
#************************************************************************
# -*- coding: utf-8 -*-

'''
Utilizando Raspberry Pi 3 Model B
Pacote pyserial
Fontes:
http://elinux.org/RPi_Serial_Connection
https://diyprojects.io/python-code-read-serial-port-raspberry-pi/#.WWu9h1FJnIU
https://diyprojects.io/raspberry-pi-useful-commands/
https://www.raspberrypi.org/forums/viewtopic.php?f=63&t=151454
I
'''
#http://pyserial.readthedocs.io/en/latest/shortintro.html
#http://pyserial.readthedocs.io/en/latest/pyserial_api.html
import serial

#CONFIGURAÇÃO
def port_config(com, baud):   #Exemplo: port_config(serial0, 9600)
    global ser
    ser = serial.Serial('/dev/'+com)  #Abre a porta e configura
    ser.baud = baud
    ser.parity = serial.PARITY_NONE
    ser.stopbits = serial.STOPBITS_ONE
    ser.bytesize = serial.EIGHTBITS
    ser.timeout = 1
    print('Aberta porta:'+ser.name)  # check which port was really used

print('TERMINAL SERIAL PARA RASPBERRY PI 3')
print('Atualizado: 19.07.17')
porta=input('Digite a porta que deseja abrir:')
vel=input('Velocidade de comunicação[baud]:')
port_config(porta, vel)
sair = 'n'
while(sair!='s'):    
    print('Escolha a opção desejada:')
    print('    1: Enviar um dado.')
    print('    2: Receber um dado.')
    opcao=input('Opção:')
    if (opcao=='1'):
        print('Escolha a opção desejada:')
        print('    1: Enviar uma string ou char ASCII.')
        print('    2: Enviar um valor em decimal.')
        opcao2=input('Opção:')
        if (opcao2=='1'):
            caractere=input('Digite o caractere:')
            caractere=str.encode(caractere)
            ser.write(caractere)        #Envia uma string pela porta serial
        elif(opcao2=='2'):
            dec_num=int(input('Digite o valor em decimal:'))
            dado = bytes([dec_num])
            ser.write(dado)
    elif(opcao=='2'):
        input('Aperte qualquer tecla quando o byte for transmitido.')
        line = ser.read()          #Lê 1 byte
        #line2=line.decode()       #Valido para utf-8
        line2=chr(int.from_bytes(line, byteorder='big'))
        print('O caractere recebido foi:'+str(line2))
        print('Correspondente ao decimal:'+str(int.from_bytes(line, byteorder='big')))
    sair=input('Deseja sair do programa?[s/n]')
ser.close()                  #Fecha a porta


'''
Função serial.Serial()
Parameters:
    port – Device name or None.
    baudrate (int) – Baud rate such as 9600 or 115200 etc.
    bytesize – Number of data bits. Possible values: FIVEBITS, SIXBITS, SEVENBITS, EIGHTBITS
    parity – Enable parity checking. Possible values: PARITY_NONE, PARITY_EVEN, PARITY_ODD PARITY_MARK, PARITY_SPACE
    stopbits – Number of stop bits. Possible values: STOPBITS_ONE, STOPBITS_ONE_POINT_FIVE, STOPBITS_TWO
    timeout (float) – Set a read timeout value.
    xonxoff (bool) – Enable software flow control.
    rtscts (bool) – Enable hardware (RTS/CTS) flow control.
    dsrdtr (bool) – Enable hardware (DSR/DTR) flow control.
    write_timeout (float) – Set a write timeout value.
    inter_byte_timeout (float) – Inter-character timeout, None to disable (default).

Raises:
    ValueError – Will be raised when parameter are out of range, e.g. baud rate, data bits.
    SerialException – In case the device can not be found or can not be configured.

'''
#OPÇÕES PARA RECEBIMENTO DE DADOS
#line = ser.read()          #Lê 1 byte
#line = ser.read(10)        #Lê 10 bytes
#line = ser.readline()      #Lê o caractere '\n' e interpreta como quebra de linha
#line = ser.read_all()      #Lê todos os bytes
#print(line)                 #Imprime em formato string
#print(int.from_bytes(line, byteorder='big'))     #Converte Bytes para inteiro
