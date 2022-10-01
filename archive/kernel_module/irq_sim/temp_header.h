#ifndef _TEMP_HEADER
#define _TEMP_HEADER

#include <linux/irq.h>
#include <linux/irq_sim.h>
#include <linux/irq_work.h>
#include <linux/interrupt.h>
#include <linux/slab.h>

struct irq_sim_work_ctx {
	struct irq_work		work;
	int			irq_base;
	unsigned int		irq_count;
	unsigned long		*pending;
	struct irq_domain	*domain;
};

struct irq_sim_irq_ctx {
	int			irqnum;
	bool			enabled;
	struct irq_sim_work_ctx	*work_ctx;
};

static void irq_sim_handle_irq(struct irq_work *work);

#endif