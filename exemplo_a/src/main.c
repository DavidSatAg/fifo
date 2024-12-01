#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/syscalls/gpio.h>

struct data_item_t{
	void *fifo_reserved;
	int data;
};

K_FIFO_DEFINE(my_fifo);
K_SEM_DEFINE(fifo_sem, 0, 50);

void thread_produtora1(void *arg1, void *arg2, void *arg3){
	int i=0;
	struct data_item_t tx_data;
	while(1){
		tx_data.data = i;
		k_fifo_put(&my_fifo, &tx_data);
		printk("Alocou o dado %d\n", tx_data.data);
		k_sem_give(&fifo_sem);
		k_msleep(1000);
		i++;
	}
}

void thread_produtora2(void *arg1, void *arg2, void *arg3){
	int j=0;
	struct data_item_t tx_data;
	while(1){
		tx_data.data = j;
		k_fifo_put(&my_fifo, &tx_data);
		printk("Alocou o dado %d\n", tx_data.data);
		k_sem_give(&fifo_sem);
		j++;
		k_msleep(1000);
	}
}

void thread_consumidora1(void *arg1, void *arg2, void *arg3){	
	struct data_item_t *rx_data;
	while(1){
		k_sem_take(&fifo_sem, K_FOREVER);
		rx_data = k_fifo_get(&my_fifo, K_FOREVER);
		printk("Recebeu o dado %d\n", rx_data->data);
		k_msleep(1000);
	}
}

// void thread_consumidora2(void *arg1, void *arg2, void *arg3){	
// 	struct data_item_t *rx_data;
// 	while(1){
// 		k_sem_take(&fifo_sem, K_FOREVER);
// 		rx_data = k_fifo_get(&my_fifo, K_FOREVER);
// 		printk("Recebeu o dado %d\n", rx_data->data);
// 		k_msleep(1000);
// 	}
// }

K_THREAD_DEFINE(thread_produtora1_id, 1024, thread_produtora1, NULL, NULL, NULL, 1, 0, 0);
K_THREAD_DEFINE(thread_produtora2_id, 1024, thread_produtora2, NULL, NULL, NULL, 1, 0, 0);
K_THREAD_DEFINE(thread_consumidora1_id, 1024, thread_consumidora1, NULL, NULL, NULL, 1, 0, 0);
// K_THREAD_DEFINE(thread_consumidora2_id, 1024, thread_consumidora2, NULL, NULL, NULL, 1, 0, 0);