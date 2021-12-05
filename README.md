# msgpack2
My standard and library for C++ for binary json representation



## Formats

### Overview

type name      | first byte | first byte  | size range           | value range
-------------- | ---------- | ----------- | -------------------- | --------------
uint7          | 0xxxxxxx   | 0x00 - 0x7F |                      | 0x00 - 0x7F
string short   | 10xxxxxx   | 0x80 - 0xBF | 0x00 - 0x3F          |
array short    | 110xxxxx   | 0xC0 - 0xDF | 0x00 - 0x1F          |
UNUSED         |            | 0xE0 - 0xE8 |                      |
map long 8     | 11101001   | 0xE9        | 0x00 - uint8 max     |
map long 16    | 11101010   | 0xEA        | 0x00 - uint16 max    |
map long 32    | 11101011   | 0xEB        | 0x00 - uint32 max    |
map long 64    | 11101100   | 0xEC        | 0x00 - uint64 max    |
string long 8  | 11101101   | 0xED        | 0x00 - uint8 max     |
string long 16 | 11101110   | 0xEE        | 0x00 - uint16 max    |
string long 32 | 11101111   | 0xEF        | 0x00 - uint32 max    |
string long 64 | 11110000   | 0xF0        | 0x00 - uint64 max    |
array long 8   | 11110001   | 0xF1        | 0x00 - uint8 max     |
array long 16  | 11110010   | 0xF2        | 0x00 - uint16 max    |
array long 32  | 11110011   | 0xF3        | 0x00 - uint32 max    |
array long 64  | 11110100   | 0xF4        | 0x00 - uint64 max    |
true           | 11110101   | 0xF5        |                      | true
false          | 11110110   | 0xF6        |                      | false
real           | 11110111   | 0xF7        |                      | -a lot - +a lot
uint8          | 11111000   | 0xF8        |                      | 0x0 - 0xFF
uint16         | 11111001   | 0xF9        |                      | 0x0 - 0xFFFF
uint32         | 11111010   | 0xFA        |                      | 0x0 - 0xFFFFFFFF
uint64         | 11111011   | 0xFB        |                      | 0x0 - 0xFFFFFFFFFFFFFFFF
sint8          | 11111100   | 0xFC        |                      | -0x80 - 0x7F
sint16         | 11111101   | 0xFD        |                      | -0x8000 - 0x7FFF
sint32         | 11111110   | 0xFE        |                      | -0x80000000 - 0x7FFFFFFF
sint64         | 11111111   | 0xFF        |                      | -0x8000000000000000 - 0x7FFFFFFFFFFFFFFF



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
     
#### map short
     +--------+================+
     |110xxxxx| anytype[xxxxx] |
     +--------+================+
     
#### map long 8
     +--------+-------------+==============+
     |  0xE9  | uint8 size | anytype[size] |
     +--------+-------------+==============+
     
#### map long 16
     +--------+-------------+===============+
     |  0xEA  | uint16 size | anytype[size] |
     +--------+-------------+===============+
     
#### map long 32
     +--------+-------------+===============+
     |  0xEB  | uint32 size | anytype[size] |
     +--------+-------------+===============+
     
#### map long 64
     +--------+-------------+===============+
     |  0xEC  | uint64 size | anytype[size] |
     +--------+-------------+===============+
     
#### string long 8
     +--------+-------------+==============+
     |  0xED  | uint8 size | anytype[size] |
     +--------+-------------+==============+
     
#### string long 16
     +--------+-------------+===============+
     |  0xEE  | uint16 size | anytype[size] |
     +--------+-------------+===============+
     
#### string long 32
     +--------+-------------+===============+
     |  0xEF  | uint32 size | anytype[size] |
     +--------+-------------+===============+
     
#### string long 64
     +--------+-------------+===============+
     |  0xF0  | uint64 size | anytype[size] |
     +--------+-------------+===============+
     
#### array long 8
     +--------+-------------+==============+
     |  0xF1  | uint8 size | anytype[size] |
     +--------+-------------+==============+
     
#### array long 16
     +--------+-------------+===============+
     |  0xF2  | uint16 size | anytype[size] |
     +--------+-------------+===============+
     
#### array long 32
     +--------+-------------+===============+
     |  0xF3  | uint32 size | anytype[size] |
     +--------+-------------+===============+
     
#### array long 64
     +--------+-------------+===============+
     |  0xF4  | uint64 size | anytype[size] |
     +--------+-------------+===============+

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



