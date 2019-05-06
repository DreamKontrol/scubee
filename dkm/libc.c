#include "lwip/arch.h"

#include <stdint.h>
#include <string.h>

// Memory

int memcmp( const void *s1, const void *s2, size_t n ) {
    for ( size_t i = 0; i < n; i++ ) {
        if ( ( ( uint8_t * )s1 )[ i ] != ( ( uint8_t * )s2 )[ i ] ) {
            return ( ( uint8_t * )s1 )[ i ] - ( ( uint8_t * )s2 )[ i ];
        }
    }
    return 0;
}

void *memcpy( void *dest, const void *src, size_t n ) {
    for ( size_t i = 0; i < n; i++ ) {
        ( ( uint8_t * )dest )[ i ] = ( ( uint8_t * )src )[ i ];
    }
    return dest;
}

void *memmove( void *dest, const void *src, size_t n ) {

    if ( dest > src && dest < src + n ) {
        size_t ol = src - dest;
        memcpy( dest + ol, src + ol, n - ol );
        memcpy( dest, src, ol );
    } else {
        memcpy( dest, src, n );
    }

    return dest;
}

void *memset( void *s, int c, size_t n ) {
    for ( size_t i = 0; i < n; i++ ) {
        ( ( uint8_t * )s )[ i ] = c;
    }
    return s;
}

// Random

static uint32_t rng;

int rand( void ) {
    rng = 0xCAFEBABE * rng + 0xDEADBEEF; // TODO
    return rng;
}

void srand( unsigned seed ) {
    rng = seed;
}

// String

size_t strlen( const char *s ) {
    size_t i = 0;
    while ( s[ i ] ) {
        i++;
    }
    return i;
}

char *strcpy( char *dest, const char *src ) {
    size_t i = 0;
    for ( ; src[ i ]; i++ ) {
        dest[ i ] = src[ i ];
    }
    dest[ i ] = 0;
    return dest;
}

char *strncpy( char *dest, const char *src, size_t n ) {
    size_t i = 0;
    for ( ; i < n && src[ i ]; i++ ) {
        dest[ i ] = src[ i ];
    }
    if ( i < n ) {
        dest[ i ] = 0;
    }
    return dest;
}

int strncmp( const char *s1, const char *s2, size_t n ) {
    for ( size_t i = 0; i < n && s1[ i ] && s2[ i ]; i++ ) {
        if ( s1[ i ] != s2[ i ] ) {
            return s1[ i ] - s2[ i ];
        }
    }
    return 0;
}

// Miscellanious

int atoi( const char *nptr ) {
    int i = 0;
    int s = 1;

    while ( lwip_isspace( *nptr ) ) {
        nptr++;
    }

    if ( *nptr == '-' ) {
        s = -1;
        nptr++;
    } else if ( *nptr == '+' ) {
        nptr++;
    }

    while ( *nptr ) {
        i = i * 10 + ( *nptr++ - '0' );
    }

    return i * s;
}