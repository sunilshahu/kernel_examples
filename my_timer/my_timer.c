#define DEBUG
#include <linux/kernel.h>	/* We're doing kernel work */
#include <linux/module.h>	/* Specifically, a module */
#include <linux/proc_fs.h>	/* Necessary because we use the proc fs */
#include <linux/init.h>		/* For __init and __exit */
#include <linux/interrupt.h>	/* For irqreturn_t */
#include <linux/timer.h>
#include <linux/jiffies.h>
#include <linux/workqueue.h>

struct timer_list my_timer;

struct tasklet_struct my_tasklet;

static struct workqueue_struct *my_workqueue_struct = NULL;
struct work_struct my_work_struct;

static struct workqueue_struct *my_delayed_workqueue_struct = NULL;
struct delayed_work my_delayed_work_struct;

static void my_tasklet_fun(unsigned long data)
{
	pr_debug("&%s() : data = %lu\n", __func__, data);
}

static void my_timer_function(unsigned long data)
{
	pr_debug("%s\n", __func__);

	tasklet_schedule(&my_tasklet);

	my_timer.expires = jiffies + 500;
	add_timer(&my_timer);
}

void my_work_fn(struct work_struct *work)
{
	pr_debug("&%s()\n", __func__);
}

void my_delayed_work_fn(struct work_struct *work)
{
	pr_debug("&%s()\n", __func__);
}

static int __init my_timer_init(void)
{
	int  ret = 0;
	unsigned long data = 15;
	pr_debug("%s()\n", __func__);

	INIT_WORK(&my_work_struct, my_work_fn);
	my_workqueue_struct = create_workqueue("my_workq");
	queue_work(my_workqueue_struct, &my_work_struct);

	INIT_DELAYED_WORK(&my_delayed_work_struct, my_delayed_work_fn);
	my_delayed_workqueue_struct = create_workqueue("my_delayed_workq");
	queue_delayed_work(my_delayed_workqueue_struct, &my_delayed_work_struct,
			msecs_to_jiffies(3000));
	

	tasklet_init(&my_tasklet, my_tasklet_fun, data);

	init_timer(&my_timer);
	my_timer.function = my_timer_function;
	my_timer.data = 0;
	my_timer.expires = jiffies + 500;
	add_timer(&my_timer);
	return ret;
}

static void __exit my_timer_exit(void)
{
	pr_debug("%s()\n", __func__);

	if(my_workqueue_struct)
		destroy_workqueue(my_workqueue_struct);
	
	if(my_delayed_workqueue_struct)
		destroy_workqueue(my_delayed_workqueue_struct);
	
	tasklet_kill(&my_tasklet);

	del_timer(&my_timer);
}



module_init(my_timer_init);
module_exit(my_timer_exit);
MODULE_LICENSE("GPL");
