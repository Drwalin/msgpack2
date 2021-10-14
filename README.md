# msgpack2
My standard and library for C++ for binary json representation



## Formats

### Overview

type name    | first byte | first byte  | size range           | value range
------------ | ---------- | ----------- | -------------------- | --------------
uint7        | 0xxxxxxx   | 0x00 - 0x7F |                      | 0x00 - 0x7F
string short | 10xxxxxx   | 0x80 - 0xBF | 0x00 - 0x3F          | 
array short  | 110xxxxx   | 0xC0 - 0xDF | 0x00 - 0x1F          | 
UNUSED       |            | 0xE0 - 0xEE |                      |             
array long   | 11101111   | 0xEF        | 0x20 - uint64 max    | 
array uint8  | 11110000   | 0xF0        | 0x00 - uint64 max    |
array uint16 | 11110001   | 0xF1        | 0x00 - uint64 max    |  
array uint32 | 11110010   | 0xF2        | 0x00 - uint64 max    | 
array uint64 | 11110011   | 0xF3        | 0x00 - uint64 max    | 
array real   | 11110100   | 0xF4        | 0x00 - uint64 max    | 
true         | 11110101   | 0xF5        |                      | true
false        | 11110110   | 0xF6        |                      | false
real         | 11110111   | 0xF7        |                      | -a lot - +a lot
uint8        | 11111000   | 0xF8        |                      | 0x00 - 0xFF
uint16       | 11111001   | 0xF9        |                      | 0x100 - 0x100FF
uint32       | 11111010   | 0xFA        |                      | 0x10100 - 0x1000100FF
uint64       | 11111011   | 0xFB        |                      | 0x0 - 0xFFFFFFFFFFFFFFFF
sint8        | 11111100   | 0xFC        |                      | 
sint16       | 11111101   | 0xFD        |                      | 
sint32       | 11111110   | 0xFE        |                      | 
sint64       | 11111111   | 0xFF        |                      | 



### Byte format

All integers are formatted in bigendian.

#### uint7
     +--------+
	 |0xxxxxxx|
	 +--------+
	 
#### string short
     +--------+==============+
	 |10xxxxxx| char[xxxxxx] |
	 +--------+==============+
	 
#### array short
     +--------+================+
	 |110xxxxx| anytype[xxxxx] |
	 +--------+================+
	 
#### array long
     +--------+-----------+====================+
	 |  0xEF  | uint size | anytype[size+0x20] |
	 +--------+-----------+====================+
	 
#### array uint8
     +--------+-----------+=============+
	 |  0xF0  | uint size | uint8[size] |
	 +--------+-----------+=============+
	 
#### array uint16
     +--------+-----------+==============+
	 |  0xF1  | uint size | uint16[size] |
	 +--------+-----------+==============+
	 
#### array uint32
     +--------+-----------+==============+
	 |  0xF2  | uint size | uint32[size] |
	 +--------+-----------+==============+
	 
#### array uint64
     +--------+-----------+==============+
	 |  0xF3  | uint size | uint64[size] |
	 +--------+-----------+==============+
	 
#### array real
     +--------+-----------+============+
	 |  0xF4  | uint size | real[size] |
	 +--------+-----------+============+

#### true
     +--------+
	 |  0xF5  |
	 +--------+

#### false
     +--------+
	 |  0xF6  |
	 +--------+

#### real
     +--------+--------------+--------------+
	 |  0xF7  | int mantissa | int exponent |
	 +--------+--------------+--------------+
	 
#### uint8
     +--------+--------+
	 |  0xF8  | val  0 |   value = src
	 +--------+--------+

#### uint16
     +--------+--------+--------+
	 |  0xF9  | val  0 | val  1 |   value = src-0x80
	 +--------+--------+--------+

#### uint32
     +--------+--------+--------+--------+--------+
	 |  0xFA  | val  0 | val  1 | val  2 | val  3 |   value = src-0x80
	 +--------+--------+--------+--------+--------+

#### uint64
     +--------+--------+--------+--------+--------+--------+--------+--------+--------+
	 |  0xFB  | val  0 | val  1 | val  2 | val  3 | val  4 | val  5 | val  6 | val  7 |   value = src-0x80
	 +--------+--------+--------+--------+--------+--------+--------+--------+--------+

#### sint8
     +--------+--------+
	 |  0xFC  | val  0 |   value = -src
	 +--------+--------+

#### sint16
     +--------+--------+--------+
	 |  0xFD  | val  0 | val  1 |   value = -src
	 +--------+--------+--------+

#### sint32
     +--------+--------+--------+--------+--------+
	 |  0xFE  | val  0 | val  1 | val  2 | val  3 |   value = -src
	 +--------+--------+--------+--------+--------+

#### sint64
     +--------+--------+--------+--------+--------+--------+--------+--------+--------+
	 |  0xFF  | val  0 | val  1 | val  2 | val  3 | val  4 | val  5 | val  6 | val  7 |   value = -src
	 +--------+--------+--------+--------+--------+--------+--------+--------+--------+






