#include "clk.h"

void setUp16MhzExternalOsc()
{
        PORTD_DIR = 0x01;
        //16MHz external crystal
        OSC_XOSCCTRL = OSC_FRQRANGE_12TO16_gc | OSC_XOSCSEL_XTAL_16KCLK_gc;
        //Enable external oscillator
        OSC_CTRL |= OSC_XOSCEN_bm;
        //Wait for clock stabilization
        while(!(OSC_STATUS & OSC_XOSCRDY_bm));
        // Selects clock system as external clock
        // through change protection mechanism
        CCP = CCP_IOREG_gc;
        CLK_CTRL = CLK_SCLKSEL_XOSC_gc;
}

void setUp32MhzInternalOsc()
{
    OSC_CTRL |= OSC_RC32MEN_bm; //Setup 32Mhz crystal

    while(!(OSC_STATUS & OSC_RC32MRDY_bm));

    CCP = CCP_IOREG_gc; //Trigger protection mechanism
    CLK_CTRL = CLK_SCLKSEL_RC32M_gc; //Enable internal  32Mhz crystal
}

void ClkInit()
{
	setUp32MhzInternalOsc();
}
