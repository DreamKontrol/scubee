# Commands and Responses

All commands and responses without a payload consist of a single byte, called the opcode.

All commands and responces with a payload consist of at least three bytes: the opcode, the high byte of the payload length, and the low byte of the payload length; followed by the endian-agnostic payload.

## Commands

**NOP**
* Opcode: `00`
* Description: Does nothing.
* Response: `ACK`
* Payload: None

**SEND**
* Opcode: `40`
* Description: Writes a data packet to the modem.
* Response: `ACK`
* Response: `NAK` in `MODE_OFF`
* Payload: 4096 byte maximum
* Payload: 1960 byte maximum in `MODE_SBD`

**SEND_AT**
* Opcode: `41`
* Description: Writes a command packet to the modem.
* Response: `ACK`
* Response: `NAK` in `MODE_OFF`
* Payload: 125 byte maximum

**RECV**
* Opcode: `50`
* Description: Reads a data packet from the modem.
* Response: `DAT`
* Response: `NAK` in `MODE_OFF`
* Payload: None

**MODE_OFF**
* Opcode: `C0`
* Description: Disables the modem.
* Response: `ACK`
* Payload: None

**MODE_RAW**
* Opcode: `C1`
* Description: Enables the modem, and treats all data packets as command packets.
* Response: `ACK`
* Payload: None

**MODE_SBD**
* Opcode: `C2`
* Description: Enables the modem, and sends all data packets via SBD.
* Response: `ACK`
* Payload: None

**MODE_TCP**
* Opcode: `C3`
* Description: Enables the modem, and sends all data packets via TCP/IP. \[WORK IN PROGRESS\]
* Response: `ACK`
* Payload: None

# Responses

**ACK**
* Opcode: `00`
* Description: Command was acknowledged and performed.
* Payload: None

**NAK**
* Opcode: `40`
* Description: Command was acknowledged, but was not performed.
* Payload: None

**DAT**
* Opcode: `80`
* Description: Command was acknowledged and data was received.
* Payload: 4096 byte maximum

**ERR**
* Opcode: `C0`
* Description: Command was not acknowledged.
* Payload: None
