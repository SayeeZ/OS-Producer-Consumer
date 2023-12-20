# OS-Producer-Consumer

# Producer-Consumer Problem Solution in C using Semaphores, Mutex, and Pthreads

This repository contains a solution to the classical producer-consumer problem implemented in C language using synchronization primitives like semaphores, mutex, and pthreads.

## Problem Description

The producer-consumer problem involves two processes, the producer and the consumer, sharing a common fixed-size buffer. The producer's task is to generate data items and place them into the buffer, while the consumer's task is to consume these items from the buffer.

## Implementation Details

### Files Included:

- `pc_m.c`: Contains the C code implementing the solution using mutex and pthreads.
- `pc_r.c`: Contains the C code implementing the solution using semaphore and pthreads.
- `README.md`: Provides an overview of the solution and instructions.


