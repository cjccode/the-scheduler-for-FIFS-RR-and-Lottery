#include "threads.h"
extern int scheduling_type;
extern thread_queue_t *ready_list;

void thread_enqueue(thread_t *t, thread_queue_t *q){
    //enqueue t to the back of the q;
    thread_node_t *tempnode=malloc(sizeof(thread_node_t));//arrange space for tempnode;
    tempnode->thread = t;
    tempnode->next = NULL;//initial tempnode;

    if(q->size == 0) 
    {
        q->size++;
        q->head = tempnode;
        q->tail = tempnode;
       
    }
    else
    {
        q->size++;
        q->tail->next = tempnode;
        q->tail = tempnode;
    }
}

thread_t *thread_dequeue(thread_queue_t *q){
    // dequeue from front of queue q
    thread_t *tempthread;
    tempthread = q->head->thread;

    if(q->size == 1)//if there are only one thread in the ready_list;
    {
        
        free(q->head);
        free(q->tail);
        q->size--;

        return tempthread;
    }


    q->head = q->head->next;
    q->size--;

    return tempthread;
}

thread_t* scheduler(){
    switch(scheduling_type)
    {
        case 0: // Round Robin
        {         
            thread_t *tempthread;
            tempthread = thread_dequeue(ready_list);
            thread_enqueue(tempthread,ready_list);

            return tempthread;
        }


        case 1:// Lottery
        {          
            thread_node_t * node = NULL;
            node = ready_list->head; 
            int sum[ready_list->size];//The *sum is used to store the sum of first i priorities
            int i;
            int priority[ready_list->size];//The *priority is used to store the priorities
            int ran_num;//It is the random number

            for(i = 0; i < ready_list->size; i++)// Make all the priorities of threads in the matrix priority[];
            {
                if(node != NULL)//If the node is not at the end of the ready_list
                {
                    priority[i] = node->thread->priority;
                    node = node->next;
                }
                else
                    break;

                if(i == 0)
                    sum[i] = priority[i]; //Make the value of priority[0] to sum[0];
                else
                    sum[i] = sum[i-1] + priority[i];
            }

                srand(time(NULL));//To generate a random number;
                ran_num = rand()%sum[ready_list->size - 1];//The range is 0 to the sum of all pritories;

                node = ready_list->head;//make the node back to the head of ready_list;
                for(i = 0; i < ready_list->size; i++)
                {
                    if(ran_num < sum[i])
                    {
                        return node->thread;
                    }
                    else
                        node = node->next;
                }
        }

        case 2:          //First come first serve
            return ready_list->head->thread;

        default:
            return NULL;
    }
}
