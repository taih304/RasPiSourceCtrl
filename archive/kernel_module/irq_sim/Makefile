TARGET = irq_sim_test
irq_sim_test-objs := irq_sim.o test_irq.o
obj-m += $(TARGET).o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
