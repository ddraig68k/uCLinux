/*
 * drivers/i2c/i2c-ibm_iic.c
 *
 * Support for the IIC peripheral on IBM PPC 4xx
 *
 * Copyright (c) 2003 Zultys Technologies.
 * Eugene Surovegin <eugene.surovegin@zultys.com> or <ebs@ebshome.net>
 *
 * Based on original work by 
 * 	Ian DaSilva  <idasilva@mvista.com>
 *      Armin Kuster <akuster@mvista.com>
 * 	Matt Porter  <mporter@mvista.com>
 *
 *      Copyright 2000-2003 MontaVista Software Inc.
 *
 * Original driver version was highly leveraged from i2c-elektor.c
 *
 *   	Copyright 1995-97 Simon G. Vogl
 *                1998-99 Hans Berglund
 *
 *   	With some changes from Ky?sti M?lkki <kmalkki@cc.hut.fi> 
 *	and even Frodo Looijaard <frodol@dds.nl>
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
 */

#include <linux/config.h>
#ifdef CONFIG_I2C_DEBUG_BUS
#define DEBUG	1
#endif

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/ioport.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <linux/i2c.h>
#include <linux/i2c-id.h>
#include <asm/ocp.h>
#include <asm/ibm4xx.h>

#include "i2c-ibm_iic.h"

#define DRIVER_VERSION "2.0"

MODULE_DESCRIPTION("IBM IIC driver v" DRIVER_VERSION);
MODULE_LICENSE("GPL");

static int iic_force_poll = 0;
MODULE_PARM(iic_force_poll, "i");
MODULE_PARM_DESC(iic_force_poll, "Force polling mode");

static int iic_force_fast = 0;
MODULE_PARM(iic_force_fast, "i");
MODULE_PARM_DESC(iic_fast_poll, "Force fast mode (400 kHz)");

#define DBG_LEVEL 0

#ifdef DBG
#undef DBG
#endif

#ifdef DBG2
#undef DBG2
#endif

#if DBG_LEVEL > 0
#  define DBG(x...)	printk(KERN_DEBUG "ibm-iic" ##x)
#else
#  define DBG(x...)	((void)0)
#endif
#if DBG_LEVEL > 1
#  define DBG2(x...) 	DBG( ##x )
#else
#  define DBG2(x...) 	((void)0)
#endif
#if DBG_LEVEL > 2
static void dump_iic_regs(const char* header, struct ibm_iic_private* dev)
{
	volatile struct iic_regs *iic = dev->vaddr;
	printk(KERN_DEBUG "ibm-iic%d: %s\n", dev->idx, header);
	printk(KERN_DEBUG "  cntl     = 0x%02x, mdcntl = 0x%02x\n"
	       KERN_DEBUG "  sts      = 0x%02x, extsts = 0x%02x\n"
	       KERN_DEBUG "  clkdiv   = 0x%02x, xfrcnt = 0x%02x\n"
	       KERN_DEBUG "  xtcntlss = 0x%02x, directcntl = 0x%02x\n",
		in_8(&iic->cntl), in_8(&iic->mdcntl), in_8(&iic->sts), 
		in_8(&iic->extsts), in_8(&iic->clkdiv), in_8(&iic->xfrcnt), 
		in_8(&iic->xtcntlss), in_8(&iic->directcntl));
}
#  define DUMP_REGS(h,dev)	dump_iic_regs((h),(dev))
#else
#  define DUMP_REGS(h,dev)	((void)0)
#endif

/* Enable/disable interrupt generation */
static inline void iic_interrupt_mode(struct ibm_iic_private* dev, int enable)
{
	out_8(&dev->vaddr->intmsk, enable ? INTRMSK_EIMTC : 0);
}
 
/*
 * Initialize IIC interface.
 */
static void iic_dev_init(struct ibm_iic_private* dev)
{
	volatile struct iic_regs *iic = dev->vaddr;

	DBG("%d: init\n", dev->idx);
	
	/* Clear master address */
	out_8(&iic->lmadr, 0);
	out_8(&iic->hmadr, 0);

	/* Clear slave address */
	out_8(&iic->lsadr, 0);
	out_8(&iic->hsadr, 0);

	/* Clear status & extended status */
	out_8(&iic->sts, STS_SCMP | STS_IRQA);
	out_8(&iic->extsts, EXTSTS_IRQP | EXTSTS_IRQD | EXTSTS_LA
			    | EXTSTS_ICT | EXTSTS_XFRA);

	/* Set clock divider */
	out_8(&iic->clkdiv, dev->clckdiv);

	/* Clear transfer count */
	out_8(&iic->xfrcnt, 0);

	/* Clear extended control and status */
	out_8(&iic->xtcntlss, XTCNTLSS_SRC | XTCNTLSS_SRS | XTCNTLSS_SWC
			    | XTCNTLSS_SWS);

	/* Clear control register */
	out_8(&iic->cntl, 0);
	
	/* Enable interrupts if possible */
	iic_interrupt_mode(dev, dev->irq >= 0);

	/* Set mode control */
	out_8(&iic->mdcntl, MDCNTL_FMDB | MDCNTL_EINT | MDCNTL_EUBS
			    | (dev->fast_mode ? MDCNTL_FSM : 0));

	DUMP_REGS("iic_init", dev);
}

/* 
 * Reset IIC interface
 */
static void iic_dev_reset(struct ibm_iic_private* dev)
{
	volatile struct iic_regs *iic = dev->vaddr;
	int i;
	u8 dc;
	
	DBG("%d: soft reset\n", dev->idx);
	DUMP_REGS("reset", dev);
	
    	/* Place chip in the reset state */
	out_8(&iic->xtcntlss, XTCNTLSS_SRST);
	
	/* Check if bus is free */
	dc = in_8(&iic->directcntl);	
	if (!DIRCTNL_FREE(dc)){
		DBG("%d: trying to regain bus control\n", dev->idx);
	
		/* Try to set bus free state */
		out_8(&iic->directcntl, DIRCNTL_SDAC | DIRCNTL_SCC);	
	
		/* Wait until we regain bus control */
		for (i = 0; i < 100; ++i){
			dc = in_8(&iic->directcntl);
			if (DIRCTNL_FREE(dc))
				break;
			
			/* Toggle SCL line */
			dc ^= DIRCNTL_SCC;
			out_8(&iic->directcntl, dc);
			udelay(10);
			dc ^= DIRCNTL_SCC;
			out_8(&iic->directcntl, dc);
			
			/* be nice */
			cond_resched();
		}
	}
	
	/* Remove reset */
	out_8(&iic->xtcntlss, 0);
	
	/* Reinitialize interface */
	iic_dev_init(dev);
}

/*
 * IIC interrupt handler
 */
static irqreturn_t iic_handler(int irq, void *dev_id, struct pt_regs *regs)
{
	struct ibm_iic_private* dev = (struct ibm_iic_private*)dev_id;
	volatile struct iic_regs* iic = dev->vaddr;
	
	DBG2("%d: irq handler, STS = 0x%02x, EXTSTS = 0x%02x\n", 
	     dev->idx, in_8(&iic->sts), in_8(&iic->extsts));
	
	/* Acknowledge IRQ and wakeup iic_wait_for_tc */
	out_8(&iic->sts, STS_IRQA | STS_SCMP);
	wake_up_interruptible(&dev->wq);
	
	return IRQ_HANDLED;
}

/*
 * Get master transfer result and clear errors if any.
 * Returns the number of actually transferred bytes or error (<0)
 */
static int iic_xfer_result(struct ibm_iic_private* dev)
{
	volatile struct iic_regs *iic = dev->vaddr;	
	
	if (unlikely(in_8(&iic->sts) & STS_ERR)){
		DBG("%d: xfer error, EXTSTS = 0x%02x\n", dev->idx, 
			in_8(&iic->extsts));
				
		/* Clear errors and possible pending IRQs */
		out_8(&iic->extsts, EXTSTS_IRQP | EXTSTS_IRQD | 
			EXTSTS_LA | EXTSTS_ICT | EXTSTS_XFRA);
			
		/* Flush master data buffer */
		out_8(&iic->mdcntl, in_8(&iic->mdcntl) | MDCNTL_FMDB);
		
		/* Is bus free?
		 * If error happened during combined xfer
		 * IIC interface is usually stuck in some strange
		 * state, the only way out - soft reset.
		 */
		if ((in_8(&iic->extsts) & EXTSTS_BCS_MASK) != EXTSTS_BCS_FREE){
			DBG("%d: bus is stuck, resetting\n", dev->idx);
			iic_dev_reset(dev);
		}
		return -EREMOTEIO;
	}
	else
		return in_8(&iic->xfrcnt) & XFRCNT_MTC_MASK;
}

/*
 * Try to abort active transfer.
 */
static void iic_abort_xfer(struct ibm_iic_private* dev)
{
	volatile struct iic_regs *iic = dev->vaddr;
	unsigned long x;
	
	DBG("%d: iic_abort_xfer\n", dev->idx);
	
	out_8(&iic->cntl, CNTL_HMT);
	
	/*
	 * Wait for the abort command to complete.
	 * It's not worth to be optimized, just poll (timeout >= 1 tick)
	 */
	x = jiffies + 2;
	while ((in_8(&iic->extsts) & EXTSTS_BCS_MASK) != EXTSTS_BCS_FREE){
		if (time_after(jiffies, x)){
			DBG("%d: abort timeout, resetting...\n", dev->idx);
			iic_dev_reset(dev);
			return;
		}
		schedule();
	}

	/* Just to clear errors */
	iic_xfer_result(dev);
}

/*
 * Wait for master transfer to complete.
 * It puts current process to sleep until we get interrupt or timeout expires.
 * Returns the number of transferred bytes or error (<0)
 */
static int iic_wait_for_tc(struct ibm_iic_private* dev){
	
	volatile struct iic_regs *iic = dev->vaddr;
	int ret = 0;
	
	if (dev->irq >= 0){
		/* Interrupt mode */
		wait_queue_t wait;
    		init_waitqueue_entry(&wait, current);
		
		add_wait_queue(&dev->wq, &wait);
		set_current_state(TASK_INTERRUPTIBLE);
		if (in_8(&iic->sts) & STS_PT)
			schedule_timeout(dev->adap.timeout * HZ);
		set_current_state(TASK_RUNNING);
		remove_wait_queue(&dev->wq, &wait);
		
		if (unlikely(signal_pending(current))){
			DBG("%d: wait interrupted\n", dev->idx);
			ret = -ERESTARTSYS;
		} else if (unlikely(in_8(&iic->sts) & STS_PT)){
			DBG("%d: wait timeout\n", dev->idx);
			ret = -ETIMEDOUT;
		}
	}
	else {
		/* Polling mode */
		unsigned long x = jiffies + dev->adap.timeout * HZ;
		
		while (in_8(&iic->sts) & STS_PT){
			if (unlikely(time_after(jiffies, x))){
				DBG("%d: poll timeout\n", dev->idx);
				ret = -ETIMEDOUT;
				break;
			}
		
			if (unlikely(signal_pending(current))){
				DBG("%d: poll interrupted\n", dev->idx);
				ret = -ERESTARTSYS;
				break;
			}
			schedule();
		}	
	}
	
	if (unlikely(ret < 0))
		iic_abort_xfer(dev);
	else
		ret = iic_xfer_result(dev);
	
	DBG2("%d: iic_wait_for_tc -> %d\n", dev->idx, ret);
	
	return ret;
}

/*
 * Low level master transfer routine
 */
static int iic_xfer_bytes(struct ibm_iic_private* dev, struct i2c_msg* pm, 
			  int combined_xfer)
{
	volatile struct iic_regs *iic = dev->vaddr;
	char* buf = pm->buf;
	int i, j, loops, ret = 0;
	int len = pm->len;

	u8 cntl = (in_8(&iic->cntl) & CNTL_AMD) | CNTL_PT;
	if (pm->flags & I2C_M_RD)
		cntl |= CNTL_RW;
	
	loops = (len + 3) / 4;
	for (i = 0; i < loops; ++i, len -= 4){
		int count = len > 4 ? 4 : len;
		u8 cmd = cntl | ((count - 1) << CNTL_TCT_SHIFT);
		
		if (!(cntl & CNTL_RW))
			for (j = 0; j < count; ++j)
				out_8((volatile u8*)&iic->mdbuf, *buf++);
		
		if (i < loops - 1)
			cmd |= CNTL_CHT;
		else if (combined_xfer)
			cmd |= CNTL_RPST;
		
		DBG2("%d: xfer_bytes, %d, CNTL = 0x%02x\n", dev->idx, count, cmd);
		
		/* Start transfer */
		out_8(&iic->cntl, cmd);
		
		/* Wait for completion */
		ret = iic_wait_for_tc(dev);

		if (unlikely(ret < 0))
			break;
		else if (unlikely(ret != count)){
			DBG("%d: xfer_bytes, requested %d, transfered %d\n", 
				dev->idx, count, ret);
			
			/* If it's not a last part of xfer, abort it */
			if (combined_xfer || (i < loops - 1))
    				iic_abort_xfer(dev);
				
			ret = -EREMOTEIO;
			break;				
		}
		
		if (cntl & CNTL_RW)
			for (j = 0; j < count; ++j)
				*buf++ = in_8((volatile u8*)&iic->mdbuf);
	}
	
	return ret > 0 ? 0 : ret;
}

/*
 * Set target slave address for master transfer
 */
static inline void iic_address(struct ibm_iic_private* dev, struct i2c_msg* msg)
{
	volatile struct iic_regs *iic = dev->vaddr;
	u16 addr = msg->addr;
	
	DBG2("%d: iic_address, 0x%03x (%d-bit)\n", dev->idx, 
		addr, msg->flags & I2C_M_TEN ? 10 : 7);
	
	if (msg->flags & I2C_M_TEN){
	    out_8(&iic->cntl, CNTL_AMD);
	    out_8(&iic->lmadr, addr);
	    out_8(&iic->hmadr, 0xf0 | ((addr >> 7) & 0x06));
	}
	else {
	    out_8(&iic->cntl, 0);
	    out_8(&iic->lmadr, addr << 1);
	}
}

static inline int iic_invalid_address(const struct i2c_msg* p)
{
	return (p->addr > 0x3ff) || (!(p->flags & I2C_M_TEN) && (p->addr > 0x7f));
}

static inline int iic_address_neq(const struct i2c_msg* p1, 
				  const struct i2c_msg* p2)
{
	return (p1->addr != p2->addr) 
		|| ((p1->flags & I2C_M_TEN) != (p2->flags & I2C_M_TEN));
} 

/*
 * Generic master transfer entrypoint. 
 * Returns the number of processed messages or error (<0)
 */
static int iic_xfer(struct i2c_adapter *adap, struct i2c_msg msgs[], int num)
{
    	struct ibm_iic_private* dev = (struct ibm_iic_private*)(i2c_get_adapdata(adap));
	volatile struct iic_regs *iic = dev->vaddr;
	int i, ret = 0;
	
	DBG2("%d: iic_xfer, %d msg(s)\n", dev->idx, num);
	
	if (!num)
		return 0;
	
	/* Check the sanity of the passed messages.
	 * Uhh, generic i2c layer is more suitable place for such code...
	 */
	if (unlikely(iic_invalid_address(&msgs[0]))){
		DBG("%d: invalid address 0x%03x (%d-bit)\n", dev->idx, 
			msgs[0].addr, msgs[0].flags & I2C_M_TEN ? 10 : 7);
		return -EINVAL;
	}		
	for (i = 0; i < num; ++i){
		if (unlikely(msgs[i].len <= 0)){
			DBG("%d: invalid len %d in msg[%d]\n", dev->idx, 
				msgs[i].len, i);
			return -EINVAL;
		}
		if (unlikely(iic_address_neq(&msgs[0], &msgs[i]))){
			DBG("%d: invalid addr in msg[%d]\n", dev->idx, i);
			return -EINVAL;
		}
	}
	
	/* Check bus state */
	if (unlikely((in_8(&iic->extsts) & EXTSTS_BCS_MASK) != EXTSTS_BCS_FREE)){
		DBG("%d: iic_xfer, bus is not free\n", dev->idx);
		
		/* Usually it means something serious has happend.
		 * We *cannot* have unfinished previous transfer
		 * so it doesn't make any sense to try to stop it.
		 * Probably we were not able to recover from the 
		 * previous error.
		 * The only *reasonable* thing I can think of here
		 * is soft reset.  --ebs
		 */
		iic_dev_reset(dev);
		
		if ((in_8(&iic->extsts) & EXTSTS_BCS_MASK) != EXTSTS_BCS_FREE){
			DBG("%d: iic_xfer, bus is still not free\n", dev->idx);
			return -EREMOTEIO;
		}
	} 
	else {
		/* Flush master data buffer (just in case) */
		out_8(&iic->mdcntl, in_8(&iic->mdcntl) | MDCNTL_FMDB);
	}
	
	/* Load slave address */
	iic_address(dev, &msgs[0]);
	
	/* Do real transfer */
    	for (i = 0; i < num && !ret; ++i)
		ret = iic_xfer_bytes(dev, &msgs[i], i < num - 1);

	return ret < 0 ? ret : num;
}

static u32 iic_func(struct i2c_adapter *adap)
{
	return I2C_FUNC_I2C | I2C_FUNC_SMBUS_EMUL | I2C_FUNC_10BIT_ADDR;
}

static struct i2c_algorithm iic_algo = {
	.name 		= "IBM IIC algorithm",
	.id   		= I2C_ALGO_OCP,
	.master_xfer 	= iic_xfer,
	.smbus_xfer	= NULL,
	.slave_send	= NULL,
	.slave_recv	= NULL,
	.algo_control	= NULL,
	.functionality	= iic_func
};

/*
 * Calculates IICx_CLCKDIV value for a specific OPB clock frequency
 */
static inline u8 iic_clckdiv(unsigned int opb)
{
	/* Compatibility kludge, should go away after all cards
	 * are fixed to fill correct value for opbfreq.
	 * Previous driver version used hardcoded divider value 4,
	 * it corresponds to OPB frequency from the range (40, 50] MHz
	 */
	if (!opb){
		printk(KERN_WARNING "ibm-iic: using compatibility value for OPB freq,"
			" fix your board specific setup\n");
		opb = 50000000;
	}

	/* Convert to MHz */
	opb /= 1000000;
	
	if (opb < 20 || opb > 150){
		printk(KERN_CRIT "ibm-iic: invalid OPB clock frequency %u MHz\n",
			opb);
		opb = opb < 20 ? 20 : 150;
	}
	return (u8)((opb + 9) / 10 - 1);
}

/*
 * Register single IIC interface
 */
static int __devinit iic_probe(struct ocp_device *ocp){

	struct ibm_iic_private* dev;
	struct i2c_adapter* adap;
	int ret;
	bd_t* bd = (bd_t*)&__res;
	
	if (!(dev = kmalloc(sizeof(*dev), GFP_KERNEL))){
		printk(KERN_CRIT "ibm-iic: failed to allocate device data\n");
		return -ENOMEM;
	}

	memset(dev, 0, sizeof(*dev));
	dev->idx = ocp->num;
	ocp_set_drvdata(ocp, dev);
	
	if (!(dev->vaddr = ioremap(ocp->paddr, sizeof(struct iic_regs)))){
		printk(KERN_CRIT "ibm-iic%d: failed to ioremap device registers\n",
			dev->idx);
		ret = -ENXIO;
		goto fail2;
	}
	
	init_waitqueue_head(&dev->wq);

	dev->irq = iic_force_poll ? -1 : ocp->irq;
	if (dev->irq >= 0){
		/* Disable interrupts until we finish intialization,
		   assumes level-sensitive IRQ setup...
		 */
		iic_interrupt_mode(dev, 0);
		if (request_irq(dev->irq, iic_handler, 0, "IBM IIC", dev)){
			printk(KERN_ERR "ibm-iic%d: request_irq %d failed\n", 
				dev->idx, dev->irq);
			/* Fallback to the polling mode */	
			dev->irq = -1;
		}
	}
	
	if (dev->irq < 0)
		printk(KERN_WARNING "ibm-iic%d: using polling mode\n", 
			dev->idx);
		
	/* Board specific settings */
	BUG_ON(dev->idx >= sizeof(bd->bi_iic_fast) / sizeof(bd->bi_iic_fast[0]));
	dev->fast_mode = iic_force_fast ? 1 : bd->bi_iic_fast[dev->idx];
	
	/* clckdiv is the same for *all* IIC interfaces, 
	 * but I'd rather make a copy than introduce another global. --ebs
	 */
	dev->clckdiv = iic_clckdiv(bd->bi_opb_busfreq);
	DBG("%d: clckdiv = %d\n", dev->idx, dev->clckdiv);
	
	/* Initialize IIC interface */
	iic_dev_init(dev);
	
	/* Register it with i2c layer */
	adap = &dev->adap;
	strcpy(adap->name, "IBM IIC");
	i2c_set_adapdata(adap, dev);
	adap->id = I2C_HW_OCP | iic_algo.id;
	adap->algo = &iic_algo;
	adap->client_register = NULL;
	adap->client_unregister = NULL;
	adap->timeout = 1;
	adap->retries = 1;

	if ((ret = i2c_add_adapter(adap)) != 0){
		printk(KERN_CRIT "ibm-iic%d: failed to register i2c adapter\n",
			dev->idx);
		goto fail;
	}
	
	printk(KERN_INFO "ibm-iic%d: using %s mode\n", dev->idx,
		dev->fast_mode ? "fast (400 kHz)" : "standard (100 kHz)");

	return 0;

fail:	
	if (dev->irq >= 0){
		iic_interrupt_mode(dev, 0);
		free_irq(dev->irq, dev);
	}	

	iounmap((void*)dev->vaddr);
fail2:	
	ocp_set_drvdata(ocp, 0);
	kfree(dev);	
	return ret;
}

/*
 * Cleanup initialized IIC interface
 */
static void __devexit iic_remove(struct ocp_device *ocp)
{
	struct ibm_iic_private* dev = (struct ibm_iic_private*)ocp_get_drvdata(ocp);
	BUG_ON(dev == NULL);
	if (i2c_del_adapter(&dev->adap)){
		printk(KERN_CRIT "ibm-iic%d: failed to delete i2c adapter :(\n",
			dev->idx);
		/* That's *very* bad, just shutdown IRQ ... */
		if (dev->irq >= 0){
		    iic_interrupt_mode(dev, 0);	
		    free_irq(dev->irq, dev);
		    dev->irq = -1;
		}
	} else {
		if (dev->irq >= 0){
		    iic_interrupt_mode(dev, 0);	
		    free_irq(dev->irq, dev);
		}
		iounmap((void*)dev->vaddr);
		kfree(dev);
	}
}

static struct ocp_device_id ibm_iic_ids[] __devinitdata = 
{
	{ .vendor = OCP_VENDOR_IBM, .device = OCP_FUNC_IIC },
	{ .vendor = OCP_VENDOR_INVALID }
};

MODULE_DEVICE_TABLE(ocp, ibm_iic_ids);

static struct ocp_driver ibm_iic_driver =
{
	.name 		= "ocp_iic",
	.id_table	= ibm_iic_ids,
	.probe		= iic_probe,
	.remove		= __devexit_p(iic_remove),
#if defined(CONFIG_PM)
	.suspend	= NULL,
	.resume		= NULL,
#endif
};

static int __init iic_init(void)
{
	printk(KERN_INFO "IBM IIC driver v" DRIVER_VERSION "\n");
	return ocp_module_init(&ibm_iic_driver);
}

static void __exit iic_exit(void)
{
	ocp_unregister_driver(&ibm_iic_driver);
}

module_init(iic_init);
module_exit(iic_exit);
