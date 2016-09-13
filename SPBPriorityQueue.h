#ifndef SPBPRIORITYQUEUE_H_
#define SPBPRIORITYQUEUE_H_
#include "SPListElement.h"
#include "SPList.h"
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
 * Creates a new bounded priority queue with a max size of maxSize
 * the new queue is empty at first.
 *
 * @param maxSize - the bounded size of the queue
 * @return
 * NULL - in case of memory allocation failure OR maxSize < 1
 * Otherwise, a new empty bounded priority queue is returned.
 */
SPBPQueue spBPQueueCreate(int maxSize);

/**
 * Creates a copy of the given queue.
 * the new copy queue will contain the same maxSize, and a
 * copy of the same elements in the same order.
 *
 * @param source - the queue which will be copied
 * @return
 * NULL - in case of memory allocation failure OR (source == NULL)
 * Otherwise, a copy of source is returned.
 */
SPBPQueue spBPQueueCopy(SPBPQueue source);

/**
 * Frees all the memory used by the source queue.
 * more specifically -
 * 			a) Deallocated all the elements using the element destroy function
 * 			b) Deallocated the queue itself
 *
 * @param source - The source queue to be destroyed.
 */
void spBPQueueDestroy(SPBPQueue source);

/**
 * Removes all elements from the queue, while deallocating the elements' memory.
 * After this function is called, (spBPQqueueIsEmpty(source) == NULL)
 *
 * @param source - The source queue to be cleared
 */
void spBPQueueClear(SPBPQueue source);

/**
 * Returns the number of elements in a given a queue.
 *
 * @param source - the queue of which the size is requested
 * @return
 * -1 if (source == NULL)
 * Otherwise, the number of elements in the queue is returned.
 */
int spBPQueueSize(SPBPQueue source);

/**
 * Returns the maximum number of elements the queue can hold.
 *
 * @param source - the queue of which the maximum size was requested
 * @return
 * -1 if (source == NULL)
 * Otherwise, the maximum number of elements the queue can hold returned.
 */
int spBPQueueGetMaxSize(SPBPQueue source);

/**
 * Adds a COPY of a given element to the queue if one of the following holds:
 * 		(a)The queue is not full.
 * 		(b)The queue is full, but given element is smaller* than last element in source queue.
 * 		  (*smaller - in respect to the element compare function)
 * The element will be inserted at the right place, which means:
 * (before the call of the function queue is (q1, q2,...,qn), and q1<=q2<=...qn-1<=qn)
 * 		(1) if queue was empty, queue will be(element)
 * 		(2) and element<q1, element will now be the first element in the queue.
 * 		(3) if queue was not full, and qn<=element, queue will be(q1,...,qn, element)
 * 		(4) if queue was full, and element<qn, queue will be(q1,..,qi,element,qi+1,..,qn-1) when qu<=element<qi+1
 * 		(5) if queue wasn't full, and element<qn, queue will be(q1,..,qi,element,qi+1,..,qn) when qu<=element<qi+1
 * 		*when ever I write < or <=, it is in regards to the element compare function
 * 		**and as I wrote, if queue was full and qn<=element, the queue will stay (q1,...,qn).
 *
 * @param source - The source that element will be inserted in.
 * @param element - the element to be inserted in source.
 * @return
 * SP_BPQUEUE_INVALID_ARGUMENT - if (source == NULL || element == NULL)
 * SP_BPQUEUE_OUT_OF_MEMORY - if memory allocation failure occurs at any stage.
 * SP_BPQUEUE_FULL - if the queue is full, and element WAS NOT INSERTED (i.e (a) and (b) didn't hold)
 * SP_BPQUEUE_SUCCESS - if element was inserted (i.e. (a) or (b) held)
 */
SP_BPQUEUE_MSG spBPQueueEnqueue(SPBPQueue source, SPListElement element);

/**
 * Removes the first element in the given queue, after deallocating memory
 * used by that first element,if the queue isn't empty.
 *
 * @param source - The queue of which the first element will be removed.
 * @return
 * SP_BPQUEUE_INVALID_ARGUMENT - if (source == NULL)
 * SP_BPQUEUE_EMPTY - if source is empty
 * SP_BPQUEUE_SUCCESS - in case an element
 */
SP_BPQUEUE_MSG spBPQueueDequeue(SPBPQueue source);

/**
 * Returns a COPY of the first element in the given queue, if the queue isn't empty.
 *
 * @param source - The queue of which the first element was requested.
 * @return
 * NULL - if (source == NULL) OR source queue is empty OR an allocation failure occurs
 * Otherwise, a copy of the first element in the queue is returned.
 */
SPListElement spBPQueuePeek(SPBPQueue source);

/**
 * Returns a COPY of the last element in the given queue, if the queue isn't empty.
 *
 * @param source - The queue of which the last element was requested.
 * @return
 * NULL - if (source == NULL) OR source queue is empty OR an allocation failure occurs
 * Otherwise, a copy of the last element in the queue is returned.
 */
SPListElement spBPQueuePeekLast(SPBPQueue source);

/**
 * Returns the minimum value of the queue, which is the value of the first element in the given queue.
 *
 * @param source - the queue of which the minimum value was requested from
 * @return
 * -1.0 if (source == NULL) OR source is empty
 * Otherwise, the value of the first element in the queue is returned.
 */
double spBPQueueMinValue(SPBPQueue source);

/**
 * Returns the maximum value of the queue, which is the value of the last element in the given queue.
 *
 * @param source - the queue of which the maximum value was requested from
 * @return
 * -1.0 if (source == NULL) OR source is empty
 * Otherwise, the value of the last element in the queue is returned.
 */
double spBPQueueMaxValue(SPBPQueue source);

/**
 * Returns true iff the queue is empty
 *
 * @param source - The given queue that user is checking to see if empty
 * @assert (source!=NULL)
 * @return
 * true - if the queue is empty
 * false - if the queue is not empty
 */
bool spBPQueueIsEmpty(SPBPQueue source);

/**
 * Returns true iff the queue is full
 *
 * @param source - The given queue that user is checking to see if empty
 * @assert (source!=NULL)
 * @return
 * true - if the queue is full
 * false - if the queue is not full
 */
bool spBPQueueIsFull(SPBPQueue source);


int spBPQueuePeekIndex(SPBPQueue source);
#endif
