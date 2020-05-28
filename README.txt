Willis A. Hershey
May 28th, 2020

This is a pthread library to help add context to signal handling.

The functions in this library maintain a stack of saved void pointers each of which can only be accessed by the pthreads that stored them. With the exception of the function pthread_store_close(), this library is thread-safe, and treats each thread distinctly, telling them apart by the pthread_t value returned by the POSIX function pthread_self().

The functions in this library are as follows:

int pthread_store(void*);

A call to pthread_store() by some pthread with some void pointer as an argument stores that void pointer for later use to that pthread. This function returns zero on success, and returns negative one if malloc fails. There is no limit on the number of void pointers a pthread may store using pthread_store() outside of the memory capacity of the machine. If more than one void pointer is stored by any given pthread, calls to pthread_recall() and pthread_discard() return the value that that pthread most recently stored. pthread_store() does not check the validity of a pointer before it stores it.

void* pthread_recall();

A call to pthread_recall() returns the void pointer most recently stored using pthread_store() by the calling thread. This function does not remove the stored void pointer from storage, so any number of calls to pthread_recall() by the same thread can be expected to return the same value until that thread stores a new value using pthread_store(), or removes that value from storage with pthread_discard() or pthread_discard_all(). If pthread_recall() is called by a thread that has never stored a value, or by a thread that has discarded all of its stored values, pthread_recall() will return NULL. pthread_recall() does not check the validity of a pointer before it returns it, so, for instance, if the most recently stored value was a NULL void pointer, then pthread_recall() will return NULL even if there are other valid less-recently-stored values.

void* pthread_discard();

A call to pthread_discard() returns the void pointer most recently stored using pthread_store() by the calling thread. After a call to pthread_discard() succeeds, the void pointer stored most recently by the calling thread has been removed from storage, so subsequent calls to pthread_discard() will return the next most recently stored value. If pthread_discard() is called by a thread that has never stored a value, or by a thread that has discarded all of its stored values, pthread_discard() will return NULL. In the same manner as pthread_recall(), pthread_discard() does not check the validity of values before returning them, so, for instance, if the most recently stored value was a NULL void pointer then pthread_discard() will return NULL, but subsequent calls to pthread_discard() may return valid values until there are no more, at which point it will return NULL again.

int pthread_discard_all();

A call to pthread_discard_all() removes from storage all void pointers stored by the calling thread. This function returns the number of values removed. pthread_discard_all() does not return any of the stored values, but instead throws them all away, so if these values are needed, pthread_discard_all() can be simulated by running pthread_discard() in a loop until pthread_discard() returns NULL; by simulating the function in this way, the calling thread will recieve all stored values one by one in reverse storing order. Note that this looping mechanism is only functional if you can confirm that a NULL void pointer was never stored by this thread using pthread_store(). This function is thread-safe

int pthread_store_close();

A call to pthread_store_close() frees all memory associated with the storage structures of this library. This function can be called by any thread in a process, but this function is not thread-safe, so it it up to the programmer to insure that there are no pending calls to any of the other functions when this function is called. After this function succeeds, calls to all other functions in this library are undefined behavior. This function returns zero on success, or returns EBUSY or EINVAL if one of those errors was returned to pthread_store_close() by pthread_mutex_destroy().
