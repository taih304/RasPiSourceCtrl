#include <linux/module.h>
#include <linux/kernel.h>

#include <linux/irqdomain.h>
#include <linux/irq.h>
#include <linux/irq_sim.h>
#include <linux/irq_work.h>
#include <linux/interrupt.h>
#include "temp_header.h"

MODULE_LICENSE("GPL");

#define IRQ 		1
#define IRQ_BASE 	2
#define FLAG		0

struct irq_sim_irq_ctx 	irq_sim_ctx;
struct irq_work			irq_sim_work;
struct irq_sim_work_ctx	irq_sim_work_ctx_obj;
// struct irq_sim *sim;

// unsigned int num_irqs;

void irq_handler(struct irq_work *work){
	printk("IRQ sim handler\n");
}

int init_module(void)
{
	irq_sim_ctx.irqnum = IRQ;
	irq_sim_ctx.enabled = true;

	// irq_sim_work.flags = FLAG;
	irq_sim_work.func = irq_handler;

	unsigned long a = 0;
	unsigned long		*pending = &a;
	irq_sim_work_ctx_obj.work = irq_sim_work;
	irq_sim_work_ctx_obj.pending = pending;

	// sim = (struct irq_sim *) kmalloc(sizeof(struct irq_sim), GFP_KERNEL);
	// sim->work_ctx = irq_sim_work_ctx_obj;
	// sim->irqs = &irq_sim_ctx;
    irq_sim_handle_irq(&irq_sim_work);
	#define	NUM_IRQS	10
	// irq_sim_init(sim, NUM_IRQS);

	printk(KERN_INFO "irq_sim_init(sim, NUM_IRQS) passed\n");
	return 0;
}

void cleanup_module(void)
{
	printk(KERN_INFO "clean up module\n");
}

// module_init(init_module);
// module_exit(cleanup_module);