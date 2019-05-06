
# Configuration

## Microcontroller

### Memory

#### `dkm/dkm.ld:4-5`

Set the LENGTH field of each section to be equal to the size of the memory in that section.  Put the size of FLASH in the `rom` section, and the size of SRAM in the `ram` section.

#### `dkm/crt0.s:12`

Set this to be equal to 0x20000000 plus the size of SRAM.

### Serial Ports

#### `dkm/scubee.c:8`

Set this to be equal to the id of the Serial sercom device.

#### `dkm/scubee.c:9`

Set this to be equal to the baud rate of the Serial sercom device.

#### `dkm/scubee.c:10`

Set this to be equal to the id of the Serial TX port.  Use `0` for `PA` and `1` for `PB`.

#### `dkm/scubee.c:11`

Set this to be equal to the id of the Serial TX pin.  Note that this is the virtual pin (e.g. `PA10`) not the physical pin (e.g. `P15`).

#### `dkm/scubee:12-13`

Same as lines `10-11`, except for Serial RX instead.

#### `dkm/scubee:15-20`

Same as lines `8-13`, except for the Modem instead.

#### `dkm/scubee:22`

Set this to be equal to the id of the Modem 5V Enable pin.

#### `dkm/scubee:23`

Set this to be equal to the id of the Modem Enable pin.

#### `dkm/scubee:24`

Set this to be equal to the id of the Modem TES Bus Enable pin.

## PC

#### `pc/scubee:8`

Set this to the path of the modem.  Should be `/dev/ttyUSB0`, `/dev/ttyS0`, `//COM18`, or similar.
