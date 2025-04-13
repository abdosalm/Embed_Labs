/**
 * Exercise1: Threads Intro
 */


// #include <zephyr/kernel.h>


// void Thread1(void)
// {
//     while(1)
//     {
//         printk("Thread0\n\r");
//         // k_yield();
//         // k_msleep(5);
//     }
// }

// void Thread2(void)
// {
//     while(1)
//     {
//         printk("Thread1\n\r");
//         // k_yield();
//         // k_msleep(5);
//     }
// }

// /**
//      * Parameters:
//         name	Name of the thread.
//         stack_size	Stack size in bytes.
//         entry	Thread entry function.
//         p1	1st entry point parameter.
//         p2	2nd entry point parameter.
//         p3	3rd entry point parameter.
//         prio	Thread priority.
//         options	Thread options.
//         delay	Scheduling delay (in milliseconds), zero for no delay.
//     */
       
// K_THREAD_DEFINE(thread0_ID, 1024, Thread1, NULL, NULL, NULL, 7, 0, 0);
// K_THREAD_DEFINE(thread1_ID, 1024, Thread2, NULL, NULL, NULL, 7, 0, 0);

// int main(void)
// {
//     return 0;
// }


/**
 * Exercise2: Mutex
 */


#include <zephyr/kernel.h>
#include <zephyr/random/random.h>

#define COMBINED_TOTAL   40
int32_t increment_count = 0; 
int32_t decrement_count = COMBINED_TOTAL;

K_MUTEX_DEFINE(mutex);

void critical_section()
{
     k_mutex_lock(&mutex, K_FOREVER);
    increment_count++;
    decrement_count--;
    increment_count = increment_count % COMBINED_TOTAL; 
    if (decrement_count == 0) 
    {
        decrement_count = COMBINED_TOTAL;
    }
     k_mutex_unlock(&mutex);

    // at any time, the inc_count + dec_count  = 40
    if(increment_count + decrement_count != COMBINED_TOTAL )
	{
		printk("Race condition happend! current Thread = %s\n\r", k_thread_name_get(k_current_get()));
		printk("Increment_count (%d) + Decrement_count (%d) = %d \n\r",
	                increment_count, decrement_count, (increment_count + decrement_count));

        // sleep a radnom time from 400ms to 409ms 
		k_msleep(400 + sys_rand32_get() % 10);
	}

}

void Thread1(void)
{
    printk("Thread 1 started\n\r");
    while(1)
    {
        critical_section();
    }
}

void Thread2(void)
{   
    printk("Thread 2 started\n\r");
    while(1)
    {
        critical_section();
    }
}
       
K_THREAD_DEFINE(thread1_ID, 1024, Thread1, NULL, NULL, NULL, 7, 0, 0);
K_THREAD_DEFINE(thread2_ID, 1024, Thread2, NULL, NULL, NULL, 7, 0, 0);

int main(void)
{
    printk("main started\n\r");
    return 0;
}
