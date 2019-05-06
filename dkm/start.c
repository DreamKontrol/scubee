#include <stdint.h>
#include <string.h>

extern int main( void );

extern uint32_t vectors;
extern uint32_t _etext;
extern uint32_t _data;
extern uint32_t _edata;
extern uint32_t _bss;
extern uint32_t _ebss;

void handlerReset( void ) {
    memcpy( &_data, &_etext, &_edata - &_data );
    memset( &_bss, 0x00, &_ebss - &_bss );

    *( uint32_t * )0xE000ED08 =  ( uint32_t )&vectors;

    main( );
    for ( ;; ) {
        asm( "wfi" );
    }
}
