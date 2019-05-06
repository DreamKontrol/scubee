#include "lwip/def.h"
#include "lwip/init.h"
#include "lwip/sys.h"
#include "netif/ppp/pppos.h"
#include "scubee.h"

#define CMD_NOP 0x00
#define CMD_SEND 0x40
#define CMD_SEND_AT 0x41
#define CMD_RECV 0x50
#define CMD_MODE_OFF 0xC0
#define CMD_MODE_RAW 0xC1
#define CMD_MODE_SBD 0xC2
#define CMD_MODE_TCP 0xC3

#define CMD_RESP_ACK 0x00
#define CMD_RESP_NAK 0x40
#define CMD_RESP_DAT 0x80
#define CMD_RESP_ERR 0xC0

#define BUFSIZE 4096
static uint8_t ibuffer[ BUFSIZE ];
static size_t ibuffer_len;
static size_t ibuffer_max;
static uint8_t obuffer[ BUFSIZE ];
static size_t obuffer_len;
static size_t obuffer_max;

static uint8_t readSerial1( void ) {
    uint8_t c;
    readSerial( &c, 1 );
    return c;
}

static void writeSerial1( uint8_t c ) {
    writeSerial( &c, 1 );
}

static int atcommand( const char *cmd, size_t clen, const char *resp, size_t rlen ) {
    if ( cmd ) {
        writeSerial( ( uint8_t * )"AT", 2 );
        writeSerial( ( uint8_t * )cmd, clen );
        writeSerial( ( uint8_t * )"\r", 1 );
        if ( readSerial1( ) != 'A' ) {
            return 1;
        }
        if ( readSerial1( ) != 'T' ) {
            return 1;
        }
        for ( size_t i = 0; i < clen; i++ ) {
            if ( readSerial1( ) != cmd[ i ] ) {
                return 1;
            }
        }
        if ( readSerial1( ) != '\r' ) {
            return 1;
        }
    }
    if ( resp ) {
        for ( size_t i = 0; i < rlen; i++ ) {
            if ( readSerial1( ) != resp[ i ] ) {
                return 1;
            }
        }
    }
    return 0;
}

int main( void ) {

#ifdef NO_SYS || 1
    lwip_init( );
#else
    tcpip_init( 0, 0 ); // TODO
#endif

    uint8_t mode = 0;
    uint8_t newmode = CMD_MODE_OFF;

    for ( ;; ) {
        if ( mode != newmode ) {
#if !NO_SYS
            fprintf( stderr, "MODE %02X->%02X\n", mode, newmode );
#endif

            if ( mode == CMD_MODE_OFF ) {
                // TODO enable modem
            } else if ( mode == CMD_MODE_RAW ) {
            } else if ( mode == CMD_MODE_SBD ) {
            } else if ( mode == CMD_MODE_TCP ) {
                // TODO clean up TCP stack
                sys_msleep( 1000 );
                writeSerial( ( uint8_t * )"+++", 3 );
                sys_msleep( 1000 );
                atcommand( "H", 1, "OK\r\n", 4 );
            }

            mode = newmode;
            ibuffer_len = 0;
            obuffer_len = 0;
            if ( mode == CMD_MODE_OFF ) {
                // TODO disable modem
                sys_msleep( 2000 );
                ibuffer_max = 0;
                obuffer_max = 0;
            } else if ( mode == CMD_MODE_RAW ) {
                ibuffer_max = BUFSIZE;
                obuffer_max = BUFSIZE;
            } else if ( mode == CMD_MODE_SBD ) {
                atcommand( "Z", 1, "OK\r\n", 4 );
                atcommand( "S0=1V1X4E1Q0&C1", 15, "OK\r\n", 4 );
                atcommand( "+CBST=71,0,1", 12, "OK\r\n", 4 );
                atcommand( "+CR=1", 5, "OK\r\n", 4 );
                memcpy( ibuffer, "+SBDWT=", 7 );
                ibuffer_len = 7;
                ibuffer_max = 1960 + 7;
                obuffer_max = 0;
            } else if ( mode == CMD_MODE_TCP ) {
                atcommand( "Z", 1, "OK\r\n", 4 );
                atcommand( "S0=1V1X4E1Q0&C1", 15, "OK\r\n", 4 );
                atcommand( "+CBST=71,0,1", 12, "OK\r\n", 4 );
                atcommand( "+CR=1", 5, "OK\r\n", 4 );
                atcommand( "D008816000025", 13, "OK\r\n", 4 );
                // TODO create TCP stack
                ibuffer_max = BUFSIZE;
                obuffer_max = BUFSIZE;
            }
        }

        if ( mode == CMD_MODE_OFF ) {
        } else if ( mode == CMD_MODE_RAW ) {
            if ( ibuffer_len > 0 ) {
                writeSerial( ibuffer, ibuffer_len );
                ibuffer_len = 0;
            }
            if ( obuffer_len < obuffer_max ) {
                size_t len = readModemReady( );
                if ( len > obuffer_max - obuffer_len ) {
                    len = obuffer_max - obuffer_len;
                }
                if ( len > 0 ) {
                    readModem( &obuffer[ obuffer_len ], len );
                    obuffer_len += len;
                }
            }
        } else if ( mode == CMD_MODE_SBD ) {
            if ( ibuffer_len > 0 ) {
                atcommand( ( char * )ibuffer, ibuffer_len, ( char * )"OK", 2 );
                atcommand( ( char * )"+SBDI", 5, ( char * )"OK", 2 );
            }
            {
                size_t len = readModemReady( );
                while ( len-- ) {
                    uint8_t c;
                    readModem( &c, 1 );
                }
            }
        } else if ( mode == CMD_MODE_TCP ) {
            // TODO
        }

        if ( readSerialReady( ) ) {
            uint8_t cmd = readSerial1( );
#if !NO_SYS
            fprintf( stderr, "CMD %02X\n", cmd );
#endif
            switch ( cmd ) {

            case CMD_NOP:
                writeSerial1( CMD_RESP_ACK );
                break;

            case CMD_SEND: {
                uint16_t len = 0;
                readSerial( ( uint8_t * )&len, 2 );
                len = lwip_ntohs( len );
                if ( mode == CMD_MODE_OFF ) {
                    writeSerial1( CMD_RESP_NAK );
                } else {
                    size_t len2 = len;
                    if ( len > ibuffer_max - ibuffer_len ) {
                        len2 = ibuffer_max - ibuffer_len;
                    }
                    if ( len2 > 0 ) {
                        readSerial( &ibuffer[ ibuffer_len ], len2 );
                        len -= len2;
                    }
                }
                for ( int i = 0; i < len; i++ ) {
                    readSerial1( );
                }
                break;
            }

            case CMD_SEND_AT: {
                uint16_t len = 0;
                readSerial( ( uint8_t * )&len, 2 );
                len = lwip_ntohs( len );
                if ( mode == CMD_MODE_OFF ) {
                    writeSerial1( CMD_RESP_NAK );
                } else {
                    uint8_t buf[ 125 ];
                    size_t len2 = len;
                    if ( len > 125 ) {
                        len2 = 125;
                    }
                    if ( len2 > 0 ) {
                        readSerial( buf, len2 );
                        len -= len2;
                    }
                    atcommand( ( char * )buf, len2, 0, 0 );
                }
                for ( int i = 0; i < len; i++ ) {
                    readSerial1( );
                }
                break;
            }

            case CMD_RECV: {
                if ( mode == CMD_MODE_OFF ) {
                    writeSerial1( CMD_RESP_NAK );
                } else {
                    writeSerial1( CMD_RESP_DAT );
                    uint16_t len = lwip_htons( obuffer_len );
                    writeSerial( ( uint8_t * )&len, 2 );
                    writeSerial( obuffer, obuffer_len );
                }
                break;
            }

            case CMD_MODE_OFF:
            case CMD_MODE_RAW:
            case CMD_MODE_SBD:
                newmode = cmd;
                writeSerial1( CMD_RESP_ACK );
                break;

            case CMD_MODE_TCP:
                writeSerial1( CMD_RESP_NAK ); // TODO
                break;

            default:
#if !NO_SYS
                fprintf( stderr, "UNKNOWN COMMAND %02X\n", cmd );
#endif
                writeSerial1( CMD_RESP_ERR );
                break;
            }
        }
    }

    return 0;
}
