## Task for SOI subject (Operating Systems)

### Task description:

We have a FIFO buffer for integers.

* Processes A1 generate consecutive even numbers modulo 50 if there are fewer than 10 even numbers in the buffer.

* Processes A2 generate consecutive odd numbers modulo 50 if there are more even than odd numbers in the buffer.

* Processes B1 consume even numbers if the buffer contains at least 3 even numbers.

* Processes B2 consume odd numbers if the buffer contains at least 7 numbers.

There can be any number of processes of each type in the system. Implement the functionality described above using semaphores.

Assuming the FIFO buffer, in addition to standard put() and get() methods, has only a method that allows checking the number at the output (without removing it) and methods for counting even and odd elements. We also assume that semaphores have only P and V operations.