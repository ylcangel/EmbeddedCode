#include "global.h"

/*System ticks*/
static volatile unsigned long xdata g_sys_ticks;
volatile unsigned int xdata g_high_timer;

/*Timer 0 as system tick timer. Freely running.*/
void time_Initialization(void)
{
	/*Timer0 at 16-bit timer mode.*/
	TMOD = (TMOD & 0xf0u) | 0x01u;
	TL0 = 0u;
	TH0 = 0u; 
	ET0 = 1; /*Enable timer0 interrupt.*/
	TR0 = 1; /*Start running.*/
}

void timer0_ISR(void) interrupt 1
{
	g_high_timer++;
}

void tick_Task(void)
{
	static unsigned long idata old_count;
	unsigned long count;
	unsigned int timer;
	unsigned char th0,tl0;
	static unsigned char idata old_th0;
	
	/*to prevent program to executed too frequently.*/
	th0 = TH0;
	//if(th0 - old_th0 < 5) return;
	old_th0 = th0;
	
	/*Get the snapshot of current high_timer and TH0/TL0 */
	TR0 = 0;   /*Stop timer0*/
	__disable_irq();
	tl0 = TL0;
	th0 = TH0;
	timer = g_high_timer;
	if(TF0) timer++;
	__enable_irq();
	TR0 = 1;
	/*organize snapshot to a 32-bit value.*/
	count = timer;
	count <<= 16;
	timer = th0;
	timer <<= 8;
	timer |= tl0;
	count |= timer;
	/*To ignore overflow condition, which occurs every 77 minutes.*/
	if(count < old_count)
	{
		old_count = count;
		return;
	}
	/*update system ticks, which stand for elapsed running Milliseconds.*/
#if (CFG_SYSFREQ == 11059200UL)
	for( ; count - old_count > 922; old_count += 922) g_sys_ticks++;
#elif (CFG_SYSFREQ == 22118400UL)
	for( ; count - old_count > 1844; old_count += 1844) g_sys_ticks++;
#else 
	#error "Must have a choice."
#endif 
}

unsigned long time_GetTicks(void)
{
	return g_sys_ticks;
}

//----------------------------------------------------------------------------------
static void Delay1us()		//@24.000MHz
{
	unsigned char i;

	i = 6;
	while (--i);
}

static void Delay10us()		//@24.000MHz
{
	unsigned char i;

	i = 78;
	while (--i);
}


static void Delay100us()		//@24.000MHz
{
	unsigned char i, j;

	i = 4;
	j = 27;
	do
	{
		while (--j);
	} while (--i);
}


static void Delay1ms()		//@24.000MHz
{
	unsigned char i, j;

	_nop_();
	i = 32;
	j = 40;
	do
	{
		while (--j);
	} while (--i);
}
//----------------------------------------------------------------------------------
void time_HoggingDelayMs(unsigned int ms)
{
	for(;ms;ms--)
	{
		Delay1ms();
	}
}

void time_HoggingDelayUs(unsigned int us)
{
	for(;us>=100;us-=100) Delay100us();
	for(;us>=10;us-=10) Delay10us();
	for(;us>0;us--) Delay1us();
}
