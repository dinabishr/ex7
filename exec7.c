#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/moduleparam.h>
#include<linux/fs.h>
#include<linux/uaccess.h>
#include<linux/slab.h>
#include<asm/segment.h>
#include<linux/buffer_head.h>
#include<linux/processor.h>
#include<linux/kallsyms.h>
#include<linux/cred.h>


MODULE_LICENSE("GPL");
int forkcount=0;
typedef asmlinkage long(*sys_call_ptr_t)(const struct pt_regs *);
static sys_call_ptr_t *sys_ptr;
static sys_call_ptr_t old_fork;


static asmlinkage long myhook(const struct pt_regs *regs){
	forkcount++;
/*	printk(KERN_INFO "%d\n",forkcount);*/


	if(forkcount%10 ==0){
		printk(KERN_INFO "%d\n",forkcount);

	}	

	return old_fork(regs);

}

static int init(void){
	
	printk(KERN_INFO "Hello World CSCE-3402 :)\n");
	
	sys_ptr=(sys_call_ptr_t *)kallsyms_lookup_name("sys_call_table");
	printk(KERN_INFO "%p\n",sys_ptr[__NR_clone]);
	 old_fork=sys_ptr[__NR_clone];
	 write_cr0(read_cr0() & (~0x10000));
	sys_ptr[__NR_clone]=(sys_call_ptr_t)myhook;
	 printk(KERN_INFO "%p\n",sys_ptr[__NR_clone]);
	 
	write_cr0(read_cr0()|0x10000);


	return 0;
}



static void cleanup(void){

	printk(KERN_INFO "Bye bye CSCE-3402 :) \n");
	write_cr0(read_cr0() & (~0x10000));
	sys_ptr[__NR_clone]=old_fork;
	write_cr0(read_cr0()|0x10000);

}


module_init(init);
module_exit(cleanup);




