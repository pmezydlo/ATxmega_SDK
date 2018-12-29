#include "usart.h"
#include <avr/pgmspace.h>

uint8_t usart_set_baudrate(USART_t * const usart, uint32_t baud, uint32_t cpu_hz)
{
	int8_t exp;
	uint32_t div;
	uint32_t limit;
	uint32_t ratio;
	uint32_t min_rate;
	uint32_t max_rate;

	/*
	 * Check if the hardware supports the given baud rate
	 */
	/* 8 = (2^0) * 8 * (2^0) = (2^BSCALE_MIN) * 8 * (BSEL_MIN) */
	max_rate = cpu_hz / 8;
	/* 4194304 = (2^7) * 8 * (2^12) = (2^BSCALE_MAX) * 8 * (BSEL_MAX+1) */
	min_rate = cpu_hz / 4194304;

	if (!((usart)->CTRLB & USART_CLK2X_bm)) {
		max_rate /= 2;
		min_rate /= 2;
	}

	if ((baud > max_rate) || (baud < min_rate)) {
		return 0;
	}

	/* Check if double speed is enabled. */
	if (!((usart)->CTRLB & USART_CLK2X_bm)) {
		baud *= 2;
	}

	/* Find the lowest possible exponent. */
	limit = 0xfffU >> 4;
	ratio = cpu_hz / baud;

	for (exp = -7; exp < 7; exp++) {
		if (ratio < limit) {
			break;
		}

		limit <<= 1;

		if (exp < -3) {
			limit |= 1;
		}
	}

	/*
	 * Depending on the value of exp, scale either the input frequency or
	 * the target baud rate. By always scaling upwards, we never introduce
	 * any additional inaccuracy.
	 *
	 * We are including the final divide-by-8 (aka. right-shift-by-3) in
	 * this operation as it ensures that we never exceeed 2**32 at any
	 * point.
	 *
	 * The formula for calculating BSEL is slightly different when exp is
	 * negative than it is when exp is positive.
	 */
	if (exp < 0) {
		/* We are supposed to subtract 1, then apply BSCALE. We want to
		 * apply BSCALE first, so we need to turn everything inside the
		 * parenthesis into a single fractional expression.
		 */
		cpu_hz -= 8 * baud;

		/* If we end up with a left-shift after taking the final
		 * divide-by-8 into account, do the shift before the divide.
		 * Otherwise, left-shift the denominator instead (effectively
		 * resulting in an overall right shift.)
		 */
		if (exp <= -3) {
			div = ((cpu_hz << (-exp - 3)) + baud / 2) / baud;
		} else {
			baud <<= exp + 3;
			div = (cpu_hz + baud / 2) / baud;
		}
	} else {
		/* We will always do a right shift in this case, but we need to
		 * shift three extra positions because of the divide-by-8.
		 */
		baud <<= exp + 3;
		div = (cpu_hz + baud / 2) / baud - 1;
	}

	(usart)->BAUDCTRLB = (uint8_t)(((div >> 8) & 0X0F) | (exp << 4));
	(usart)->BAUDCTRLA = (uint8_t)div;

	return 1;
}

void USART_putchar(USART_t * const usart, char ch)
{
	while(!(usart->STATUS & USART_DREIF_bm));
	USARTC0.DATA=ch;
}

void USART_send(USART_t * const usart, const char *txt)
{
	while(*txt)
	{
		USART_putchar(usart, *txt);
		++txt;
	}
}

void USART_send_F(USART_t * const usart, const char *txt)
{
	char tmp;
	while((tmp=pgm_read_byte(txt)))
	{
		USART_putchar(usart, tmp);
		++txt;
	}
}

void USART_send_block(USART_t * const usart, const uint8_t *block, uint8_t size)
{
	while(size--)
	{
		USART_putchar(usart, *block);
		++block;
	}
}

void USART_send_block_F(USART_t * const usart, const uint8_t *block, uint8_t size)
{
	while(size--)
	{
		USART_putchar(usart, pgm_read_byte(block));
		++block;
	}
}

void UsartInit() {
	PORTC_OUTSET = PIN3_bm;
	PORTC_DIRSET = PIN3_bm;
	USARTC0.CTRLB = USART_TXEN_bm | USART_RXEN_bm;
	USARTC0.CTRLC = USART_CHSIZE_8BIT_gc;
	//UsartSetBaudrate(&USARTC0, 9600, F_CPU);
	usart_set_baudrate(&USARTC0, 115200, F_CPU);
}

int get(FILE *stream)
{
	while(!(USARTC0_STATUS & USART_RXCIF_bm));
	return USARTC0_DATA;
}

int put(char c, FILE *stream)
{
	while(!(USARTC0_STATUS & USART_DREIF_bm));
	USARTC0_DATA=c;
	return 0;
}
