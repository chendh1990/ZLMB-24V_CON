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

#ifndef NULL
#define NULL		((void *)0)
#endif
#ifndef null
#define null 		NULL
#endif

#ifndef XDATA
#define XDATA 		xdata
#endif

#endif
