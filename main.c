#include "Library/Usart/usart.h"
#include "Library/Ebi/ebi.h"
#include "Library/Clk/clk.h"
#include "Library/Debug/debug.h"

#include <util/delay.h>

void BoardInit()
{
	EbiInit();
}

int main( void )
{
	// ClkInit is required for correct working all pheripherals
	ClkInit();
	UsartInit();
	DEBUG ("START");
	BoardInit();

	int i=0;

	while(1) {
		i++;
		_delay_ms(1000);
		DEBUG ("dsa %s %d %s %d\n\r", __func__, __LINE__, __FILE__ , i );
		DEBUG ("%s \n\r", __BASE_FILE__);
	}
}
