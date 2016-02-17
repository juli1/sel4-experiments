#include <stdio.h>

//#include <platsupport/mach/epit.h>
#include <platsupport/timer.h>
#include <platsupport/plat/timer.h>
#include <sel4platsupport/plat/timer.h>
#include <sel4platsupport/timer.h>
#include <sel4utils/util.h>


#include <Timer.h>


#define INTERRUPT DMTIMER2_INTERRUPT

pstimer_t *timer_drv = NULL;

volatile uint64_t ticks = 0;
uint64_t period = 1;
void irq_callback(void *_ UNUSED)
{
    dm_handle_irq(timer_drv, INTERRUPT);

    printf ("received one int\n");
 
    ticks++;
		Prod_Pinger_activator_emit();
		Cons_PingMe_activator_emit();
    
    irq_reg_callback(irq_callback, NULL);
//    timer_oneshot_relative(timer_drv,period);
    dm_oneshot_relative(timer_drv, period);
   }

int run (void)
{
    printf ("starts timer\n");
    /*
    epit_config_t config;
    
 yy   config.vaddr = (void*)reg;
    config.irq = INTERRUPT;
    config.prescaler = 0;
    */
    
//    timer_drv = epit_get_timer(&config);

    timer_config_t config;
    config.irq = INTERRUPT;
    config.vaddr = (void*)DMTIMER2_PADDR;
 //   config.prescaler = 0;
    
    timer_drv = ps_get_timer (DMTIMER2, &config);
    assert(timer_drv);

    irq_reg_callback(irq_callback, NULL);

    dm_oneshot_relative(timer_drv, period);

}
