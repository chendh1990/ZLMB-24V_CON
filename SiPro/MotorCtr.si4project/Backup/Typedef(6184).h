#ifndef __TYPEDEF_H__
#define __TYPEDEF_H__

typedef bit                   BOOL;
typedef bit                   bool;
typedef unsigned char         UINT8;
typedef unsigned char         uint8;
typedef unsigned int          UINT16;
typedef unsigned int          uint16;
typedef unsigned long         UINT32;
typedef unsigned long         uint32;

typedef unsigned char         uint8_t;
typedef unsigned int          uint16_t;
typedef unsigned long         uint32_t;


typedef unsigned char sysServerTO_t;
typedef sysServerTO_t (*sysServer_t)(void);



#ifndef BIT
#define BIT(x)		(1<<(x))
#endif

#ifndef TRUE
#define	 TRUE		1
#endif
#ifndef FALSE
#define	 FALSE		0
#endif

#ifndef true
#define	 true		1
#endif
#ifndef false
#define	 false		0
#endif

#ifndef HIGH
#define	 HIGH		1
#endif

#ifndef LOW
#define	 LOW		1
#endif


#ifndef SUCCESS
#define         SUCCESS             1
#endif

#ifndef ERROR
#define         ERROR               0
#endif

#ifndef ENABLE
#define         ENABLE                1
#endif
//
#ifndef DISABLE
#define         DISABLE               0
#endif


#ifndef NULL
#define NULL		((void *)0)
#endif
#ifndef null
#define null 		NULL
#endif

#ifndef XDATA
#define XDATA 		xdata
#endif


#define BUILD_UINT32(Byte0, Byte1, Byte2, Byte3) \
          ((uint32)((uint32)((Byte0) & 0x00FF) \
          + ((uint32)((Byte1) & 0x00FF) << 8) \
          + ((uint32)((Byte2) & 0x00FF) << 16) \
          + ((uint32)((Byte3) & 0x00FF) << 24)))

#define BUILD_UINT16(loByte, hiByte) ((uint16)(((loByte) & 0x00FF) + (((hiByte) & 0x00FF) << 8)))
#define HI_UINT16(a) (((a) >> 8) & 0xFF)
#define LO_UINT16(a) ((a) & 0xFF)

#define BUILD_UINT8(hiByte, loByte) ((uint8)(((loByte) & 0x0F) + (((hiByte) & 0x0F) << 4)))
#define HI_UINT8(a) (((a) >> 4) & 0x0F)
#define LO_UINT8(a) ((a) & 0x0F)

#endif
