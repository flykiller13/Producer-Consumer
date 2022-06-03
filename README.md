# Producer-Consumer
Classic solution to the Producer-Consumer threading problem. Solved as an excercise.

The problem is defined as follows: there is a fixed-size buffer and a Producer process, and a Consumer process.

The Producer process creates an item (in this case a random number) and adds it to the shared buffer. The Consumer process takes items out of the shared buffer and “consumes” them. (in this case prints)

The problem is solved in C using threading.
The threads are divided into consumers and producers.
The first error we get is Segmentation Fault which occurs because of our fixed buffer size. A producer tries to put an item into the buffer while the buffer is full, since the consumer is slower (Also doesn’t work the other way around). 
This is solved using a mutex, to lock the producer from creating items when the buffer is full, and to lock the consumer from consuming when the buffer is empty. This solution adds ifs to check if the buffer is full or empty, causing calls to be wasted if the condition is false.
The better solution is using 2 semaphores - One to keep track of empty slots, and one to keep track of filled slots.
The “Empty” semaphore locks the producer - Makes him wait until there is an empty slot. It gets updated by the consumer, after consuming an item.
The “Filled” semaphore locks the consumer - Makes him wait until there is a product in the buffer if the buffer is empty. It gets updated by the producer, after producing an item.
This means we can remove the ifs (Since the thread will just wait instead).

Ron Yoffe.
