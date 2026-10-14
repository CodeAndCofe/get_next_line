# get_next_line

## Introduction

This project was developed in 2024 at 1337 Coding School, part of the 42 Network. It was an important step in my C programming journey because it pushed me to understand memory management more deeply.

I decided to share this project on GitHub to document what I learned and help other students understand the ideas behind reading files line by line in C.

## Project Overview

The goal of `get_next_line` is to write a function that reads from a file descriptor and returns one line per function call. Repeated calls continue reading from where the previous call stopped, until there is no more input or an error occurs.

A line normally includes its terminating newline character (`\n`) when one is present. The final line can be returned without a newline if the file ends that way. When there is no line left to return, the function returns `NULL`.

The function is commonly declared as:

```c
char *get_next_line(int fd);
```

The project also introduced a configurable `BUFFER_SIZE`, which determines how many bytes are requested in each `read()` call. A line can be longer than the buffer, so the function must keep reading and joining data until it has a complete line or reaches the end of the file.

## Example

```c
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "get_next_line.h"

int main(void)
{
    int fd = open("example.txt", O_RDONLY);
    char *line;

    if (fd < 0)
        return (1);
    while ((line = get_next_line(fd)) != NULL)
    {
        printf("%s", line);
        free(line);
    }
    close(fd);
    return (0);
}
```

Each call returns a newly allocated line in common implementations of this project, so the caller must free the returned string after using it. The exact permitted functions and implementation details depend on the version of the subject used by your campus.

## What I Learned

### 1. Memory Management

This project taught me more about dynamic memory management. A line can be larger than `BUFFER_SIZE`, so I had to think carefully about how to allocate enough space, join the data read so far with newly read bytes, extract a line, preserve the leftover characters, and release memory that was no longer needed.

I learned to:

- Check whether an allocation succeeded before using the returned pointer.
- Keep track of which function owns each allocated block of memory.
- Free temporary buffers and intermediate strings when they are no longer needed.
- Avoid memory leaks, invalid reads and writes, use-after-free errors, and double-free errors.
- Handle edge cases such as empty files, long lines, and a final line without a newline.

### 2. Static Variables

One of the key concepts in `get_next_line` is the use of a `static` variable to preserve data between function calls.

A normal local variable is created for a function call and its lifetime ends when that call returns. A local static variable retains its value between calls. This is useful because `read()` may fetch more data than is needed for the current line. The unused characters must be kept so the next call can continue from the correct position.

A simplified example is:

```c
char *get_next_line(int fd)
{
    static char *stash;

    /* Read more data, return one line, and preserve the leftover text. */
}
```

Here, `stash` remembers the leftover text between calls. The `static` keyword controls the lifetime of the pointer variable; it does **not** automatically allocate the character buffer on the heap. In a typical implementation, `stash` may point to a buffer created with `malloc()`.

### 3. Understanding the Stack and the Heap

I also learned how the stack and heap differ and why the distinction matters when writing C programs.

<img width="1200" height="650" alt="stack-vs-heap" src="https://github.com/user-attachments/assets/629fcf79-574a-4102-8854-8dc2fefd94e4" /><svg xmlns="http://www.w3.org/2000/svg" width="1200" height="650" viewBox="0 0 1200 650" role="img" aria-labelledby="title desc">
  <title id="title">Stack, static storage, and heap in get_next_line</title>
  <desc id="desc">Diagram showing local variables on the stack, a static stash pointer in static storage, and dynamically allocated character buffers on the heap.</desc>
  <defs>
    <linearGradient id="bg" x1="0" y1="0" x2="1" y2="1"><stop offset="0" stop-color="#0b1220"/><stop offset="1" stop-color="#16263d"/></linearGradient>
    <marker id="arrow" markerWidth="10" markerHeight="10" refX="8" refY="3" orient="auto"><path d="M0,0 L9,3 L0,6 Z" fill="#94a3b8"/></marker>
    <marker id="greenArrow" markerWidth="10" markerHeight="10" refX="8" refY="3" orient="auto"><path d="M0,0 L9,3 L0,6 Z" fill="#34d399"/></marker>
    <style>
      .heading{font:700 27px sans-serif;fill:#f8fafc}.sub{font:16px sans-serif;fill:#cbd5e1}.label{font:700 18px sans-serif;fill:#f8fafc}.small{font:15px sans-serif;fill:#dbeafe}.code{font:16px monospace;fill:#e2e8f0}.note{font:15px sans-serif;fill:#cbd5e1}.stack{fill:#193c67;stroke:#60a5fa;stroke-width:2}.static{fill:#49346f;stroke:#c4b5fd;stroke-width:2}.heap{fill:#124e45;stroke:#34d399;stroke-width:2}.muted{stroke:#94a3b8;stroke-width:2;fill:none;marker-end:url(#arrow)}.green{stroke:#34d399;stroke-width:2.5;fill:none;marker-end:url(#greenArrow)}
    </style>
  </defs>
  <rect width="1200" height="650" rx="24" fill="url(#bg)"/>
  <text x="44" y="55" class="heading">get_next_line: where does the data live?</text>
  <text x="44" y="85" class="sub">Conceptual diagram — exact memory layout depends on the system and compiler.</text>

  <!-- Stack column -->
  <rect x="40" y="120" width="350" height="365" rx="18" fill="#101c2d" stroke="#2b4665"/>
  <text x="64" y="158" class="label">1. Stack</text>
  <text x="64" y="184" class="note">Automatic local variables / function calls</text>
  <rect x="76" y="215" width="278" height="54" rx="8" class="stack"/>
  <text x="215" y="238" text-anchor="middle" class="small">get_next_line() frame</text>
  <text x="215" y="258" text-anchor="middle" class="small">local variables</text>
  <rect x="76" y="280" width="278" height="54" rx="8" class="stack"/>
  <text x="215" y="303" text-anchor="middle" class="small">read() temporary buffer</text>
  <text x="215" y="323" text-anchor="middle" class="small">if declared as a local array</text>
  <path d="M 215 345 L 215 380" class="muted"/>
  <text x="215" y="405" text-anchor="middle" class="note">Function returns → its frame ends</text>
  <text x="64" y="451" class="note">Lifetime is usually tied to the call.</text>
  <text x="64" y="473" class="note">Do not return a pointer to a local variable.</text>

  <!-- Static storage column -->
  <rect x="425" y="120" width="350" height="365" rx="18" fill="#101c2d" stroke="#4c3a77"/>
  <text x="449" y="158" class="label">2. Static storage</text>
  <text x="449" y="184" class="note">The static pointer itself lives here</text>
  <rect x="461" y="220" width="278" height="70" rx="8" class="static"/>
  <text x="600" y="247" text-anchor="middle" class="code">static char *stash;</text>
  <text x="600" y="271" text-anchor="middle" class="small">remembers state between calls</text>
  <path d="M 600 291 L 600 334" class="green"/>
  <text x="615" y="317" class="note">points to</text>
  <rect x="480" y="342" width="240" height="65" rx="8" class="heap"/>
  <text x="600" y="367" text-anchor="middle" class="small">allocated characters</text>
  <text x="600" y="389" text-anchor="middle" class="small">on the heap</text>
  <text x="449" y="451" class="note">The pointer and pointed-to memory</text>
  <text x="449" y="473" class="note">are different things with different lifetimes.</text>

  <!-- Heap column -->
  <rect x="810" y="120" width="350" height="365" rx="18" fill="#101c2d" stroke="#1b705f"/>
  <text x="834" y="158" class="label">3. Heap</text>
  <text x="834" y="184" class="note">Dynamic allocations with malloc()</text>
  <rect x="850" y="220" width="270" height="55" rx="8" class="heap"/>
  <text x="985" y="243" text-anchor="middle" class="small">allocated stash / joined text</text>
  <text x="985" y="263" text-anchor="middle" class="small">retained between calls</text>
  <rect x="850" y="291" width="270" height="55" rx="8" class="heap"/>
  <text x="985" y="314" text-anchor="middle" class="small">returned line</text>
  <text x="985" y="334" text-anchor="middle" class="small">owned by the caller</text>
  <path d="M 985 355 L 985 389" class="muted"/>
  <text x="985" y="415" text-anchor="middle" class="note">free() when no longer needed</text>
  <text x="834" y="451" class="note">Allocated memory remains until freed</text>
  <text x="834" y="473" class="note">or the process terminates.</text>

  <!-- Bottom takeaway -->
  <rect x="40" y="510" width="1120" height="106" rx="16" fill="#0a1423" stroke="#334155"/>
  <text x="64" y="544" class="label">Key idea</text>
  <text x="64" y="572" class="small">A static pointer can remember the leftover text between calls, while that text is stored in dynamically allocated memory.</text>
  <text x="64" y="597" class="small">When the line is returned, preserve any remaining characters and free allocations that are no longer needed.</text>
</svg>



**The stack** is commonly used for function-call information and automatic local variables. Its memory is managed automatically as functions are called and return. For example:

```c
void example(void)
{
    int count = 0; /* automatic local variable */
}
```

When `example()` returns, the lifetime of `count` ends. You should not return a pointer to a local variable whose lifetime has ended.

**The heap** is used for dynamic allocations. Functions such as `malloc()` request memory that remains allocated until it is released with `free()` or the process ends. In `get_next_line`, this is useful for storing strings whose sizes can vary depending on the input.

```c
char *line = malloc(size);

if (line == NULL)
    return (NULL);
/* Use the allocated memory. */
free(line);
```

The example is illustrative; real code must ensure that `size` is correct and that the memory is freed exactly once when it is no longer needed.

**An important distinction:** the pointer `stash` in the example above has static storage duration, while the dynamically allocated text it points to may live on the heap. The pointer and the memory it points to are separate objects.

### 4. File Descriptors and `read()`

I learned how file descriptors identify an input source, such as a file or standard input, and how `read()` retrieves a limited number of bytes at a time.

Because `read()` does not necessarily return a complete line in one call, `get_next_line` must inspect the bytes it receives, detect newline characters, and continue reading when the line is incomplete. It must also handle the end of the file and errors correctly.

### 5. String Manipulation

The project gave me more practice working with C strings and character buffers. I had to locate newline characters, calculate string lengths, concatenate chunks, extract a line, and preserve any remaining characters for the next call.

These operations reinforced how important null terminators, buffer sizes, pointer ownership, and allocation sizes are when manipulating strings in C.

## Challenges

The main challenge was preserving unread data between calls without losing characters or leaving allocated memory behind. I also had to account for lines longer than the buffer, empty files, end-of-file behavior, read errors, and the correct cleanup of temporary allocations.

Working through these cases helped me become more careful about the lifetime of data and the responsibility that comes with dynamic allocation in C.

## Conclusion

`get_next_line` was a valuable project because it took the memory-management concepts I had started learning in Libft and made me apply them to a more stateful problem.

I gained a better understanding of dynamic allocation, `free()`, static variables, the stack and heap, file descriptors, and string manipulation. Most importantly, I learned that good C code is not only about getting the expected output; it is also about managing memory safely and preserving the correct state between function calls.
