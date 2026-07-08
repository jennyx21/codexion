*This project has been created as part of the 42 curriculum by Jtruckse.*

# Codexion

## Description

Codexion is a multithreaded synchronization project inspired by the classic Dining Philosophers problem.
The objective is to design a concurrent simulation where multiple coder threads compete for shared resources ("dongles") while respecting strict timing constraints and avoiding common synchronization issues.
Each coder repeatedly performs a cycle consisting of compiling, debugging, and refactoring. To compile, a coder must acquire two shared dongles.
A dedicated monitor thread continuously checks the state of every coder to detect burnout and determine when the simulation should terminate.
The project focuses safe communication between threads while preventing deadlocks, race conditions, starvation, and inconsistent logging.


## Instructions

#### compile with
```bash
make
```

other available features:
```bash
make clean    # Remove object files
make fclean   # Remove all generated files
make re       # Rebuild from scratch
```

the Make command creates a executable: 
- ./codexion

this executable takes multiple arguments which all are mandatory

```bash
./codexion <number_of_coders> <time_to_burnout> <time_to_compile> <time_to_debug> <time_to_refactor> <number_of_compiles_required> <dongle_cooldown> <scheduler>
```
#### The Parameters

| Parameter                            | Description                                     |
| ------------------------------------ | ----------------------------------------------- |
| number_of_coders                     | Number of coder threads                         |
| time_to_burnout                      | Maximum time without compiling before burnout   |
| time_to_compile                      | Time spent compiling                            |
| time_to_debug                        | Time spent debugging                            |
| time_to_refactor                     | Time spent refactoring                          |
| number_of_compiles_required          | how many times the coders have to do the cycle  |
| dongle_cooldown                      | Time a dongle needs to be reused                |
| scheduler                            | either edf or fifo - for fair distribution      |


e.g 

```bash
./codexion 2 800 200 200 200 6 4 edf
./codexion 2 800 200 200 200 6 4 fifo
```

## Blocking cases handled

#### Deadlock prevention

Deadlocks are prevented by combining two strategies:

- Even-numbered coders acquire the left dongle first, then the right dongle.
- Odd-numbered coders acquire the right dongle first, then the left dongle.

This alternating acquisition order prevents every coder from waiting for the same resource simultaneously and therefore eliminates circular waiting.
Additionally, every dongle maintains its own waiting queue. A coder may only acquire a dongle if it is both available and the coder owns the highest-priority request.

### Scheduler-controlled fairness

Every dongle stores pending acquisition requests.
Each request receives a priority depending on the selected scheduler:

- FIFO uses the request creation order.
- EDF (Earliest Deadline First) uses the coder's burnout deadline.

Only the request with the highest priority is allowed to acquire the dongle when it becomes available.
This prevents newer requests from continuously overtaking older or more urgent ones.

Starvation is reduced through the request queues and scheduling policy.
Because each dongle always serves the highest-priority waiting request, coders waiting the longest (FIFO) or closest to burnout (EDF) receive priority.

### Burnout detection

Every coder owns a dedicated monitor thread.
The monitor periodically checks the elapsed time since the last successful compilation.

If
    current_time - last_compile > time_to_burnout

the simulation terminates immediately and reports the burnout.

### Log serialization

Console output is protected by a dedicated mutex (shared->print).
Only one thread may print at a time, preventing interleaved or corrupted log messages.


## Thread synchronization mechanisms

### pthread_mutex_t

The implementation relies on several mutexes.

#### Shared mutex

shared->mutex protects global simulation state, including:

- > simulation flag
- > request counter
- > last compilation timestamps
- > remaining compile count

This guarantees consistent access from coder and monitor threads.

#### Dongle mutex

Each dongle owns its own mutex.

The mutex protects:

- > availability (taken)
- > waiting queue
- >request insertion/removal

Only one thread may modify a dongle's state at any time.

#### Print mutex

shared->print serializes all console output.

Without this mutex, multiple threads could print simultaneously, producing unreadable output.

#### Barrier mutex

shared->waiter together with shared->cond implements a synchronization barrier.

All coder threads wait until every thread has been created.

Only then is the common simulation start time recorded and all threads begin simultaneously.

The same mechanism is reused to synchronize shutdown.

## Resources

- https://docs.oracle.com/cd/E19455-01/806-5257/6je9h032r/index.html
- https://docs.oracle.com/cd/E19455-01/806-5257/sync-112/index.html
- https://www.geeksforgeeks.org/c/multithreading-in-c/
- https://github.com/evaristoc/codexion-42
- https://medium.com/@ruinadd/philosophers-42-guide-the-dining-philosophers-problem-893a24bc0fe2

#### AI Usage

Artificial Intelligence was used as a learning and documentation aid.

AI assisted with:

- explaining multithreading concepts
- reviewing synchronization strategies
- improving documentation
- proofreading the README
