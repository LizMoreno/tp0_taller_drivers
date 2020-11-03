#include <linux/module.h>
#include <linux/kernel.h>

int init_module(void){ /* Constructor */
    printk ( KERN_INFO "Mi primer driver: Driver registrado \n") ;
    return 0;
}

void cleanup_module(void){ /* Destructor */
    printk ( KERN_INFO "Mi primer driver: Driver desregistrado \n") ;
}

MODULE_LICENSE ("GPL") ;
MODULE_AUTHOR ("UNGS-Liz Moreno") ;
MODULE_DESCRIPTION ("Mi primer driver :)") ;
