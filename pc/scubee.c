#include "../scubee.h"

#include <fcntl.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

#define MODEMPATH "/dev/ttyUSB0"

static int fd;

static void readFD( int ifd, uint8_t *buffer, size_t len ) {
    size_t len2 = 0;
    while ( len2 < len ) {
        size_t len3 = read( fd, &buffer[ len2 ], len - len2 );
        if ( len3 > 0 ) {
            len2 += len3;
        }
    }
}

static void writeFD( int ifd, const uint8_t *buffer, size_t len ) {
    size_t len2 = 0;
    while ( len2 < len ) {
        size_t len3 = write( fd, &buffer[ len2 ], len - len2 );
        if ( len3 > 0 ) {
            len2 += len3;
        }
    }
}

void enableModem( void ) {
    // Enabling and Disabling the modem is not supported on PC
}

void disableModem( void ) {
    // Enabling and Disabling the modem is not supported on PC
}

void sys_init( void ) {
    fd = open( MODEMPATH, O_RDWR );

    struct termios tty;
    tcgetattr( fd, &tty );
    cfsetspeed( &tty, B19200 );
    tty.c_cc[ VMIN ] = 0;
    tty.c_cc[ VTIME ] = 5;
    tty.c_iflag &= ~( IGNBRK | BRKINT | PARMRK | INLCR | IGNCR | ICRNL | ISTRIP | IXON );
    tty.c_oflag &= ~OPOST;
    tty.c_lflag &= ~( ECHO | ECHONL | ICANON | ISIG | IEXTEN );
    tty.c_cflag &= ~( CSIZE | CSTOPB | PARENB | CRTSCTS );
    tty.c_cflag |= CS8;
    tcsetattr( fd, TCSANOW, &tty );
}

extern size_t readSerialReady( void ) {
    size_t len;
    ioctl( 0, FIONREAD, &len );
    return len;
}

extern size_t readModemReady( void ) {
    size_t len;
    ioctl( fd, FIONREAD, &len );
    return len;
}

extern void readSerial( uint8_t *buffer, size_t len ) {
    readFD( 0, buffer, len );
}

extern void writeSerial( const uint8_t *buffer, size_t len ) {
    writeFD( 1, buffer, len );
}

extern void readModem( uint8_t *buffer, size_t len ) {
    readFD( fd, buffer, len );
}

extern void writeModem( const uint8_t *buffer, size_t len ) {
    writeFD( fd, buffer, len );
}
