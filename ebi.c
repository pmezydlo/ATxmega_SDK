#include "ebi.h"

void SetEbiMode(EbiConfiguration *EbiConf)
{
	uint8_t Mode = 0;

	if (EbiConf->PortMode == Ebi3PortMode) {
		Mode |= EBI_IFMODE_3PORT_gc;
	} else {
		Mode |= EBI_IFMODE_2PORT_gc;
	}

	if (EbiConf->Mode == EbiSramMode) {
		switch (EbiConf->Ale) {
		case EbiAle1:
			Mode |= EBI_SRMODE_ALE1_gc;
			break;
		case EbiAle12:
			Mode |= EBI_SRMODE_ALE12_gc;
			break;
		case EbiNoAle:
		default:
			Mode |= EBI_SRMODE_NOALE_gc;
			break;
		}
	} else {
		// here set lpcmode
	}
}

void SetEbiPort(EbiConfiguration *EbiConf)
{
	uint8_t PortMask = 0;

	// set initial port H state
	PortMask = (EBI_PORT_WE | EBI_PORT_RE);
	if (EbiConf->Cs == EbiCs0) {
		PortMask |= EBI_PORT_CS0;
	}
	if (EbiConf->Cs == EbiCs1) {
	}

	// set initial port H direction
	PortMask = (EBI_PORT_WE | EBI_PORT_RE);
	switch (EbiConf->Ale) {
	case EbiAle1:
		PortMask |= EBI_PORT_ALE1;
	case EbiAle12:
		PortMask |= (EBI_PORT_ALE1 | EBI_PORT_ALE2);
		break;
	case EbiNoAle:
	default:
		break;
	}
	PORTH.DIR = PortMask;

	// set portk
	if (EbiConf->PortMode == Ebi3PortMode) {
		PortMask = 0;
		if (EbiConf->Mode == EbiSramMode) {
			if (EbiConf->NumberOfAddressLines < 8) {
				PortMask = (1 << EbiConf->NumberOfAddressLines) - 1;
			} else {
				PortMask = 0xFF;
			}
		} else {
			if (EbiConf->NumberOfAddressLines <= 8) {
				PortMask = 0x00;
			} else if (EbiConf->NumberOfAddressLines < 16) {
				PortMask = (1 << (EbiConf->NumberOfAddressLines - 8)) -1;
			} else {
				PortMask = 0xFF;
			}
		}
		PORTK.DIR = PortMask;
	}
}


void EbiInit()
{
	// setup EBI port
}

