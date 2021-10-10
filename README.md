# msgpack2
My standard and library for c++ for binary json representation



## Formats

### Overview

type name    | first byte | first byte  | size range           | value range
------------ | ---------- | ----------- | -------------------- | --------------
uint7        | 0xxxxxxx   | 0x00 - 0x7F |                      | 0x00 - 0x7F
string short | 10xxxxxx   | 0x80 - 0xBF | 0x00 - 0x3F          | 
map short    | 110xxxxx   | 0xC0 - 0xDF | 0x00 - 0x1F          | 
map short    | 1110xxxx   | 0xE0 - 0xEF | 0x00 - 0x0F          | 
string long  | 11110000   | 0xF0        | 0x40 - uint64 max    | 
array long   | 11110001   | 0xF1        | 0x20 - uint64 max    | 
map long     | 11110010   | 0xF2        | 0x10 - uint64 max    | 
array uint16 | 11110011   | 0xF3        | 0x10 - uint64 max    |  
array uint32 | 11110100   | 0xF4        | 0x10 - uint64 max    | 
array uint64 | 11110101   | 0xF5        | 0x10 - uint64 max    | 
true         | 11110110   | 0xF6        |                      | true
false        | 11110111   | 0xF7        |                      | false
real         | 11111000   | 0xF8        |                      | -a lot - +a lot
uint16       | 11111001   | 0xF9        |                      | 0x80 - 0x1007F
uint32       | 11111010   | 0xFA        |                      | 0x80 - 0x10000007F
uint64       | 11111011   | 0xFB        |                      | 0x80 - 0xFFFFFFFFFFFFFFFF
sint8        | 11111100   | 0xFC        |                      | -0x100 - -0x01
sint16       | 11111101   | 0xFD        |                      | -0x10000 - -0x01
sint32       | 11111110   | 0xFE        |                      | -0x100000000 - -0x01
sint64       | 11111111   | 0xFF        |                      | -0x10000000000000000 - -0x01



### Byte format

All integers are formatted in bigendian.

#### uint7
     +========+
	 |0xxxxxxx|
	 +========+
	 
#### string short
     +========+
	 |10xxxxxx|
	 +========+
	 
#### array short
     +========+
	 |110xxxxx|
	 +========+
	 
#### map short
     +========+
	 |1110xxxx|
	 +========+
	 
#### string long
     +========+===========+=================+
	 |  0xF0  | uint size | char[size+0x40] |
	 +========+===========+=================+
	 
#### array long
     +========+===========+====================+
	 |  0xF1  | uint size | anytype[size+0x20] |
	 +========+===========+====================+
	 
#### map long
     +========+===========+===============================+
	 |  0xF2  | uint size | (anytype, anytype)[size+0x10] |
	 +========+===========+===============================+
	 
#### array uint16
     +========+===========+==============+
	 |  0xF3  | uint size | uint16[size] |
	 +========+===========+==============+
	 
#### array uint32
     +========+===========+==============+
	 |  0xF4  | uint size | uint32[size] |
	 +========+===========+==============+
	 
#### array uint64
     +========+===========+==============+
	 |  0xF5  | uint size | uint64[size] |
	 +========+===========+==============+

#### true
     +========+
	 |  0xF6  |
	 +========+

#### false
     +========+
	 |  0xF7  |
	 +========+

#### real
     +========+==============+==============+
	 |  0xF8  | int mantissa | int exponent |
	 +========+==============+==============+

#### uint16
     +========+========+========+
	 |  0xF9  | val  0 | val  1 |   value = src-0x80
	 +========+========+========+

#### uint32
     +========+========+========+========+========+
	 |  0xFA  | val  0 | val  1 | val  2 | val  3 |   value = src-0x80
	 +========+========+========+========+========+

#### uint64
     +========+========+========+========+========+========+========+========+========+
	 |  0xFB  | val  0 | val  1 | val  2 | val  3 | val  4 | val  5 | val  6 | val  7 |   value = src-0x80
	 +========+========+========+========+========+========+========+========+========+

#### sint8
     +========+========+
	 |  0xFC  | val  0 |   value = -src
	 +========+========+

#### sint16
     +========+========+========+
	 |  0xFD  | val  0 | val  1 |   value = -src
	 +========+========+========+

#### sint32
     +========+========+========+========+========+
	 |  0xFE  | val  0 | val  1 | val  2 | val  3 |   value = -src
	 +========+========+========+========+========+

#### sint64
     +========+========+========+========+========+========+========+========+========+
	 |  0xFF  | val  0 | val  1 | val  2 | val  3 | val  4 | val  5 | val  6 | val  7 |   value = -src
	 +========+========+========+========+========+========+========+========+========+






