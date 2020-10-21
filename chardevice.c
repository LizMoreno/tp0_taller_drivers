/* 
 * chardevice.c: Crea un modulo que se le envia datos y se los puede leer luego. 
 *
 * Basado desde TLDP.org's LKMPG book.
 */


#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h> /* for put_user */

int init_module(void);
void cleanup_module(void);
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);

#define SUCCESS 0
#define DEVICE_NAME "UNGS"
#define BUF_LEN 80


/*Las variables globales se declaran como static, por lo que son globales dentro del archivo. */

static int Major; /*El FS para saber que driver usar necesita un Major Number*/
static int Device_Open = 0;
static char msg[BUF_LEN];
//static int msg_length = 0;

static char *msg_Ptr; 

static struct file_operations fops = { 
	.owner = THIS_MODULE,
	.read = device_read,
	.write = device_write,
	.open = device_open,
        .release = device_release
};

/*
 * Esta función se llama cuando se carga el módulo */

int init_module(void)
{
    Major = register_chrdev(0, DEVICE_NAME, &fops);

    if (Major < 0) {
        printk(KERN_ALERT "Registrando char device con %d\n", Major);
        return Major;
    }
 
    printk(KERN_INFO "Tengo major number %d. Hablale al driver :), ", Major);
    printk(KERN_INFO "crear un dev_file con \n");
    printk(KERN_INFO "sudo mknod /dev/%s c %d 0\n", DEVICE_NAME, Major); 
    printk(KERN_INFO "Dale permiso así:\n sudo chmod 666 /dev/%s\n", DEVICE_NAME);
    printk(KERN_INFO "Probá varios minor numbers. Probar cat y echo\n");
    printk(KERN_INFO "al device file.\n");
    printk(KERN_INFO "Eliminar el archivo y el módulo del dispositivo cuando haya terminado...\n");
    printk(KERN_INFO "sudo rm /dev/%s\n", DEVICE_NAME); 

    return SUCCESS;
}

/* Esta función se llama cuando el módulo se descarga */

void cleanup_module(void)
{
    /*
     * Unregister the device
     */
    unregister_chrdev(Major, DEVICE_NAME);
}

/*
 * Methods
 */

/*
 * Se llama cuando un proceso intenta abrir el archivo del dispositivo, como
 * "cat /dev/chardevice
 */
static int device_open(struct inode *inode, struct file *filp)
{
	static int counter = 0;

	if (Device_Open)
		return -EBUSY;

	Device_Open++;
	sprintf(msg, "Ya te dije %d veces ¡Hola mundo!\n",	counter++);
		msg_Ptr = msg;
		try_module_get(THIS_MODULE);
	    return SUCCESS;
}

/* Se invoca cuando un proceso cierra el archivo del dispositivo. */

static int device_release(struct inode *inode, struct file *filp)
{
	Device_Open--;/* Ahora estamos listas para nuestra próxima llamada */

/** decremento el contador, o una vez que abrió el archivo,  nunca te deshagas del módulo.
*/
	module_put(THIS_MODULE);

	return 0;
    
}

/* Llamada cuando un proceso, que ya abrió el archivo dev, intenta leer de eso. */

static ssize_t device_read(struct file *filp, /* ver include / linux / fs.h */
                           char *buffer,      /* buffer para llenar con datos */
                           size_t length,     /* longitud del búfer */
                           loff_t *offset) //
{
/* Número de bytes realmente escritos en el búfer */
	int bytes_read = 0;

	/* Si estamos al final del mensaje, devuelve 0 que significa el final del archivo */

	if (*msg_Ptr == 0)
		 return 0;

	/* coloca los datos en el búfer */
	while (length && *msg_Ptr) {
		/* El búfer está en el segmento de datos del usuario, no en el núcleo
		* segmento, por lo que la asignación "*" no funcionará. Tenemos que usar
		* put_user que copia datos del segmento de datos del kernel a
		* el segmento de datos del usuario. */

		put_user(*(msg_Ptr++), buffer++);
		length--;
		bytes_read++;
	}
/* La mayoría de las funciones de lectura devuelven el número de bytes puestos en el búfer */

    return bytes_read;
}

/* Llamada cuando un proceso escribe en el archivo de desarrollo: echo "hi" > /dev/UNGS */

static ssize_t device_write(struct file *filp, const char *tmp, size_t length, loff_t *offset) { 

/*es muy importante verificar cuántos bytes envía el usuario y cuántos bytes podemos aceptar.

La función copy_from_user devuelve el número de bytes que no se pudieron copiar. En caso de éxito, esto será cero.
Si no se pudieron copiar algunos datos, esta función rellenará los datos copiados al tamaño solicitado usando cero bytes*/

   size_t maxdatalen = 30, ncopied;
   uint8_t databuf[maxdatalen]; 

   if (length < maxdatalen) {
        maxdatalen = length;
    }

    ncopied = copy_from_user(databuf, tmp, maxdatalen);

    if (ncopied == 0) {
        printk("Copió %zd bytes del usuario\n", maxdatalen);
    } else {
        printk("No se pudo copiar %zd bytes del usuario\n", ncopied);
    }

    databuf[maxdatalen] = 0;

    printk("data para usuario: %s\n", databuf);

    return length;

}

