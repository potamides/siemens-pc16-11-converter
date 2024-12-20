/*
Copyright 2012 Jun Wako <wakojun@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CONFIG_H
#define CONFIG_H


#define VENDOR_ID       0xFEED
#define PRODUCT_ID      0x6800
#define DEVICE_VER      0x0001
#define MANUFACTURER    Siemens
#define PRODUCT         Siemens PC 16-11 keyboard converter
#define DESCRIPTION     converts Siemens PC 16-11 keyboard protocol to USB


#define LOCKING_SUPPORT_ENABLE
#define LOCKING_RESYNC_ENABLE


/* matrix size */
#define MATRIX_ROWS 12
#define MATRIX_COLS 8


#define MATRIX_ROW(code)    ((code)/MATRIX_COLS)
#define MATRIX_COL(code)    ((code)%MATRIX_COLS)


/* USART configuration
 *     asynchronous, 2400baud, 8-data bit, even parity, 1-stop bit, no flow control
 */
#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega32U2__)
    #define SERIAL_UART_BAUD       2400
    #define SERIAL_UART_DATA       UDR1
    #define SERIAL_UART_UBRR       ((F_CPU/(16UL*SERIAL_UART_BAUD))-1)
    #define SERIAL_UART_RXD_VECT   USART1_RX_vect
    #define SERIAL_UART_TXD_READY  (UCSR1A&(1<<UDRE1))
    #define SERIAL_UART_INIT()     do { \
        UBRR1L = (uint8_t) SERIAL_UART_UBRR;       /* baud rate */ \
        UBRR1H = (uint8_t) (SERIAL_UART_UBRR>>8);  /* baud rate */ \
        UCSR1B = (1<<RXCIE1) | (1<<RXEN1) | /* RX: interrupt, RX: enable */ \
                 (1<<TXEN1);                /* TX: enable */ \
        UCSR1C = (0<<UPM11) | (1<<UPM10) | /* parity: none(00), even(01), odd(11) */ \
                 (0<<UCSZ12) | (1<<UCSZ11) | (1<<UCSZ10); /* data-8bit(011) */ \
        sei(); \
    } while(0)
#else
#   error "USART configuration is needed."
#endif

#endif
