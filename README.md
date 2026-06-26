*This project has been created as part of the 42 curriculum by emmmilla.*

# get_next_line

A custom implementation of a buffered line-reading function developed as part of the 42 curriculum.

---

## Description

**get_next_line** is one of the core projects in the 42 curriculum. Its objective is to implement a function capable of reading text from a file descriptor one line at a time while preserving the reading state between successive calls.

Unlike standard line-reading utilities, the project must be implemented using only a very limited set of authorized functions. This constraint encourages the design of a custom buffering mechanism capable of handling partial reads, dynamic memory allocation and efficient state management without relying on the C standard library.

The project introduces several fundamental concepts of low-level systems programming:

* File descriptors and POSIX I/O
* Static variables and persistent state
* Buffered reading strategies
* Dynamic memory management
* String manipulation without `libft`
* Modular software architecture
* Defensive programming and error handling

The resulting function provides the following public interface:

```c
char	*get_next_line(int fd);
```

Each call returns the next available line from the specified file descriptor, including the terminating newline character (`'\n'`) when present. When the end of the file is reached or an error occurs, the function returns `NULL`.

Unlike implementations that load an entire file into memory, this project follows an incremental reading strategy. Data is read only when necessary and any unread characters are preserved internally for subsequent calls, minimizing unnecessary system calls while maintaining a small memory footprint.

Although the mandatory implementation supports a single reading state, the project bonus extends the same architecture to handle multiple file descriptors simultaneously without modifying the core reading algorithm.

## Learning Objectives

The primary goal of **get_next_line** is not simply to read lines from a file, but to develop a deeper understanding of how buffered input and persistent program state can be implemented at a low level using only a minimal set of system functions.

Throughout this project, several fundamental concepts of systems programming are explored:

### File Descriptors

Understanding how UNIX file descriptors abstract different input sources (files, standard input, pipes, sockets, etc.) and how the `read()` system call interacts with them.

### Buffered Reading

Designing a reading strategy that minimizes system calls by reading data in fixed-size chunks while preserving any unread characters for future function calls.

### Persistent State

Using a static variable to maintain the reading state between successive calls to `get_next_line()`, allowing the function to continue exactly where the previous call stopped.

### Dynamic Memory Management

Allocating, resizing and releasing memory safely throughout the execution while preventing:

* Memory leaks
* Invalid memory accesses
* Double frees
* Lost allocations

### Modular Software Design

Breaking the implementation into small helper functions, each responsible for a single task, resulting in code that is easier to understand, maintain and extend.

### Defensive Programming

Handling invalid file descriptors, allocation failures, end-of-file conditions and unexpected read errors without compromising the internal state of the function.

### Algorithm Design

Developing an incremental reading algorithm capable of returning complete lines while reading only the minimum amount of data required from the input stream.

### Bonus: Multiple File Descriptors

Extending the same architecture to support multiple simultaneous file descriptors by maintaining an independent reading state for each descriptor, without modifying the core algorithm.

## Architecture Overview

One of the primary design goals of this implementation was to build a **simple, modular and maintainable architecture** before writing any code.

Instead of concentrating all the logic inside `get_next_line()`, the implementation is divided into several small helper functions, each with a single, well-defined responsibility.

This modular approach provides several advantages:

* Easier to understand and maintain.
* Simpler debugging and testing.
* Better separation of responsibilities.
* Full compliance with the 42 Norm.
* Straightforward extension to the Bonus implementation.

---

### High-Level Architecture

```text
                    get_next_line()
                           │
                           ▼
                    read_file(fd)
                           │
                    ┌──────┴──────┐
                    │             │
                    ▼             ▼
               fill_cache()   extract_line()
                    │
                    ▼
               clean_cache()
```

The public function **`get_next_line()`** acts only as a coordinator.

Each helper performs a single task and returns its result to the next stage of the pipeline.

---

### Execution Flow

Each call to `get_next_line()` follows the same sequence:

```text
Validate arguments
        │
        ▼
Read additional data if needed
        │
        ▼
Extract one complete line
        │
        ▼
Preserve unread characters
        │
        ▼
Return the extracted line
```

This execution model ensures that only the minimum amount of data required is read from the file descriptor while preserving any remaining characters for future calls.

---

### Function Responsibilities

| Function          | Responsibility                                                                             |
| ----------------- | ------------------------------------------------------------------------------------------ |
| `get_next_line()` | Public API. Coordinates the complete reading process.                                      |
| `read_file()`     | Initializes the cache (if necessary) and prepares the reading buffer.                      |
| `fill_cache()`    | Reads from the file descriptor until a newline is found or the end of the file is reached. |
| `extract_line()`  | Builds and returns the next complete line from the cache.                                  |
| `clean_cache()`   | Removes the returned line from the cache while preserving unread characters.               |

---

### Utility Functions

To keep the implementation independent from **libft**, a small collection of utility functions was implemented.

| Function       | Purpose                                                                        |
| -------------- | ------------------------------------------------------------------------------ |
| `ft_strlen()`  | Returns the length of a string.                                                |
| `ft_strdup()`  | Creates a duplicate of a string.                                               |
| `ft_strchr()`  | Searches for a character inside a string.                                      |
| `ft_substr()`  | Extracts a substring from a string.                                            |
| `ft_strjoin()` | Concatenates two strings while transferring ownership of the first allocation. |

---

### Design Philosophy

The implementation follows a **Single Responsibility Principle (SRP)** approach, where every function performs exactly one task.

Rather than implementing a large monolithic function, the project is organized as a small processing pipeline.

Each helper receives a well-defined input, performs one specific operation and returns a predictable result to the next stage.

This design significantly improves readability, reduces coupling between components and makes future modifications—such as the Bonus implementation supporting multiple file descriptors—possible without changing the core algorithm.

## Reading Algorithm

The core idea behind **get_next_line()** is simple:

> Read only as much data as necessary to return exactly one line while preserving any unread characters for future calls.

To achieve this, the implementation maintains an internal **cache** that stores data already read from the file descriptor but not yet returned to the caller.

Each invocation follows the same sequence:

```text
File Descriptor
      │
      ▼
read()
      │
      ▼
Append data to cache
      │
      ▼
Newline found?
      │
 ┌────┴────┐
 │         │
 │ No      │ Yes
 │         │
 ▼         ▼
Read   Extract line
again        │
             ▼
      Clean cache
             │
             ▼
      Return line
```

---

### Example Execution

Consider the following input file:

```text
Hello World!
Welcome to 42.
get_next_line
```

For simplicity, assume that the entire file is read during the first call.

---

#### First Call

Initial state:

```text
cache = NULL
```

After reading:

```text
cache =
---------------------------------
Hello World!\n
Welcome to 42.\n
get_next_line
---------------------------------
```

`extract_line()` returns:

```text
Hello World!\n
```

`clean_cache()` preserves the remaining data:

```text
cache =
---------------------------------
Welcome to 42.\n
get_next_line
---------------------------------
```

---

#### Second Call

The cache already contains unread characters.

No additional read is required because a newline is already present.

Current cache:

```text
---------------------------------
Welcome to 42.\n
get_next_line
---------------------------------
```

Returned line:

```text
Welcome to 42.\n
```

Remaining cache:

```text
---------------------------------
get_next_line
---------------------------------
```

---

#### Third Call

The cache contains the last line.

Since no newline is present, `fill_cache()` attempts another read.

The file descriptor reaches **EOF**, so no new data is appended.

Returned line:

```text
get_next_line
```

The cache is now empty and is released.

```text
cache = NULL
```

---

#### Fourth Call

The cache is recreated as an empty string.

`read()` immediately returns **0** (End Of File).

No complete line exists.

The function returns:

```c
NULL
```

---

### Incremental Reading

One of the main requirements of the project is to avoid reading more data than necessary.

Whenever a newline character already exists inside the cache, **no additional call to `read()` is performed**.

For example:

```text
cache =
---------------------------------
Line 2\n
Line 3\n
---------------------------------
```

Since the next line is already available, the function immediately extracts:

```text
Line 2\n
```

without accessing the file descriptor again.

This strategy minimizes system calls while preserving the expected sequential behavior.

---

### Internal Cache Lifecycle

The cache evolves during the execution as follows:

```text
NULL
 │
 ▼
""
 │
 ▼
"Hello\nWorld\n"
 │
 ▼
"World\n"
 │
 ▼
""
 │
 ▼
NULL
```

The cache exists only while unread characters remain.

Once every stored character has been returned to the caller, the allocated memory is released automatically.

---

### Reading Strategy

Unlike implementations that read the entire file into memory, this implementation follows an incremental strategy:

```text
read()
      │
      ▼
Append
      │
      ▼
Newline?
      │
 ┌────┴────┐
 │         │
 │ No      │ Yes
 │         │
 ▼         ▼
Read    Return Line
Again
```

This guarantees:

* Minimal memory usage.
* Minimal number of system calls.
* Correct sequential reading.
* Compatibility with any valid `BUFFER_SIZE`.

---

### Key Takeaways

* The file is never loaded entirely into memory.
* Data is read only when necessary.
* The cache preserves unread characters between successive calls.
* Each invocation returns exactly one line.
* The cache is automatically released once all pending data has been consumed.

## Cache Management & Memory Ownership

One of the most important aspects of **get_next_line()** is the management of dynamically allocated memory.

Rather than reading an entire file into memory, the implementation maintains a single dynamically allocated **cache** containing only the characters that have been read from the file descriptor but have not yet been returned to the caller.

This cache evolves continuously throughout the execution of the program.

---

### Memory Ownership

Every dynamically allocated block has a single owner responsible for releasing it.

| Allocation | Owner             | Released By                       |
| ---------- | ----------------- | --------------------------------- |
| `cache`    | `get_next_line()` | `clean_cache()` or error handling |
| `buffer`   | `read_file()`     | `read_file()`                     |
| `line`     | Caller            | Caller (`free()`)                 |

This ownership model ensures that every allocation has one clear responsibility, preventing both memory leaks and double-free errors.

---

### Memory Lifecycle

The following diagram illustrates the lifecycle of the cache during several calls to `get_next_line()`.

#### Initial State

```text
Heap

cache
  │
  ▼
NULL
```

No memory has been allocated yet.

---

#### After the First Read

```text
Heap

cache
  │
  ▼
+----------------------------------+
| Hello\nWorld\n42\n\0             |
+----------------------------------+
```

The cache now contains all characters read from the file descriptor.

---

#### extract_line()

A new allocation is created for the returned line.

```text
Heap

cache                     line
  │                         │
  ▼                         ▼
+--------------------+    +----------+
| Hello\nWorld\n42\n |    | Hello\n  |
+--------------------+    +----------+
```

Both allocations coexist temporarily.

---

#### clean_cache()

The original cache is replaced with a new allocation containing only the unread characters.

```text
Heap

cache                     line
  │                         │
  ▼                         ▼
+--------------+          +----------+
| World\n42\n  |          | Hello\n  |
+--------------+          +----------+
```

The previous cache allocation has already been released.

---

#### Caller

Once the returned line is no longer needed, the caller becomes responsible for releasing it.

```c
char *line;

line = get_next_line(fd);

/* Process the line */

free(line);
```

After `free(line)`, only the cache remains allocated.

---

#### End Of File

Once every character has been returned, the cache is automatically released.

```text
Heap

cache
  │
  ▼
NULL
```

No dynamically allocated memory remains.

---

### Ownership Transfer

One important design decision of this implementation is that `ft_strjoin()` transfers the ownership of its first argument.

```text
Before

cache ----+
          │
          ▼
     +---------+

buffer
          │
          ▼
     +---------+

          │
          ▼
      ft_strjoin()

After

cache
   │
   ▼
+----------------+

(old cache released)
```

This avoids unnecessary temporary allocations and guarantees that only one valid cache exists at any time.

---

### Memory Safety

The implementation has been designed to safely handle:

* Allocation failures (`malloc()` returning `NULL`)
* Read errors
* End-of-file conditions
* Empty files
* Consecutive newline characters
* Any valid `BUFFER_SIZE`

Whenever an unrecoverable error occurs, all owned resources are released before returning `NULL`, ensuring that no memory allocated by the implementation remains unreachable.

---

### Memory Evolution

The complete evolution of the cache can be summarized as follows:

```text
NULL
 │
 ▼
""
 │
 ▼
"Hello\nWorld\n42\n"
 │
 ▼
"World\n42\n"
 │
 ▼
"42\n"
 │
 ▼
""
 │
 ▼
NULL
```

At every stage, the cache stores **only the unread portion** of the input stream.

---

### Key Takeaways

* Every allocation has a single owner.
* The cache stores only unread data.
* Previous cache allocations are released immediately after being replaced.
* The caller owns the returned line.
* Memory ownership is never shared.
* The implementation is designed to prevent memory leaks and double frees.


### Visual Memory Walkthrough

The following diagrams illustrate the conceptual state of the heap during the execution of `get_next_line()`.

The memory addresses shown are purely illustrative and are intended only to explain ownership and lifetime of dynamically allocated objects.

---

#### Step 1 — Initial State

Before the first call, no memory has been allocated.

```text
                HEAP

        (empty)

cache
  │
  ▼
 NULL
```

---

#### Step 2 — First Read

`read_file()` allocates the cache and fills it with the characters read from the file descriptor.

```text
                HEAP

0x1000
+--------------------------------------+
| Hello\nWorld\n42\n\0                 |
+--------------------------------------+
 ^
 |
cache
```

At this point:

* Only one allocation exists.
* The cache owns the entire input read so far.

---

#### Step 3 — Extracting the First Line

`extract_line()` creates a second allocation.

```text
                HEAP

0x1000                         0x2000
+------------------------+     +-----------+
| Hello\nWorld\n42\n\0   |     | Hello\n\0 |
+------------------------+     +-----------+
 ^                                  ^
 |                                  |
cache                              line
```

Notice that **both allocations coexist**.

The original cache has not yet been modified.

---

#### Step 4 — Cleaning the Cache

`clean_cache()` allocates a new cache containing only the unread characters.

```text
                HEAP

0x3000                         0x2000
+----------------+             +-----------+
| World\n42\n\0  |             | Hello\n\0 |
+----------------+             +-----------+
 ^                                  ^
 |                                  |
cache                              line
```

The previous cache (`0x1000`) has already been released.

Only the unread characters remain.

---

#### Step 5 — Returning to the Caller

Ownership of the returned line is transferred to the caller.

```text
                HEAP

0x3000                         0x2000
+----------------+             +-----------+
| World\n42\n\0  |             | Hello\n\0 |
+----------------+             +-----------+
 ^                                  ^
 |                                  |
cache                          caller
```

The implementation will never free `line`.

The caller becomes fully responsible for releasing it.

```c
char *line;

line = get_next_line(fd);

/* Process the line */

free(line);
```

---

#### Step 6 — Next Call

The process repeats.

The cache now contains:

```text
World\n42\n
```

Another allocation is created:

```text
                HEAP

0x3000                         0x4000
+----------------+             +-----------+
| World\n42\n\0  |             | World\n\0 |
+----------------+             +-----------+
```

---

#### Step 7 — End Of File

Once every character has been returned, the cache is released.

```text
                HEAP

(empty)

cache
  │
  ▼
 NULL
```

At this point, no memory allocated by the implementation remains alive.

The only possible allocation still existing is the last returned line, which is owned by the caller.

## Memory Layout (Stack vs Heap)

Understanding the interaction between the **Stack**, the **Heap** and the **static storage area** is essential to fully understand how `get_next_line()` works.

Although the function appears simple from its public interface, several memory regions cooperate internally to preserve the reading state between successive calls.

---

### Memory Regions

The implementation uses three different memory regions.

| Region             | Contents                                                 | Lifetime                 |
| ------------------ | -------------------------------------------------------- | ------------------------ |
| **Stack**          | Local variables (`fd`, `line`, `buffer`, pointers, etc.) | One function call        |
| **Heap**           | Dynamically allocated strings (`cache`, `line`)          | Until explicitly freed   |
| **Static Storage** | Static pointer `cache`                                   | Entire program execution |

Each region has a different purpose and lifetime.

---

### Before Calling `get_next_line()`

Initially, only the static pointer exists.

```text
               STATIC STORAGE

        +----------------------+
cache ─▶|        NULL          |
        +----------------------+


                 STACK

        (empty)


                 HEAP

        (empty)
```

No dynamic memory has been allocated.

---

### During the First Call

The stack frame is created.

```text
                STACK

+-------------------------------+
| fd = 3                        |
| line = NULL                   |
| buffer -------------------+   |
+---------------------------|---+
                            |
                            ▼

                HEAP

0x1000
+-------------------------------+
| BUFFER_SIZE bytes             |
+-------------------------------+
```

At this moment:

* `buffer` lives on the Heap.
* The pointer itself lives on the Stack.

---

### After Reading the File

The cache is created.

```text
             STATIC STORAGE

+-----------------------------+
cache -------------------------+
+-----------------------------+
                              |
                              ▼

                HEAP

0x2000
+-------------------------------------------+
| Hello\nWorld\n42\n\0                      |
+-------------------------------------------+
```

Notice that:

* the **pointer** is stored in static memory;
* the **characters** are stored on the Heap.

This distinction is extremely important.

---

### Extracting One Line

A second allocation is created.

```text
               STATIC STORAGE

cache --------------------------+
                                |
                                ▼

                HEAP

0x2000                          0x3000
+----------------------+        +-----------+
| Hello\nWorld\n42\n   |        | Hello\n\0 |
+----------------------+        +-----------+
        ^                              ^
        |                              |
      cache                           line
```

Now two heap allocations coexist.

Each has a different owner.

---

### Returning to the Caller

The ownership of the extracted line changes.

```text
                  STACK

+-------------------------------+
| line ----------------------+   |
| fd = 3                     |   |
+----------------------------|---+
                             |
                             ▼

                HEAP

0x3000
+-----------+
| Hello\n\0 |
+-----------+

Caller owns this allocation.
```

The implementation will **never free this block**.

Only the caller may execute:

```c
free(line);
```

---

### Cleaning the Cache

The old cache is discarded.

A new cache is allocated containing only the unread characters.

```text
             STATIC STORAGE

cache -----------------------------+
                                   |
                                   ▼

                HEAP

0x4000
+----------------------+
| World\n42\n\0        |
+----------------------+
```

The previous cache allocation has already been released.

There is never more than one valid cache allocation.

---

### End Of File

When every pending character has been returned:

```text
            STATIC STORAGE

+----------------------+
cache ─▶ NULL
+----------------------+


                HEAP

(empty)
```

Only the caller may still own the final returned line.

Once that line is freed:

```text
                HEAP

(empty)
```

No memory allocated by `get_next_line()` remains alive.

---

### Memory Ownership Summary

```text
                STACK

line -----------------------------+
                                  |
                                  ▼

                HEAP

+------------------+
| Returned line    |
+------------------+

Owner:
    Caller

--------------------------------------------

            STATIC STORAGE

cache ----------------------------+
                                  |
                                  ▼

                HEAP

+------------------+
| Remaining data   |
+------------------+

Owner:
    get_next_line()
```

Ownership is **never shared**.

Each allocation has exactly one owner responsible for releasing it.

This design prevents:

* Memory leaks
* Double frees
* Dangling pointers
* Lost allocations

while keeping the implementation simple and predictable.

## Complete Execution Trace

The following sequence illustrates the complete lifecycle of a single call to `get_next_line()`.

Unlike the previous sections, which described the architecture and memory model separately, this walkthrough combines:

* Function calls
* Data flow
* Memory allocations
* Cache evolution
* Ownership transfers

into one complete execution trace.

---

### Initial State

Before the first call:

```text
                     STATIC STORAGE

             +----------------------+
cache ------>|        NULL          |
             +----------------------+


                        │
                        ▼

User
 │
 ▼
get_next_line(fd)
```

No memory has been allocated.

---

## #Step 1 — Parameter Validation

```text
User
 │
 ▼
get_next_line(fd)
 │
 ▼
Validate:

• fd >= 0
• BUFFER_SIZE > 0
• read(fd, NULL, 0)
```

If validation fails:

```text
free(cache)

↓

return NULL
```

No further processing occurs.

---

### Step 2 — Preparing the Cache

```text
get_next_line()
        │
        ▼
read_file()
```

If this is the first invocation:

```text
cache

NULL

↓

ft_strdup("")

↓

""
```

The cache now exists but contains no data.

---

### Step 3 — Reading From the File

```text
read_file()
      │
      ▼
fill_cache()
      │
      ▼
read(fd)
```

Suppose the file contains:

```text
Hello
World
42
```

After the first read:

```text
STATIC

cache
 │
 ▼

HEAP

+----------------------------+
| Hello\nWorld\n42\n\0       |
+----------------------------+
```

---

### Step 4 — Is A Complete Line Available?

```text
cache

Hello\nWorld\n42\n
```

`fill_cache()` checks:

```text
Newline found?

      │
 ┌────┴────┐
 │         │
 │  Yes    │
 │         │
 ▼         ▼
Stop     Continue reading
```

Since a newline already exists, no further `read()` calls are performed.

---

### Step 5 — Extracting One Line

```text
extract_line(cache)
```

Creates a second allocation.

```text
STATIC

cache
 │
 ▼

HEAP

0x1000

+----------------------------+
| Hello\nWorld\n42\n         |
+----------------------------+

                +

0x2000

+-----------+
| Hello\n\0 |
+-----------+

        ▲
        │
      line
```

Two allocations now coexist.

---

### Step 6 — Cleaning The Cache

The already returned line must disappear from the cache.

```text
clean_cache(cache)
```

New cache:

```text
STATIC

cache
 │
 ▼

HEAP

0x3000

+----------------+
| World\n42\n\0  |
+----------------+

line

0x2000

+-----------+
| Hello\n\0 |
+-----------+
```

The previous cache allocation (`0x1000`) has already been released.

---

### Step 7 — Returning The Line

Control returns to the caller.

```text
Caller

line
 │
 ▼

+-----------+
| Hello\n\0 |
+-----------+
```

Ownership changes.

```text
Before

Owner:

get_next_line()

↓

After

Owner:

Caller
```

Only the caller may execute:

```c
free(line);
```

---

### Step 8 — Second Invocation

The cache already contains:

```text
World\n42\n
```

The execution restarts:

```text
get_next_line()

↓

read_file()

↓

fill_cache()
```

Since a newline already exists:

```text
read()

NOT CALLED
```

The next line is immediately extracted.

---

### Step 9 — End Of File

Eventually the cache becomes:

```text
42
```

No newline remains.

`fill_cache()` performs one final read.

```text
read()

↓

0

↓

EOF
```

The remaining characters are still returned.

Afterwards:

```text
cache

↓

NULL
```

---

### Complete Execution Pipeline

```text
                 User

                  │

                  ▼

          get_next_line()

                  │

        Validate Parameters

                  │

                  ▼

            read_file()

                  │

                  ▼

            fill_cache()

                  │

                  ▼

               read()

                  │

                  ▼

        Append Data To Cache

                  │

                  ▼

        Newline Available?

          │             │

         Yes           No

          │             │

          ▼             │

    extract_line()      │

          │             │

          ▼             │

    clean_cache() ◄─────┘

          │

          ▼

      return line

          │

          ▼

      Caller owns line

          │

          ▼

        free(line)
```

---

### Key Takeaways

* Every call follows exactly the same execution path.
* Reading stops as soon as one complete line becomes available.
* The cache always contains only unread characters.
* Memory ownership is transferred exactly once.
* The caller becomes responsible for freeing the returned line.
* The Bonus implementation reuses exactly the same pipeline.

## File Structure

The project is intentionally organized into a small set of source files, each with a clearly defined responsibility.

This separation keeps the implementation simple, improves readability and fully complies with the 42 Norm.

```text
get_next_line/
├── Makefile
├── README.md
│
├── get_next_line.h
├── get_next_line.c
├── get_next_line_utils.c
│
├── get_next_line_bonus.h
├── get_next_line_bonus.c
└── get_next_line_utils_bonus.c
```

---

### Project Organization

#### `get_next_line.h`

The public header of the project.

It contains:

* Public function prototype.
* Utility function prototypes.
* Required standard library includes.
* `BUFFER_SIZE` default definition.
* Compile-time validation of `BUFFER_SIZE`.

This header defines the public interface exposed by the implementation.

---

#### `get_next_line.c`

Contains the complete implementation of the reading algorithm.

Its responsibilities include:

* Public API (`get_next_line()`).
* Reading data from the file descriptor.
* Building the internal cache.
* Extracting complete lines.
* Preserving unread characters.

The file contains only the functions directly involved in the reading process.

---

#### `get_next_line_utils.c`

Contains the auxiliary string manipulation functions required by the project.

Since the use of **libft** is forbidden, these helper functions provide only the minimal functionality needed by the implementation.

Implemented utilities:

* `ft_strlen()`
* `ft_strdup()`
* `ft_strchr()`
* `ft_substr()`
* `ft_strjoin()`

These functions remain independent from the reading algorithm and can be tested separately.

---

#### Bonus Files

The Bonus implementation mirrors exactly the same structure.

```text
get_next_line_bonus.c

↓

Same algorithm

↓

Different cache management
```

The only architectural difference is the replacement of the single static cache with one cache per file descriptor.

This design allows the Bonus implementation to reuse the same reading algorithm without modifying any of its helper functions.

---

### Separation of Responsibilities

The project follows a modular organization where each file has a single responsibility.

| File                          | Responsibility                       |
| ----------------------------- | ------------------------------------ |
| `get_next_line.c`             | Reading algorithm                    |
| `get_next_line_utils.c`       | String utilities                     |
| `get_next_line.h`             | Public interface                     |
| `get_next_line_bonus.c`       | Multi-file descriptor implementation |
| `get_next_line_utils_bonus.c` | Bonus utilities                      |
| `get_next_line_bonus.h`       | Bonus public interface               |

This organization keeps the implementation easy to navigate while reducing coupling between modules.

---

### Key Takeaways

* The project is divided into small, cohesive modules.
* Each source file has a single responsibility.
* Utility functions remain independent from the reading algorithm.
* The Bonus implementation reuses the same architecture as the mandatory version.
* The project structure closely reflects the logical architecture presented in the previous sections.

## Helper Functions

The implementation follows a modular design where each helper function performs a single well-defined task.

Rather than concentrating the entire algorithm inside `get_next_line()`, the project is divided into small reusable components with explicit responsibilities.

Each helper acts as a stage in the reading pipeline and communicates with the next stage through clearly defined inputs and outputs.

---

### Core Reading Functions

#### `read_file()`

**Purpose**

Initializes the cache (when necessary), allocates the temporary reading buffer and coordinates the reading process.

**Input**

* File descriptor.
* Current cache.

**Output**

* Updated cache containing all characters read so far.

**Guarantees**

* Allocates the buffer.
* Initializes an empty cache on the first call.
* Releases the buffer before returning.
* Returns `NULL` on allocation or read failure.

---

#### `fill_cache()`

**Purpose**

Reads data from the file descriptor until either:

* a newline character is found, or
* the end of the file is reached.

**Input**

* File descriptor.
* Current cache.
* Temporary buffer.

**Output**

* Updated cache.

**Guarantees**

* Never reads more than necessary.
* Appends every successful read to the cache.
* Preserves previously unread characters.
* Frees the cache if a read error occurs.

---

#### `extract_line()`

**Purpose**

Builds the next complete line from the cache.

**Input**

* Current cache.

**Output**

* Newly allocated line.

**Guarantees**

* Includes the terminating newline when present.
* Returns the remaining characters unchanged.
* Returns `NULL` only when no complete line can be produced or an allocation fails.

---

#### `clean_cache()`

**Purpose**

Removes the returned line from the cache while preserving every unread character.

**Input**

* Current cache.

**Output**

* New cache.

**Guarantees**

* Releases the previous cache allocation.
* Preserves unread data.
* Returns `NULL` when no characters remain.

---

### Utility Functions

#### `ft_strlen()`

Returns the number of characters in a string.

---

#### `ft_strdup()`

Creates a dynamically allocated copy of a string.

---

#### `ft_strchr()`

Searches a string for a specific character.

Used by the implementation to determine whether the cache already contains a complete line.

---

#### `ft_substr()`

Extracts a portion of a string into a newly allocated buffer.

Used for both:

* extracting the returned line;
* preserving unread characters.

---

#### `ft_strjoin()`

Concatenates two strings into a newly allocated buffer.

Unlike the standard implementation, this version transfers ownership of its first argument by releasing the previous allocation before returning the newly created string.

This greatly simplifies memory management throughout the reading algorithm.

---

### Function Relationships

The helper functions form a simple processing pipeline.

```text
                get_next_line()

                       │

                       ▼

                  read_file()

                       │

                       ▼

                 fill_cache()

                       │

                       ▼

                extract_line()

                       │

                       ▼

                 clean_cache()
```

Each helper performs exactly one operation before passing the result to the next stage.

This minimizes coupling while making the implementation significantly easier to debug and maintain.

---

### Function Contracts

One important design principle of this implementation is that every helper has a clearly defined contract.

| Function         | Receives                | Returns       | Side Effects                  |
| ---------------- | ----------------------- | ------------- | ----------------------------- |
| `read_file()`    | `fd`, `cache`           | Updated cache | Allocates and releases buffer |
| `fill_cache()`   | `fd`, `cache`, `buffer` | Updated cache | Reads from file descriptor    |
| `extract_line()` | Cache                   | Line          | Allocates returned line       |
| `clean_cache()`  | Cache                   | New cache     | Releases previous cache       |
| `ft_strjoin()`   | Two strings             | Joined string | Releases first allocation     |

Because each function has a single responsibility and a predictable contract, the overall implementation remains modular, easy to reason about and straightforward to extend.

---

### Key Takeaways

* Every helper has a single responsibility.
* Helper functions communicate through simple and predictable contracts.
* Memory ownership is clearly defined for every allocation.
* Utility functions remain completely independent from the reading algorithm.
* The modular design greatly simplifies both testing and future extensions such as the Bonus implementation.

## Algorithm Complexity

Although `get_next_line()` appears to be a simple function, its performance depends on how data is buffered and how many characters must be processed before a complete line can be returned.

This implementation follows an incremental reading strategy, ensuring that only the minimum amount of data required is read from the file descriptor.

---

### Time Complexity

The overall time complexity is proportional to the number of processed characters.

| Operation                        | Complexity |
| -------------------------------- | ---------: |
| Reading from the file descriptor |   **O(n)** |
| Searching for a newline          |   **O(n)** |
| Extracting a line                |   **O(n)** |
| Cleaning the cache               |   **O(n)** |

Where **n** represents the number of characters currently stored in the cache.

Since every character is processed a limited number of times during its lifetime, the overall algorithm remains linear with respect to the size of the input.

---

### Space Complexity

The implementation only stores the data that has already been read but not yet returned.

The required memory is therefore:

```text
O(BUFFER_SIZE + cache)
```

where:

* **BUFFER_SIZE** is the temporary reading buffer.
* **cache** contains only the unread portion of the input stream.

Unlike implementations that load an entire file into memory, this approach keeps memory usage proportional to the amount of unread data.

---

### Reading Efficiency

One of the main objectives of the project is to minimize system calls.

Whenever a newline character already exists inside the cache, no additional call to `read()` is performed.

```text
cache

Hello\nWorld\n

↓

extract_line()

↓

return "Hello\n"

↓

No read() required
```

This significantly reduces unnecessary disk access while preserving the expected sequential behavior.

---

### Cache Evolution

During execution, the cache continuously shrinks as complete lines are returned.

```text
NULL

↓

""

↓

"Hello\nWorld\n42\n"

↓

"World\n42\n"

↓

"42\n"

↓

""

↓

NULL
```

At no point does the implementation intentionally retain characters that have already been returned to the caller.

---

### Scalability

The implementation has been designed to work correctly regardless of the chosen `BUFFER_SIZE`.

Typical examples include:

| BUFFER_SIZE | Behaviour                                                   |
| ----------: | ----------------------------------------------------------- |
|         `1` | Reads one byte at a time. Maximum number of `read()` calls. |
|        `42` | Balanced behaviour.                                         |
|      `4096` | Fewer system calls for large files.                         |
|  Very large | Reads larger chunks while preserving the same algorithm.    |

Because the reading strategy is independent of the buffer size, the implementation behaves correctly for any positive `BUFFER_SIZE` supported by the system.

---

### Design Considerations

Rather than optimizing for the smallest possible number of operations, this implementation prioritizes:

* Correctness
* Readability
* Modularity
* Memory safety
* Compliance with the 42 Norm

The chosen architecture favors maintainability while still providing efficient sequential reading.

---

### Key Takeaways

* The algorithm processes the input incrementally.
* Reading stops as soon as one complete line is available.
* Memory usage depends only on unread data.
* The implementation works with any valid `BUFFER_SIZE`.
* The design favors clarity and robustness without sacrificing efficiency.

## Build & Usage

The project can be compiled using the provided **Makefile** or directly with the C compiler.

---

### Requirements

* C Compiler (`cc`)
* POSIX-compliant operating system
* Standard C Library
* GNU Make (recommended)

---

### Compilation

#### Mandatory Version

Compile the mandatory implementation:

```bash
make
```

This generates the executable:

```text
Get_Next_Line
```

---

#### Bonus Version

Compile the Bonus implementation:

```bash
make bonus
```

This generates:

```text
Get_Next_Line_Bonus
```

The Bonus version supports multiple file descriptors simultaneously while preserving an independent reading state for each descriptor.

---

### Cleaning the Project

Remove object files:

```bash
make clean
```

Remove all generated files:

```bash
make fclean
```

Rebuild everything:

```bash
make re
```

---

### BUFFER_SIZE

The project uses the compile-time macro `BUFFER_SIZE` to determine how many bytes are requested from the operating system on each call to `read()`.

If no value is specified during compilation, a default value is provided by the project.

Example:

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 \
get_next_line.c get_next_line_utils.c
```

Different values may be used to evaluate the implementation.

Examples:

```bash
-D BUFFER_SIZE=1
```

```bash
-D BUFFER_SIZE=42
```

```bash
-D BUFFER_SIZE=1024
```

```bash
-D BUFFER_SIZE=100000
```

The implementation has been designed to behave correctly for any positive value of `BUFFER_SIZE`.

---

### Public Interface

The project exposes a single public function:

```c
char *get_next_line(int fd);
```

Parameters:

| Parameter | Description                  |
| --------- | ---------------------------- |
| `fd`      | File descriptor to read from |

Return values:

| Return Value | Meaning              |
| ------------ | -------------------- |
| Pointer      | Newly allocated line |
| `NULL`       | End of file or error |

The returned string includes the terminating newline character (`'\n'`) whenever one exists in the input stream.

---

### Example

```c
#include <fcntl.h>
#include <stdlib.h>
#include "get_next_line.h"

int	main(void)
{
	int		fd;
	char	*line;

	fd = open("example.txt", O_RDONLY);
	if (fd < 0)
		return (1);
	while ((line = get_next_line(fd)))
	{
		/* Process the line */

		free(line);
	}
	close(fd);
	return (0);
}
```

---

### Reading From Standard Input

`get_next_line()` can also read directly from the standard input.

Example:

```c
char *line;

line = get_next_line(STDIN_FILENO);
```

This allows the same implementation to read from:

* Files
* Standard input
* Pipes
* Any valid POSIX file descriptor

without requiring any modification to the algorithm.

---

### Debug Builds

During development, different build configurations can be useful.

Compile with debugging symbols:

```bash
make debug
```

Compile the Bonus version with debugging symbols:

```bash
make debug_bonus
```

Compile with AddressSanitizer:

```bash
make asan
```

These targets are intended for development and testing and are not required for the final project submission.

---

### Key Takeaways

* The project can be compiled in both Mandatory and Bonus modes.
* `BUFFER_SIZE` is configurable at compile time.
* The implementation supports any valid file descriptor.
* The same public API is used in both Mandatory and Bonus versions.
* Additional build targets simplify debugging during development.

## Mandatory vs Bonus

One of the main objectives during the design of this implementation was to build an architecture that could support the Bonus requirements without requiring a complete redesign of the reading algorithm.

Rather than implementing two independent solutions, the project was structured so that the **Mandatory** and **Bonus** versions share exactly the same execution pipeline.

The only difference lies in how the internal reading state is stored.

---

### Mandatory Implementation

The mandatory version maintains a single persistent reading state.

```c
static char	*cache;
```

Conceptually:

```text
                STATIC STORAGE

          +----------------------+
cache ───▶| Remaining characters |
          +----------------------+
```

This implementation assumes that only one file descriptor is being tracked at any given time.

Each call updates the same cache until the end of the file is reached.

---

### Bonus Implementation

The Bonus extends the same idea by maintaining one independent cache for every file descriptor.

```c
static char	*cache[OPEN_MAX];
```

Conceptually:

```text
                STATIC STORAGE

cache[0]  ──▶ NULL
cache[1]  ──▶ NULL
cache[2]  ──▶ NULL

cache[3]  ──▶ "Hello\n42\n"
cache[4]  ──▶ "World\n"
cache[5]  ──▶ NULL
```

Each file descriptor now owns its own independent reading state.

---

### Independent Reading States

Suppose three files are opened simultaneously.

```text
fd3 → file_A.txt

fd4 → file_B.txt

fd5 → file_C.txt
```

The Bonus implementation allows the following sequence:

```text
get_next_line(fd3)

↓

get_next_line(fd4)

↓

get_next_line(fd5)

↓

get_next_line(fd3)

↓

get_next_line(fd4)

↓

get_next_line(fd5)
```

Each descriptor continues reading exactly where it stopped previously.

No cache is shared between descriptors.

---

### Architecture Comparison

```text
Mandatory

                get_next_line()

                       │

                static cache

                       │

                Reading Pipeline

──────────────────────────────────────

Bonus

                get_next_line()

                       │

             static cache[OPEN_MAX]

                       │

                Reading Pipeline
```

Notice that the **reading pipeline remains identical**.

Only the storage of the internal state changes.

---

### Why This Design Matters

Because every helper function operates only on the cache passed as an argument, none of them needed to be redesigned for the Bonus implementation.

The following functions remain unchanged:

* `read_file()`
* `fill_cache()`
* `extract_line()`
* `clean_cache()`
* All utility functions

Only the static storage used by `get_next_line()` changes.

This separation of concerns was one of the primary architectural goals of the project.

---

### Benefits

Designing the project around an isolated cache abstraction provides several advantages.

* The Bonus implementation reuses the Mandatory algorithm.
* Helper functions remain completely independent.
* No code duplication is required.
* The reading pipeline remains unchanged.
* Memory ownership rules remain exactly the same.
* The implementation is easier to maintain and test.

---

### Execution Example

The following sequence illustrates the independence between descriptors.

```text
Initial State

cache[3] = NULL
cache[4] = NULL

──────────────────────────────

get_next_line(fd3)

↓

cache[3]

Hello\n42\n

──────────────────────────────

get_next_line(fd4)

↓

cache[4]

World\nMadrid\n

──────────────────────────────

get_next_line(fd3)

↓

Returns

42\n

──────────────────────────────

get_next_line(fd4)

↓

Returns

Madrid\n
```

Neither descriptor modifies the state of the other.

---

### Key Takeaways

* The Bonus does not introduce a new reading algorithm.
* Only the storage strategy changes.
* Each file descriptor owns its own cache.
* The helper functions are reused without modification.
* The modular architecture makes the Bonus implementation almost trivial once the Mandatory version is complete.


## Design Decisions

Throughout the implementation of **get_next_line()**, several architectural decisions were made with the goal of producing code that is simple, modular, memory-safe and easy to extend.

Rather than focusing only on satisfying the project requirements, the implementation was designed around a set of engineering principles intended to improve maintainability and readability.

---

### Modular Architecture

Instead of implementing the entire algorithm inside a single function, the project is divided into a small set of helpers.

```text
get_next_line()

↓

read_file()

↓

fill_cache()

↓

extract_line()

↓

clean_cache()
```

Each function performs exactly one task before passing the result to the next stage.

This reduces complexity while making debugging significantly easier.

---

### Single Responsibility Principle

Every helper has one clearly defined responsibility.

| Function         | Responsibility                    |
| ---------------- | --------------------------------- |
| `read_file()`    | Prepare reading                   |
| `fill_cache()`   | Read until a complete line exists |
| `extract_line()` | Build the returned line           |
| `clean_cache()`  | Preserve unread characters        |

Keeping each helper focused on one operation minimizes coupling and simplifies future modifications.

---

### Cache-Centric Design

The entire implementation revolves around a single concept:

> **The cache always contains every unread character.**

No helper needs to know where the cache comes from.

It simply receives a pointer, performs its operation and returns the updated cache.

Because of this abstraction, supporting multiple file descriptors only required replacing:

```c
static char *cache;
```

with

```c
static char *cache[OPEN_MAX];
```

without modifying the reading pipeline.

---

### Memory Ownership

One of the main design goals was to make ownership explicit.

Every allocation has exactly one owner.

| Allocation    | Owner             |
| ------------- | ----------------- |
| Cache         | `get_next_line()` |
| Buffer        | `read_file()`     |
| Returned line | Caller            |

Ownership is never shared.

This greatly simplifies memory management while preventing:

* Memory leaks
* Double frees
* Dangling pointers

---

### Ownership Transfer in `ft_strjoin()`

Unlike the traditional implementation of `strjoin()`, this project deliberately transfers ownership of the first allocation.

```text
Before

cache + buffer

↓

ft_strjoin()

↓

new cache

↓

free(old cache)
```

This decision avoids repetitive `free()` calls throughout the reading algorithm and centralizes memory ownership inside a single helper.

As a result, the implementation becomes both simpler and less error-prone.

---

### Incremental Reading

The implementation never attempts to read the entire file.

Instead, it follows a simple strategy:

```text
Read

↓

Newline found?

↓

Yes

↓

Return one line

↓

Preserve the rest
```

Only the minimum amount of data required to build the next line is requested from the operating system.

---

### Defensive Programming

Several defensive checks are performed before processing any data.

Examples include:

* Invalid file descriptors.
* Invalid `BUFFER_SIZE`.
* Failed memory allocations.
* Read errors.
* Empty files.
* End-of-file detection.

Whenever an unrecoverable error occurs, owned resources are released before returning `NULL`.

---

### Norm Compliance

The project was developed while respecting the constraints imposed by the 42 Norm.

Special attention was given to:

* Maximum function length.
* Maximum number of functions per file.
* Readability.
* Clear separation of responsibilities.

Rather than treating these restrictions as limitations, the architecture was designed around them from the beginning.

---

### Bonus-Ready Architecture

One important design objective was ensuring that the Mandatory implementation could naturally evolve into the Bonus implementation.

This resulted in an architecture where:

* Helper functions remain unchanged.
* The reading pipeline remains unchanged.
* Memory ownership remains unchanged.

Only the storage strategy changes.

This demonstrates the benefits of designing software around abstractions instead of concrete implementations.

---

### Engineering Principles

The implementation follows several software engineering principles.

* Single Responsibility Principle (SRP)
* Separation of Concerns
* Explicit Memory Ownership
* Defensive Programming
* Modular Design
* Incremental Processing
* Code Reusability

Although this is a relatively small project, applying these principles results in code that is easier to understand, test and maintain.

---

### Engineering Insight

One of the most valuable lessons learned during this project is that a good software architecture should anticipate future evolution.

By separating the reading algorithm from the storage mechanism, the Mandatory implementation naturally evolved into the Bonus implementation with minimal changes.

Rather than designing two different solutions, the project was designed once and extended through abstraction.

This approach illustrates how thoughtful architectural decisions made early in development can significantly reduce the complexity of future features.

---

### Key Takeaways

* The project was designed before it was implemented.
* Every helper has a single responsibility.
* Memory ownership is always explicit.
* The Bonus implementation reuses the Mandatory architecture.
* A modular design improves readability, maintainability and extensibility.

## Edge Cases

A robust implementation must behave correctly not only under normal conditions but also when confronted with unusual or unexpected inputs.

Throughout the development of this project, special attention was given to handling edge cases safely while preserving memory integrity and maintaining a consistent public interface.

---

### Empty File

Input:

```text
(empty file)
```

Behavior:

* `read()` immediately returns `0`.
* No line can be extracted.
* The cache is released.
* `get_next_line()` returns `NULL`.

Result:

```text
NULL
```

---

### File Containing Only Newlines

Input:

```text
\n
\n
\n
```

Behavior:

Each newline represents a valid line.

Successive calls return:

```text
"\n"

"\n"

"\n"

NULL
```

Even though each line contains no visible characters, every newline is treated as a complete line according to the project specification.

---

### File Without a Final Newline

Input:

```text
Hello
World
Last line
```

Behavior:

The final line is still returned correctly.

Sequence:

```text
Hello\n

World\n

Last line

NULL
```

The last line simply omits the trailing newline because none exists in the input.

---

### BUFFER_SIZE = 1

Behavior:

The implementation reads one byte at a time.

Example:

```text
H

e

l

l

o

\n
```

Although this produces the highest number of system calls, the algorithm behaves exactly the same.

Correctness is preserved regardless of performance.

---

### Large BUFFER_SIZE

Example:

```text
BUFFER_SIZE = 100000
```

Behavior:

The implementation reads larger chunks from the file descriptor.

If multiple lines are read at once, they remain stored inside the cache and are returned one at a time during subsequent calls.

No modification to the algorithm is required.

---

### Invalid File Descriptor

Example:

```c
get_next_line(-1);
```

Behavior:

Validation fails immediately.

```c
read(fd, NULL, 0)
```

detects the invalid descriptor.

The cache is released and the function returns:

```text
NULL
```

---

### Read Error

If `read()` returns `-1` during execution:

* The cache is released.
* Temporary allocations are released.
* The function returns `NULL`.

No memory owned by the implementation remains allocated.

---

### malloc() Failure

Every allocation performed by the implementation is verified.

If any allocation fails:

```text
malloc()

↓

NULL
```

The implementation releases every owned resource before returning `NULL`.

This prevents:

* Memory leaks
* Dangling pointers
* Partial internal states

---

### Consecutive Calls After EOF

Once the end of the file has been reached:

```text
cache

↓

NULL
```

Subsequent calls behave consistently.

```c
get_next_line(fd);
```

continues returning:

```text
NULL
```

No additional memory is allocated.

---

### Reading From Standard Input

The implementation behaves exactly the same when reading from:

```text
STDIN_FILENO
```

Each line entered by the user is returned independently.

The algorithm is completely independent of the input source.

---

### Bonus: Multiple File Descriptors

The Bonus implementation maintains an independent cache for every file descriptor.

Example:

```text
fd3

Hello\n42\n

fd4

World\nMadrid\n
```

Reading alternately:

```text
fd3

↓

Hello\n

fd4

↓

World\n

fd3

↓

42\n

fd4

↓

Madrid\n
```

Each descriptor preserves its own reading state without interfering with the others.

---

### Memory Safety

Special care has been taken to ensure that edge cases never compromise memory ownership.

Whenever execution cannot continue safely:

* Owned allocations are released.
* Internal pointers are reset when necessary.
* The function returns `NULL`.

At no point does the implementation intentionally leak memory or leave an inconsistent internal state.

---

### Edge Case Summary

| Scenario                  | Mandatory | Bonus |
| ------------------------- | :-------: | :---: |
| Empty file                |     ✅     |   ✅   |
| Only newline characters   |     ✅     |   ✅   |
| Missing final newline     |     ✅     |   ✅   |
| `BUFFER_SIZE = 1`         |     ✅     |   ✅   |
| Large `BUFFER_SIZE`       |     ✅     |   ✅   |
| Invalid file descriptor   |     ✅     |   ✅   |
| Read error                |     ✅     |   ✅   |
| Allocation failure        |     ✅     |   ✅   |
| Reading from `stdin`      |     ✅     |   ✅   |
| Multiple file descriptors |     ❌     |   ✅   |

---

## Key Takeaways

* The implementation behaves consistently across all tested edge cases.
* Memory ownership rules are preserved even during failures.
* Every allocation is validated before use.
* End-of-file and error conditions are handled safely.
* The Bonus implementation extends the same guarantees to multiple simultaneous file descriptors.


## Testing

A correct implementation of `get_next_line()` must behave consistently under a wide variety of input conditions.

Throughout the development of this project, the implementation was continuously validated using custom test programs, official project requirements and external testing tools.

The objective was not only to verify correctness, but also to ensure proper memory management and robust behavior under edge cases.

---

### Mandatory Tests

The mandatory implementation was validated using a custom test program capable of reading complete files line by line.

Typical scenarios included:

* Empty files
* Single-line files
* Multi-line files
* Files ending with `'\n'`
* Files without a trailing newline
* Files containing only newline characters
* Very large lines
* Different `BUFFER_SIZE` values

Example:

```bash
./Get_Next_Line
```

---

### Bonus Tests

The Bonus implementation was validated using multiple file descriptors simultaneously.

Example execution:

```text
fd3 → file_A.txt
fd4 → file_B.txt

↓

get_next_line(fd3)

↓

get_next_line(fd4)

↓

get_next_line(fd3)

↓

get_next_line(fd4)
```

Each descriptor preserves its own reading state independently.

---

### BUFFER_SIZE Validation

The implementation was tested using several compile-time values.

```bash
-D BUFFER_SIZE=1
```

```bash
-D BUFFER_SIZE=42
```

```bash
-D BUFFER_SIZE=1024
```

```bash
-D BUFFER_SIZE=100000
```

The reading algorithm remains identical regardless of the selected buffer size.

---

### Memory Validation

Memory correctness was verified using **Valgrind**.

Example:

```bash
valgrind \
--leak-check=full \
--show-leak-kinds=all \
--track-origins=yes \
./Get_Next_Line
```

Validation focused on:

* Memory leaks
* Invalid reads
* Invalid writes
* Double frees
* Lost allocations

No memory allocated by the implementation remains reachable after execution.

---

### External Test Suites

The implementation was also designed to be compatible with the testing tools commonly used within the 42 community.

These include:

* Francinette
* Tripouille's get_next_line Tester

Both provide extensive validation against the project specification and help identify corner cases that may not appear during manual testing.

---

### Manual Validation

Additional manual tests were performed during development to verify scenarios that frequently cause incorrect implementations.

Examples include:

* Files containing only consecutive newline characters.
* Repeated calls after reaching EOF.
* Reading from `stdin`.
* Invalid file descriptors.
* Allocation failure paths.
* Alternating reads between multiple file descriptors (Bonus).

These tests were especially useful for validating memory ownership and cache management.

---

### Testing Philosophy

Testing was considered an integral part of the development process rather than a final verification step.

Each helper function was designed with a single responsibility, making it easier to reason about its behavior and isolate problems during debugging.

Special attention was given to:

* Correct memory ownership.
* Predictable error handling.
* Consistent behavior across edge cases.
* Architectural simplicity.

---

### Test Coverage Summary

| Category                       | Mandatory | Bonus |
| ------------------------------ | :-------: | :---: |
| Empty file                     |     ✅     |   ✅   |
| Multiple lines                 |     ✅     |   ✅   |
| Missing final newline          |     ✅     |   ✅   |
| Only newline characters        |     ✅     |   ✅   |
| Different `BUFFER_SIZE` values |     ✅     |   ✅   |
| Invalid file descriptor        |     ✅     |   ✅   |
| Read errors                    |     ✅     |   ✅   |
| Memory leaks                   |     ✅     |   ✅   |
| Standard input (`stdin`)       |     ✅     |   ✅   |
| Multiple file descriptors      |     ❌     |   ✅   |

---

## Key Takeaways

* The implementation has been validated through both manual and automated testing.
* Memory safety was verified using Valgrind.
* The algorithm behaves correctly across a wide range of edge cases.
* Both Mandatory and Bonus implementations follow the same testing philosophy.
* Testing was integrated into the development process from the beginning, helping ensure both correctness and robustness.


## Resources

The following references were used throughout the design, implementation and validation of this project.

Although the final solution was developed independently, these resources were valuable for understanding POSIX file handling, dynamic memory management and best practices in C programming.

---

### Official Documentation

* **42 Project Subject**

  * Official project specification provided by 42.

* **The Open Group Base Specifications**

  * POSIX `read()` documentation.
  * File descriptor behavior.
  * Standard I/O concepts.

* **Linux Manual Pages**

```bash
man 2 read
```

```bash
man 2 open
```

```bash
man 2 close
```

---

### Recommended Reading

* *The C Programming Language*

  * Brian W. Kernighan & Dennis M. Ritchie

* *Advanced Programming in the UNIX Environment*

  * W. Richard Stevens

* *Computer Systems: A Programmer's Perspective*

  * Randal E. Bryant & David R. O'Hallaron

These books provide excellent explanations of:

* Dynamic memory management
* UNIX file descriptors
* Buffered input/output
* System calls
* Process memory layout

---

### Development Tools

The project was developed and validated using:

* GCC / Clang
* GNU Make
* GDB
* LLDB
* Valgrind
* Norminette
* Visual Studio Code

---

### Testing Tools

The implementation was validated using:

* Custom Mandatory Tester
* Custom Bonus Tester
* Francinette
* Tripouille's get_next_line Tester
* Valgrind

---

### Concepts Explored

This project provides practical experience with several important systems programming concepts.

* POSIX File Descriptors
* Buffered Reading
* Dynamic Memory Management
* Static Storage Duration
* Heap vs Stack
* Memory Ownership
* Incremental Algorithms
* Modular Software Design
* Defensive Programming

---

### Further Exploration

Readers interested in expanding this implementation may explore topics such as:

* Buffered stream abstractions
* Circular buffers
* Ring buffers
* Custom allocators
* Event-driven I/O
* Asynchronous file handling
* Memory pools

Many of these concepts are explored in more advanced systems programming and operating systems courses.

---

### Key Takeaways

* Understanding the underlying operating system APIs is essential when working with low-level I/O.
* Good software architecture is as important as algorithm correctness.
* Careful memory management remains one of the most important aspects of C programming.
* Projects like `get_next_line` provide an excellent foundation for more advanced topics such as network programming, shells and operating systems.


## Artificial Intelligence

The development of this project was supported by modern Artificial Intelligence tools as complementary engineering assistants.

Rather than generating the final implementation, AI was used to facilitate technical discussions, validate architectural decisions and explore alternative approaches throughout the development process.

Its primary contributions included:

* Reviewing software architecture before implementation.
* Discussing memory ownership strategies.
* Evaluating different helper function designs.
* Identifying potential edge cases.
* Reviewing algorithm correctness.
* Improving code readability and maintainability.
* Assisting in the preparation of the technical documentation.

Every design decision and every line of code was carefully reviewed, understood and, when necessary, adapted before being incorporated into the final implementation.

Particular emphasis was placed on understanding the underlying concepts rather than simply obtaining a working solution.

The project was developed following the philosophy that Artificial Intelligence should enhance the learning process by acting as an experienced reviewer and technical discussion partner, while the final responsibility for the design, implementation and validation always remains with the developer.

This collaborative approach not only accelerated development but also encouraged deeper reflection on software architecture, modular design and memory management, ultimately resulting in a more robust and maintainable implementation.


## Lessons Learned

Although **get_next_line** is presented as a relatively small programming exercise, developing a robust implementation revealed that the real challenge lies not in reading lines from a file descriptor, but in designing a solution that remains simple, reliable and maintainable under every possible scenario.

Looking back at the development process, several important lessons emerged.

---

### Design Before Implementation

Perhaps the most valuable lesson learned during this project was the importance of investing time in the architecture before writing code.

Instead of immediately implementing the reading algorithm, the project began by defining:

* The responsibilities of every helper function.
* The ownership of every dynamically allocated block.
* The interaction between modules.
* The complete execution pipeline.

This initial design phase significantly simplified the implementation and reduced the number of architectural changes later in the project.

---

### Good Architecture Simplifies Future Features

One of the original design goals was ensuring that the Mandatory implementation could naturally evolve into the Bonus implementation.

Because the reading algorithm was designed independently from the storage mechanism, supporting multiple file descriptors required only a minimal modification to the internal state representation.

Rather than rewriting the algorithm, the existing architecture was simply extended.

This demonstrated how a well-designed abstraction can dramatically reduce the cost of future features.

---

### Memory Ownership Matters

Managing dynamic memory safely is not simply a matter of calling `malloc()` and `free()`.

The project highlighted the importance of defining ownership explicitly.

Every allocation should answer three simple questions:

* Who owns it?
* Who is responsible for releasing it?
* When should it cease to exist?

Answering these questions before implementation greatly reduced the risk of leaks, double frees and dangling pointers.

---

### Simplicity Is Often the Best Optimization

Several implementation alternatives were considered throughout the project.

In many cases, the simplest solution also proved to be the easiest to understand, debug and maintain.

Examples include:

* Keeping helper functions focused on a single task.
* Centralizing memory ownership.
* Reading only when necessary.
* Avoiding unnecessary abstractions.

This reinforced the idea that clear code is often more valuable than clever code.

---

### Edge Cases Should Be Part of the Design

Many implementation problems originate from treating edge cases as an afterthought.

Throughout this project, scenarios such as:

* Empty files
* Consecutive newline characters
* Missing trailing newlines
* Allocation failures
* Invalid file descriptors

were considered during the design phase rather than after the implementation was completed.

As a result, the final solution remained consistent across both normal and exceptional execution paths.

---

### Constraints Can Improve Design

The restrictions imposed by the 42 Norm initially appear limiting.

However, designing around these constraints encouraged:

* Better modularization.
* Smaller functions.
* Clearer responsibilities.
* Improved readability.

Rather than becoming an obstacle, the constraints ultimately contributed to a cleaner architecture.

---

### Documentation Is Part of the Project

Writing this README reinforced another important lesson:

A good implementation deserves good documentation.

Explaining the architecture, memory model and design decisions required understanding them at a much deeper level than simply writing working code.

Documenting the project became an extension of the learning process itself.

---

### Continuous Improvement

Many parts of the implementation evolved through several iterations.

Functions were simplified.

Memory ownership became clearer.

Helper responsibilities were refined.

The overall architecture gradually improved as new edge cases and design opportunities were discovered.

This iterative process demonstrated that software development is rarely about finding the perfect solution immediately, but rather about continuously improving an existing design through analysis, testing and refinement.

---

### Final Thoughts

Beyond its functional objective, **get_next_line** became an excellent exercise in software engineering.

It demonstrated that writing correct code is only one part of building quality software.

Thoughtful architecture, explicit memory ownership, careful testing and clear documentation are equally important components of a maintainable implementation.

The principles learned during this project will naturally carry over into more complex projects such as **push_swap**, **pipex**, **minishell** and future systems programming challenges.

---

## Key Takeaways

* Design before implementation.
* Build around clear abstractions.
* Make memory ownership explicit.
* Treat edge cases as first-class requirements.
* Prefer clarity over unnecessary complexity.
* Consider documentation an integral part of software development.
* View every project as an opportunity to refine both code and engineering practices.


## Author

### Emmanuel Millán Contreras

**Software Engineer | Data Engineer | 42 Student**

This project was developed as part of the **42 School** curriculum with the objective of deepening my understanding of low-level systems programming, dynamic memory management and software architecture.

Throughout its development, the focus extended beyond implementing a correct solution. Particular emphasis was placed on designing a clean and maintainable architecture, establishing explicit memory ownership rules and producing technical documentation that explains not only *how* the implementation works, but also *why* it was designed that way.

The engineering principles that guided this project include:

* Clean and modular architecture.
* Single Responsibility Principle (SRP).
* Explicit memory ownership.
* Defensive programming.
* Readability over unnecessary complexity.
* Continuous improvement through iterative refinement.
* Documentation as an integral part of software development.

I believe that software engineering is not only about writing code that works, but about building solutions that remain understandable, maintainable and extensible over time.

Every project completed throughout the 42 curriculum represents an opportunity to strengthen both technical knowledge and engineering practices. This repository documents not only the final implementation, but also the design decisions, architectural reasoning and lessons learned during its development.

Thank you for taking the time to explore this project.

I hope both the implementation and its accompanying documentation prove useful to other students, developers and anyone interested in learning more about systems programming in C.

---

*"First solve the problem. Then write the code."*
— **John Johnson**

---

**Designed and developed by**

**Emmanuel Millán Contreras**

© 2026 All Rights Reserved.
