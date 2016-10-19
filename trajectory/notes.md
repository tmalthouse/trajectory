#### body.c
ALL threads that call update_state_vectors MUST call `update_sv_thread_destructor` before exiting in order to free the thread-specific buffer. Typically, this is done in the pthread-wrapper function. If running single-threaded, this isn't an issue. The memory will be cleaned up on exit, and is never leaked (since it is always accessable via the function)
