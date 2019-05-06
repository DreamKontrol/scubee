#ifndef SCUBEE_H
#define SCUBEE_H

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

extern void enableModem( void );
extern void disableModem( void );

extern size_t readSerialReady( void );
extern size_t readModemReady( void );

extern void readSerial( uint8_t *buffer, size_t len );
extern void writeSerial( const uint8_t *buffer, size_t len );
extern void readModem( uint8_t *buffer, size_t len );
extern void writeModem( const uint8_t *buffer, size_t len );

#endif