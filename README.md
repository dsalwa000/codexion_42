*This project has been created as part of the 42 curriculum by dsalwa.*

# codexion

## Description

The goal of this project is to understand multithreaded programming. This specific task is based on the Dining Philosophers problem, but it adds a scheduler: fifo or edf, which determines which coder has priority to take a dongle.

- N coders sit in a circle and share N dongles (one between each pair of neighbours).
- Cycle of each coder: ... -> **take 2 dongles** -> **compile** -> **release dongles** -> **debug** -> **refactor** -> ...
- A coder **burns out** if they do not start compiling within `time_to_burnout` ms
  of their previous compile start (or of the simulation start).
- The simulation stops when a coder burns out or when everyone has compiled
  `number_of_compiles_required` times.

## Instructions

### Compilation

```sh
make		# builds ./codexion
make clean	# removes object files
make fclean	# removes object files and the binary
make re		# full rebuild
```

### Usage

```sh
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug \
		time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

All arguments are mandatory and all times are in milliseconds.
`number_of_coders`, `time_to_burnout`, `time_to_compile`, `time_to_debug`, `time_to_refactor`, `number_of_compiles_required` and `dongle_cooldown` must be integer values: `number_of_coders` at least 1, the others at least 0.
`scheduler` must be exactly `fifo` or `edf`.

The program stops when every coder has compiled `number_of_compiles_required` times, or when one of the coders burns out.


### Examples

```sh
./codexion 5 800 200 200 200 5 50 fifo
./codexion 4 410 200 100 100 5 0 edf
./codexion 1 800 200 200 200 5 0 fifo
```

### Output format

```
<timestamp_ms> <coder_id> has taken a dongle
<timestamp_ms> <coder_id> is compiling
<timestamp_ms> <coder_id> is debugging
<timestamp_ms> <coder_id> is refactoring
<timestamp_ms> <coder_id> burned out
```

## Resources

### References

- Dining philosophers problem – [link](https://en.wikipedia.org/wiki/Dining_philosophers_problem)
- Coffman conditions (deadlock) – [link](https://faq.computersciencewiki.org/index.php/home/article/coffman-conditions)

### Use of AI

AI was used as a development assistant for:
- **Edge cases & validation**: Catching hidden bugs and verifying output correctness.
- **Concurrency & safety**: Explaining thread lifecycles and spotting potential deadlock scenarios.
- **Implementation**: Assisting with core function design and multi-threading logic.
- **Documentation**: Structuring and editing the README.md file.

## Blocking cases handled

### Deadlock and Coffman conditions

Deadlock can only occur if all 4 Coffman conditions hold at the same time, so breaking any one of them prevents it.

- **Mutual exclusion** is kept on purpose: a dongle can be used by only one coder at a time (`is_held`, protected by the dongle's mutex).
- **Hold and wait** is broken in `start_compiling()`: it takes both dongles atomically under both mutexes, or takes none and retries.
- **No preemption** is kept as well: a dongle is never taken away from a coder who is compiling.
- **Circular wait** is broken by resource ordering in `init_coder()` (`setup.c`): every coder locks the dongle with the lower index first, and for the last coder the order is reversed (`dongles[0]` before `dongles[N-1]`). A cycle would need a thread holding a higher index while waiting for a lower one, which never happens.

### Starvation prevention

Each dongle has its own priority queue (binary heap). A coder may take a dongle
only if it is free, its cooldown has expired and the coder is at the head of
its queue (`can_take()`), so nobody can jump ahead by winning the mutex race.
The scheduler decides the order:
- **fifo** – priority is the request time; the oldest request is served first.
- **edf** – priority is `last_compile_start` (deadline = `last_compile_start +
  time_to_burnout`); the coder closest to burnout is served first.

### Dongle cooldown

When a dongle is released, `available_at` is set to `now + dongle_cooldown`.
`can_take()` refuses the dongle until that time, so a released dongle is neither
held nor grantable during the cooldown. The coder at the head of the queue keeps
its place and gets the dongle as soon as the cooldown ends.

### Burnout detection

The monitor thread checks every coder each ~1 ms (`CHECK_WAIT`), so a burnout
is reported well within the required 10 ms. If
`now - last_compile_start > time_to_burnout`, it prints the burnout line and
sets `stopped` in one critical section of `log_mutex`. Coders sleep in 0.5 ms
steps (`ft_sleep()`) and check `stopped` each time, so they stop almost
immediately.

### Log serialization

All output goes through `print_status()`, which locks `log_mutex`, computes the
timestamp and prints only if the simulation has not stopped. Lines can't
interleave, and nothing is printed after the simulation ends.

## Thread synchronization mechanisms

- **`dongle->mutex`** (one per dongle) – protects `is_held`, `available_at` and
  the dongle's queue. `start_compiling()` checks and takes both dongles in one
  critical section, so two neighbours can't take the same dongle.
- **`coder->state_mutex`** (one per coder) – protects `last_compile_start` and
  `compiles`. Coders write them, the monitor reads them – both under this mutex.
- **`stop_mutex`** – protects the `stopped` flag. The monitor sets it, coders
  check it in every waiting loop and exit.
- **`log_mutex`** – every `printf` happens under it, so lines never interleave.
  The burnout line and `stopped = 1` are done in one critical section, so
  nothing is printed after a burnout.
- **`start_mutex` + `start_cond`** – a start event: all threads wait until main
  broadcasts that the simulation has started, so they share one start time.

Mutexes are always locked in a fixed order (dongles by ascending index,
`log_mutex` before `stop_mutex`) and are never held while sleeping.
