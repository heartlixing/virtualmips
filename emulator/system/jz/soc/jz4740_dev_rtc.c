/*RTC. */

#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>


#include "device.h"
#include "mips64_memory.h"
#include "cpu.h"
#include "jz4740.h"



 m_uint32_t jz4740_rtc_table[JZ4740_RTC_INDEX_MAX];

struct jz4740_rtc_data {
   struct vdevice *dev;
   m_uint8_t *jz4740_rtc_ptr;
   m_uint32_t jz4740_rtc_size;
};

void *dev_jz4740_rtc_access(cpu_mips_t *cpu,struct vdevice *dev,
                     m_uint32_t offset,u_int op_size,u_int op_type,
                     m_reg_t *data,m_uint8_t *has_set_value)
{

	struct jz4740_rtc_data *d = dev->priv_data;
   cpu_log(cpu,"","rtc offset %x\n",offset);
	if (offset >= d->jz4740_rtc_size) {
      *data = 0;
      return NULL;
   }
  return((void *)(d->jz4740_rtc_ptr + offset));

}

void dev_jz4740_rtc_init_defaultvalue()
{

/*RTC_RCR (RTC_RCR_WRDY )=1 bit 7*/
jz4740_rtc_table[RTC_RCR/4] |= 0x80;
    
}
int dev_jz4740_rtc_init(vm_instance_t *vm,char *name,m_pa_t paddr,m_uint32_t len)
{
 	struct jz4740_rtc_data *d;

   /* allocate the private data structure */
   if (!(d = malloc(sizeof(*d)))) {
      fprintf(stderr,"jz4740_rtc: unable to create device.\n");
      return  (-1);
   }

   memset(d,0,sizeof(*d));
   if (!(d->dev = dev_create(name)))
     goto err_dev_create;
   d->dev->priv_data = d;
   d->dev->phys_addr = paddr;
   d->dev->phys_len = len;
   d->jz4740_rtc_ptr = (m_uint8_t*)(&jz4740_rtc_table[0]);
   d->jz4740_rtc_size = len;
   d->dev->handler   = dev_jz4740_rtc_access;
   d->dev->flags     = VDEVICE_FLAG_NO_MTS_MMAP;
   
	vm_bind_device(vm,d->dev);
	dev_jz4740_rtc_init_defaultvalue();
	
	return  (0);

   err_dev_create:
   		free(d);
   		return  (-1);
}


   


   
