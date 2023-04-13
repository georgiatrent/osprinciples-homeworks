#include <linux/module.h>
#include <linux/printk.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/sched.h>

static struct proc_dir_entry *entry;

static int example(struct seq_file *m, void *v){
	struct task_struct *p;
	int count = 0;
	for_each_process(p) {
		count++;
	}
	seq_printf(m, "%d\n", count);
	return 0;
}

static int __init example_init(void)
{
	entry = proc_create_single("count", 0, NULL, example);
	pr_info("example: init\n");
	return 0;
}

static void __exit example_exit(void)
{
	proc_remove(entry);
	pr_info("example: exit\n");
}

module_init(example_init);
module_exit(example_exit);

MODULE_AUTHOR("Georgia Trentalange");
MODULE_DESCRIPTION("CS111 lab0");
MODULE_LICENSE("GPL");
