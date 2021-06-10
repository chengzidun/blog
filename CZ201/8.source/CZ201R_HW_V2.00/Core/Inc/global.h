#ifndef __GLOBAL_H__
#define __GLOBAL_H__

typedef	        char	           s8_t;
typedef         unsigned char	   u8_t;
typedef		    short     		   s16_t;
typedef		    unsigned short	   u16_t;
typedef		    int         	   s32_t;
typedef		    unsigned int  	   u32_t;
// typedef			long		s32_t;
// typedef			unsigned long	u32_t;
typedef 	    long long		        s64_t;
typedef		    unsigned long long	    u64_t;
typedef 	    float			        f32_t;
typedef		    double			        f64_t;

typedef         char*			        string_t;

//boolean type
typedef enum _boolean{
	_False = 0,
	_True = !_False
}boolean;

//level status type
typedef enum	_level_t{
	_Low = 0,
	_High = !_Low
}level_t;

//switch status type
typedef enum _status_t{
  _Off = 0,
  _On = !_Off
}sw_status_t;

//function status type
typedef enum _func_status_t{
  _Disable = 0,
  _Enable = !_Disable
}func_staus_t;

typedef union {
	u8_t u8[4];
	s8_t s8[4];
	u16_t u16[2];
	s16_t s16[2];
	u32_t u32;
	s32_t s32;
	float f32;
}union_type_t;

typedef struct
{
	u32_t baudrate;
	u8_t parity;
	u8_t databit;
	u8_t stopbit;
}serial_format_t;

typedef struct
{
	u8_t state;
	u16_t buffer_size;
	u8_t *pbuffer;
	u16_t receive_size;
	serial_format_t format;
}serial_info_t;

//binary type
#define	OOOOOOOO	0x00
#define	OOOOOOOI	0x01
#define	OOOOOOIO	0x02
#define	OOOOOOII	0x03
#define	OOOOOIOO	0x04
#define	OOOOOIOI	0x05
#define	OOOOOIIO	0x06
#define	OOOOOIII	0x07
#define	OOOOIOOO	0x08
#define	OOOOIOOI	0x09
#define	OOOOIOIO	0x0a
#define	OOOOIOII	0x0b
#define	OOOOIIOO	0x0c
#define	OOOOIIOI	0x0d
#define	OOOOIIIO	0x0e
#define	OOOOIIII	0x0f
#define	OOOIOOOO	0x10
#define	OOOIOOOI	0x11
#define	OOOIOOIO	0x12
#define	OOOIOOII	0x13
#define	OOOIOIOO	0x14
#define	OOOIOIOI	0x15
#define	OOOIOIIO	0x16
#define	OOOIOIII	0x17
#define	OOOIIOOO	0x18
#define	OOOIIOOI	0x19
#define	OOOIIOIO	0x1a
#define	OOOIIOII	0x1b
#define	OOOIIIOO	0x1c
#define	OOOIIIOI	0x1d
#define	OOOIIIIO	0x1e
#define	OOOIIIII	0x1f
#define	OOIOOOOO	0x20
#define	OOIOOOOI	0x21
#define	OOIOOOIO	0x22
#define	OOIOOOII	0x23
#define	OOIOOIOO	0x24
#define	OOIOOIOI	0x25
#define	OOIOOIIO	0x26
#define	OOIOOIII	0x27
#define	OOIOIOOO	0x28
#define	OOIOIOOI	0x29
#define	OOIOIOIO	0x2a
#define	OOIOIOII	0x2b
#define	OOIOIIOO	0x2c
#define	OOIOIIOI	0x2d
#define	OOIOIIIO	0x2e
#define	OOIOIIII	0x2f
#define	OOIIOOOO	0x30
#define	OOIIOOOI	0x31
#define	OOIIOOIO	0x32
#define	OOIIOOII	0x33
#define	OOIIOIOO	0x34
#define	OOIIOIOI	0x35
#define	OOIIOIIO	0x36
#define	OOIIOIII	0x37
#define	OOIIIOOO	0x38
#define	OOIIIOOI	0x39
#define	OOIIIOIO	0x3a
#define	OOIIIOII	0x3b
#define	OOIIIIOO	0x3c
#define	OOIIIIOI	0x3d
#define	OOIIIIIO	0x3e
#define	OOIIIIII	0x3f
#define	OIOOOOOO	0x40
#define	OIOOOOOI	0x41
#define	OIOOOOIO	0x42
#define	OIOOOOII	0x43
#define	OIOOOIOO	0x44
#define	OIOOOIOI	0x45
#define	OIOOOIIO	0x46
#define	OIOOOIII	0x47
#define	OIOOIOOO	0x48
#define	OIOOIOOI	0x49
#define	OIOOIOIO	0x4a
#define	OIOOIOII	0x4b
#define	OIOOIIOO	0x4c
#define	OIOOIIOI	0x4d
#define	OIOOIIIO	0x4e
#define	OIOOIIII	0x4f
#define	OIOIOOOO	0x50
#define	OIOIOOOI	0x51
#define	OIOIOOIO	0x52
#define	OIOIOOII	0x53
#define	OIOIOIOO	0x54
#define	OIOIOIOI	0x55
#define	OIOIOIIO	0x56
#define	OIOIOIII	0x57
#define	OIOIIOOO	0x58
#define	OIOIIOOI	0x59
#define	OIOIIOIO	0x5a
#define	OIOIIOII	0x5b
#define	OIOIIIOO	0x5c
#define	OIOIIIOI	0x5d
#define	OIOIIIIO	0x5e
#define	OIOIIIII	0x5f
#define	OIIOOOOO	0x60
#define	OIIOOOOI	0x61
#define	OIIOOOIO	0x62
#define	OIIOOOII	0x63
#define	OIIOOIOO	0x64
#define	OIIOOIOI	0x65
#define	OIIOOIIO	0x66
#define	OIIOOIII	0x67
#define	OIIOIOOO	0x68
#define	OIIOIOOI	0x69
#define	OIIOIOIO	0x6a
#define	OIIOIOII	0x6b
#define	OIIOIIOO	0x6c
#define	OIIOIIOI	0x6d
#define	OIIOIIIO	0x6e
#define	OIIOIIII	0x6f
#define	OIIIOOOO	0x70
#define	OIIIOOOI	0x71
#define	OIIIOOIO	0x72
#define	OIIIOOII	0x73
#define	OIIIOIOO	0x74
#define	OIIIOIOI	0x75
#define	OIIIOIIO	0x76
#define	OIIIOIII	0x77
#define	OIIIIOOO	0x78
#define	OIIIIOOI	0x79
#define	OIIIIOIO	0x7a
#define	OIIIIOII	0x7b
#define	OIIIIIOO	0x7c
#define	OIIIIIOI	0x7d
#define	OIIIIIIO	0x7e
#define	OIIIIIII	0x7f
#define	IOOOOOOO	0x80
#define	IOOOOOOI	0x81
#define	IOOOOOIO	0x82
#define	IOOOOOII	0x83
#define	IOOOOIOO	0x84
#define	IOOOOIOI	0x85
#define	IOOOOIIO	0x86
#define	IOOOOIII	0x87
#define	IOOOIOOO	0x88
#define	IOOOIOOI	0x89
#define	IOOOIOIO	0x8a
#define	IOOOIOII	0x8b
#define	IOOOIIOO	0x8c
#define	IOOOIIOI	0x8d
#define	IOOOIIIO	0x8e
#define	IOOOIIII	0x8f
#define	IOOIOOOO	0x90
#define	IOOIOOOI	0x91
#define	IOOIOOIO	0x92
#define	IOOIOOII	0x93
#define	IOOIOIOO	0x94
#define	IOOIOIOI	0x95
#define	IOOIOIIO	0x96
#define	IOOIOIII	0x97
#define	IOOIIOOO	0x98
#define	IOOIIOOI	0x99
#define	IOOIIOIO	0x9a
#define IOOIIOII	0x9b
#define IOOIIIOO	0x9c
#define IOOIIIOI	0x9d
#define	IOOIIIIO	0x9e
#define	IOOIIIII	0x9f
#define	IOIOOOOO	0xa0
#define	IOIOOOOI	0xa1
#define	IOIOOOIO	0xa2
#define	IOIOOOII	0xa3
#define	IOIOOIOO	0xa4
#define	IOIOOIOI	0xa5
#define	IOIOOIIO	0xa6
#define	IOIOOIII	0xa7
#define	IOIOIOOO	0xa8
#define	IOIOIOOI	0xa9
#define	IOIOIOIO	0xaa
#define	IOIOIOII	0xab
#define	IOIOIIOO	0xac
#define	IOIOIIOI	0xad
#define	IOIOIIIO	0xae
#define	IOIOIIII	0xaf
#define	IOIIOOOO	0xb0
#define IOIIOOOI	0xb1
#define	IOIIOOIO	0xb2
#define	IOIIOOII	0xb3
#define	IOIIOIOO	0xb4
#define	IOIIOIOI	0xb5
#define	IOIIOIIO	0xb6
#define	IOIIOIII	0xb7
#define	IOIIIOOO	0xb8
#define	IOIIIOOI	0xb9
#define	IOIIIOIO	0xba
#define	IOIIIOII	0xbb
#define	IOIIIIOO	0xbc
#define	IOIIIIOI	0xbd
#define	IOIIIIIO	0xbe
#define	IOIIIIII	0xbf
#define	IIOOOOOO	0xc0
#define	IIOOOOOI	0xc1
#define	IIOOOOIO	0xc2
#define	IIOOOOII	0xc3
#define	IIOOOIOO	0xc4
#define	IIOOOIOI	0xc5
#define	IIOOOIIO	0xc6
#define	IIOOOIII	0xc7
#define	IIOOIOOO	0xc8
#define	IIOOIOOI	0xc9
#define	IIOOIOIO	0xca
#define	IIOOIOII	0xcb
#define	IIOOIIOO	0xcc
#define	IIOOIIOI	0xcd
#define	IIOOIIIO	0xce
#define IIOOIIII	0xcf
#define	IIOIOOOO	0xd0
#define	IIOIOOOI	0xd1
#define	IIOIOOIO	0xd2
#define	IIOIOOII	0xd3
#define	IIOIOIOO	0xd4
#define	IIOIOIOI	0xd5
#define	IIOIOIIO	0xd6
#define	IIOIOIII	0xd7
#define	IIOIIOOO	0xd8
#define	IIOIIOOI	0xd9
#define	IIOIIOIO	0xda
#define	IIOIIOII	0xdb
#define	IIOIIIOO	0xdc
#define	IIOIIIOI	0xdd
#define	IIOIIIIO	0xde
#define	IIOIIIII	0xdf
#define	IIIOOOOO	0xe0
#define	IIIOOOOI	0xe1
#define	IIIOOOIO	0xe2
#define	IIIOOOII	0xe3
#define	IIIOOIOO	0xe4
#define	IIIOOIOI	0xe5
#define	IIIOOIIO	0xe6
#define	IIIOOIII	0xe7
#define	IIIOIOOO	0xe8
#define	IIIOIOOI	0xe9
#define	IIIOIOIO	0xea
#define	IIIOIOII	0xeb
#define	IIIOIIOO	0xec
#define	IIIOIIOI	0xed
#define	IIIOIIIO	0xee
#define	IIIOIIII	0xef
#define	IIIIOOOO	0xf0
#define	IIIIOOOI	0xf1
#define	IIIIOOIO	0xf2
#define	IIIIOOII	0xf3
#define	IIIIOIOO	0xf4
#define	IIIIOIOI	0xf5
#define	IIIIOIIO	0xf6
#define	IIIIOIII	0xf7
#define	IIIIIOOO	0xf8
#define	IIIIIOOI	0xf9
#define	IIIIIOIO	0xfa
#define	IIIIIOII	0xfb
#define	IIIIIIOO	0xfc
#define	IIIIIIOI	0xfd
#define	IIIIIIIO	0xfe
#define	IIIIIIII	0xff

#endif

