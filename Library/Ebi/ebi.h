#ifndef _EBI_H_
#define _EBI_H_

#include <avr/io.h>

#define BIT0  (1 << 0)
#define BIT1  (1 << 1)
#define BIT2  (1 << 2)
#define BIT3  (1 << 3)
#define BIT4  (1 << 4)
#define BIT5  (1 << 5)
#define BIT6  (1 << 6)
#define BIT7  (1 << 7)
#define BIT8  (1 << 8)
#define BIT9  (1 << 9)
#define BIT10 (1 << 10)
#define BIT11 (1 << 11)
#define BIT12 (1 << 12)
#define BIT13 (1 << 13)
#define BIT14 (1 << 14)
#define BIT15 (1 << 15)

#define EBI_PORT_WE          BIT0
#define EBI_PORT_RE          BIT1
#define EBI_PORT_ALE1        BIT2
#define EBI_PORT_ALE2        BIT3
#define EBI_PORT_CS0         BIT4
#define EBI_PORT_CS1         BIT5
#define EBI_PORT_CS2         BIT6
#define EBI_PORT_CS3         BIT7
#define EBI_PORT_CS_ALL      (EBI_PORT_CS3 | EBI_PORT_CS2 | EBI_PORT_CS1 | EBI_PORT_CS0)
//#define EBI_PORT_ALE1        BIT1
//#define EBI_PORT_ALE12       (BIT2 | BIT1)

typedef enum {
	EbiCs0   = 0,
	EbiCs1   = 1,
	EbiCs2   = 2,
	EbiCs3   = 3,
	EbiCsAll = 4,
	EbiMaxCs
} EbiCsPinSelect;

typedef enum {
	EbiAle1  = 0,
	EbiAle12 = 2,
	EbiNoAle = 3
} EbiAleSelect;

typedef enum {
	Ebi2PortMode = 0,
	Ebi3PortMode = 1
} EbiPortMode;

typedef enum {
	EbiSramMode    = 0,
	EbiSramLpcMode = 1
} EbiMode;

typedef struct {
	EbiCsPinSelect   Cs;
	EbiAleSelect     Ale;
	uint8_t          NumberOfAddressLines;
	EbiPortMode      PortMode;
	EbiMode          Mode;
} EbiConfiguration;

void EbiInit();

#endif /*_EBI_H_*/
