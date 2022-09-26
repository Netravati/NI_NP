# NI_NP - C++ Coding Challenge
Program to detect Feedback Loop for Audio Effects.

# Build & Run 
Unit testing is done with 3 basic test cases.
These tests are in main() and upon run all 3 cases gets executed.
answer indications: 0 for No Feedback Loop, 1 if there is Feedback Loop.

$ make all run

# Question 2: The way AudioEffect is designed is not ideal. What would you change about the design? Please add some comments in the file you will submit for the first question.
Yes, the control block is thread-safe; but NO, the access to the resource is not thread-safe
Using shared_ptr for the audioeffect chain is not ideal because there is no explaination with the given senario that the shared ownership is required. As given example explains that the AudioEffect would own the next audioeffect, but shared_ptr used when there more resources might use of an object in its lifetime. 
Also, using shared pointer in Case 2 (FeedbackLoop occur) will leads to the memory leakage, (Also commented in code). 
This can be handled by using weak_ptrs for such cases.

A good to use might be 'unique_ptr' for better managing the allocation/deallocation of memory. But this can also be difficult to use simple, lower level data types such as a effects chain with linked list. Depending on the requirements of the system and over all system architecture and module design, using basic data structures like std::list or std::vector or even map. And accordingly the algorithm can also be varied to traversal approaches like DFS or BFS.
