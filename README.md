# memory-test-allocator
Simple memory test allocator for check of memory profiler tools

# Motivation
Let's image, you create you own memory profiler or you would like to check quality of new memory profiler from Open Source.

# How it works?

This code delivery automatic program memory allocations and deallocation with programmed scheme.

## Arguments description

Arg. list have dedicated format like: `"<delay>:<unit>:+N-N"`, where:
- `<delay>` - it's how many seconds program need to sleep before allocations/deallocation
- `<unit>` - memory unit sizes "kB" or "MB
- `+N-N` - sequence of allocation with `+` before size or deallocation `-` before size

## Example run with logs

```bash
$ make
$ ./memory_test_allocator "3:MB:+1+10-10-1"
Starting memory test with delay = 3 sec, unit = MB
Allocated 1 MB (1048576 bytes)
Allocated 10 MB (10485760 bytes)
Freed 10 MB (10485760 bytes)
Freed 1 MB (1048576 bytes)
Test complete.
```

## TODO list
- [ ] CI in Github Action
- [ ] add unit tests
- [ ] switch from Makefile to CMake
- [ ] validation for bilans of allocations/deallocation on program start (memory leak detection - only warning on STDOUT, becouse it's one of possible test scenario!)
