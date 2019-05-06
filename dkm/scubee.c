#include "../scubee.h"

typedef volatile uint8_t *mmio8_t;
typedef volatile uint16_t *mmio16_t;
typedef volatile uint32_t *mmio32_t;
typedef mmio8_t mmio_t;

#define UART_SERIAL_SERCOM 5
#define UART_SERIAL_BAUD 1152000
#define UART_SERIAL_TX_PORT 1
#define UART_SERIAL_TX_PIN 22
#define UART_SERIAL_RX_PORT 1
#define UART_SERIAL_RX_PIN 23

#define UART_MODEM_SERCOM 0
#define UART_MODEM_BAUD 19200
#define UART_MODEM_TX_PORT 0
#define UART_MODEM_TX_PIN 10
#define UART_MODEM_RX_PORT 0
#define UART_MODEM_RX_PIN 11

#define IR_5V 5
#define IR_EN 11
#define IR_TESBUS 12

static mmio_t uartSerial = ( mmio_t )( 0x42000800 + UART_SERIAL_SERCOM * 0x400 );
static mmio_t uartModem = ( mmio_t )( 0x42000800 + UART_MODEM_SERCOM * 0x400 );

static int readUartReady( mmio_t uart ) {
    return !( uart[ 0x18 ] & 4 );
}

static int writeUartReady( mmio_t uart ) {
    return !( uart[ 0x18 ] & 1 );
}

static uint8_t readUart( mmio_t uart ) {
    while ( !readUartReady( uart ) ) {
    }
    return uart[ 0x28 ];
}

static void writeUart( mmio_t uart, uint8_t value ) {
    while ( !writeUartReady( uart ) ) {
    }
    uart[ 0x28 ] = value;
}

static void configPin( int port, int pin, int mux, int mux2 ) {
    mmio_t addr = ( mmio_t )( 0x41004400 + ( port ? 0x80 : 0x00 ) );
    addr[ 0x40 + pin ] = 1;
    if ( pin & 1 ) {
        addr[ 0x30 + mux ] = ( addr[ 0x30 + mux ] & 0x0F ) | ( mux2 << 4 );
    } else {
        addr[ 0x30 + mux ] = ( addr[ 0x30 + mux ] & 0xF0 ) | mux2;
    }
}

static void configUart( mmio_t uart, int sercom, int baud ) {
    *( mmio32_t )( 0x40000420 ) |= 1 << ( sercom + 2 );
    *( mmio16_t )( 0x40000C02 ) = 0x4514 + sercom;

    *( mmio32_t )( &uart[ 0x00 ] ) = 0x00000000;
    *( mmio32_t )( &uart[ 0x00 ] ) = 0x00000001;
    *( mmio32_t )( &uart[ 0x00 ] ) = 0x40310004;
    *( mmio32_t )( &uart[ 0x04 ] ) = 0x00030000;
    *( mmio16_t )( &uart[ 0x0C ] ) = 65536 - 2048 * baud / 15625;
    *( mmio32_t )( &uart[ 0x00 ] ) = 0x40310006;
}

static uint16_t readSX1509( uint8_t addr ) {
    // TODO WIRE = I²C connection
    /*
    WIRE.beginTransmition( 0x3E );
    WIRE.write8( addr );
    WIRE.endTransmition( );
    uint8_t hi = WIRE.read16( );
    uint8_t lo = WIRE.read16( );
    return ( hi << 8 ) | lo;
    */
}

static void writeSX1509( uint8_t addr, uint16_t value ) {
    // TODO WIRE = I²C connection
    /*
    WIRE.beginTransmition( 0x3E );
    WIRE.write8( addr );
    WIRE.write8( value >> 8 );
    WIRE.write8( value );
    WIRE.endTransmition( );
    */
}

void enableModem( void ) {
    uint16_t pins = readSX1509( 0x10 );
    writeSX1509( 0x10, pins |= ( 1 << IR_5V ) );
    writeSX1509( 0x10, pins |= ( 1 << IR_EN ) );
    writeSX1509( 0x10, pins |= ( 1 << IR_TESBUS ) );
}

void disableModem( void ) {
    uint16_t pins = readSX1509( 0x10 );
    writeSX1509( 0x10, pins &= ~( 1 << IR_TESBUS ) );
    writeSX1509( 0x10, pins &= ~( 1 << IR_EN ) );
    writeSX1509( 0x10, pins &= ~( 1 << IR_5V ) );
}

void sys_init( void ) {
    *( mmio32_t )( 0x40000820 ) &= ~0x300;
    *( mmio32_t )( 0x40000C04 ) = 0x00010605;

    writeSX1509( 0x0E, readSX1509( 0x0E ) | ( 1 << IR_5V ) | ( 1 << IR_EN ) | ( 1 << IR_TESBUS ) );

    configUart( uartSerial, UART_SERIAL_SERCOM, UART_SERIAL_BAUD );
    configPin( UART_SERIAL_TX_PORT, UART_SERIAL_TX_PIN, 11, 3 );
    configPin( UART_SERIAL_RX_PORT, UART_SERIAL_RX_PIN, 11, 3 );

    configUart( uartModem, UART_MODEM_SERCOM, UART_MODEM_BAUD );
    configPin( UART_MODEM_TX_PORT, UART_MODEM_TX_PIN, 5, 2 );
    configPin( UART_MODEM_RX_PORT, UART_MODEM_RX_PIN, 5, 2 );
}

extern size_t readSerialReady( void ) {
    return readUartReady( uartSerial );
}

extern size_t readModemReady( void ) {
    return readUartReady( uartModem );
}

extern void readSerial( uint8_t *buffer, size_t len ) {
    for ( size_t i = 0; i < len; i++ ) {
        buffer[ i ] = readUart( uartSerial );
    }
}

extern void writeSerial( const uint8_t *buffer, size_t len ) {
    for ( size_t i = 0; i < len; i++ ) {
        writeUart( uartSerial, buffer[ i ] );
    }
}

extern void readModem( uint8_t *buffer, size_t len ) {
    for ( size_t i = 0; i < len; i++ ) {
        buffer[ i ] = readUart( uartModem );
    }
}

extern void writeModem( const uint8_t *buffer, size_t len ) {
    for ( size_t i = 0; i < len; i++ ) {
        writeUart( uartModem, buffer[ i ] );
    }
}
