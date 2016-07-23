#ifndef SPBPRIORITYQUEUE_H_
#define SPBPRIORITYQUEUE_H_
#include "SPListElement.h"
#include <stdbool.h>
/**
 * SP Bounded Priority Queue summary
 *
 * TODO Complete documentation
 */


/** type used to define Bounded priority queue **/
typedef struct sp_bp_queue_t* SPBPQueue;

/** type for error reporting **/
typedef enum sp_bp_queue_msg_t {
	SP_BPQUEUE_OUT_OF_MEMORY,
	SP_BPQUEUE_FULL,
	SP_BPQUEUE_EMPTY,
	SP_BPQUEUE_INVALID_ARGUMENT,
	SP_BPQUEUE_SUCCESS
} SP_BPQUEUE_MSG;

/**
 * Creates a new BPQueue with the specific maximum capacity
 *
 * @param maxSize The maximum capacity of BPQueue
 * @return
 * NULL in case of memory allocation fails.
 * A new empty BPQueue with the corresponding max capacity.
 */
SPBPQueue spBPQueueCreate(int maxSize);

/**
 * Creates a copy of target BPQueue.
 *
 * The new copy will contain the same maximum capacity and the same
 * elements as in the target BPQueue.
 *
 * @param source - The source queue
 * @assert (source != NUlL)
 * @return
 * NULL in case memory allocation occurs
 * Others a copy of source is returned.
 */
SPBPQueue spBPQueueCopy(SPBPQueue source);

/**
 * Destroys an BPQueue.
 * All memory allocation associated with the BPQueue will be freed
 *
 * @param source - the target BPQueue which will be freed.
 * 			   if source is NULL, then nothing is done
 */
void spBPQueueDestroy(SPBPQueue source);

/**
 * Clear an BPQueue
 * Removes all the elements in the queue
 *
 * @param source - the target BPQueue which his elements will be removed.
 * 			   if source is NULL, then nothing is done
 */
void spBPQueueClear(SPBPQueue source);

/**
 * A getter for the size of the BPQueue
 *
 * @param source - The source BPQueue
 * @assert source != NULL
 * @return
 * The size of the BPQueue
 */
int spBPQueueSize(SPBPQueue source);

/**
 * A getter for the  max size of the BPQueue
 *
 * @param source - The source BPQueue
 * @assert source != NULL
 * @return
 * The max size of the BPQueue
 */
int spBPQueueGetMaxSize(SPBPQueue source);

/**
 * Inserts a NEW COPY (must be allocated) element to the queue
 * if the BPQueue is in full capacity -
 * 		the new element will be insert iff element(value)<source(lastElementValue)
 * 		and remove the max element.
 *
 * @param source - The source BPQueue
 * @param element - The new element that will be insert to the source queue.
 * @return
 * 	SP_BPQUEUE_OUT_OF_MEMORY in case memory allocation fails
 * 	SP_BPQUEUE_FULL in case of full queue
 * 	SP_BPQUEUE_INVALID_ARGUMENT in case source==NULL
 *  SP_ELEMENT_SUCCESS otherwise
 */
SP_BPQUEUE_MSG spBPQueueEnqueue(SPBPQueue source, SPListElement element);

/**
 * Removes the element with the lowest value
 *
 * @param source - The source BPQueue
 * @return
 * 	SP_BPQUEUE_OUT_OF_MEMORY in case memory allocation fails
 * 	SP_BPQUEUE_EMPTY in case of empty queue
 * 	SP_BPQUEUE_INVALID_ARGUMENT in case source==NULL
 *  SP_ELEMENT_SUCCESS otherwise
 */
SP_BPQUEUE_MSG spBPQueueDequeue(SPBPQueue source);

/**
 * Returns a NEW COPY of the element with the lowest value
 *
 * @param source - The source BPQueue
 * @assert source != NULL
 * @return
 * a NEW COPY of the element with the lowest value.
 */
SPListElement spBPQueuePeek(SPBPQueue source);

/**
 * Returns a NEW COPY of the element with the highest value
 *
 * @param source - The source BPQueue
 * @assert source != NULL
 * @return
 * a NEW COPY of the element with the highest value.
 */
SPListElement spBPQueuePeekLast(SPBPQueue source);

/**
 * A getter for the minimum value in the queue
 *
 * @param source - The source BPQueue
 * @assert source != NULL
 * @return
 * The minimum value in the queue
 */
double spBPQueueMinValue(SPBPQueue source);

/**
 * A getter for the maximum value in the queue
 *
 * @param source - The source BPQueue
 * @assert source != NULL
 * @return
 * The maximum value in the queue
 */
double spBPQueueMaxValue(SPBPQueue source);

/**
 * Checks if the queue is empty
 *
 * @param source - The source BPQueue
 * @assert source != NULL
 * @return
 * True if the queue is empty
 * 		False otherwise
 */
bool spBPQueueIsEmpty(SPBPQueue source);

/**
 * Checks if the queue is full
 *
 * @param source - The source BPQueue
 * @assert source != NULL
 * @return
 * True if the queue is full
 * 		False otherwise
 */
bool spBPQueueIsFull(SPBPQueue source);

#endif
