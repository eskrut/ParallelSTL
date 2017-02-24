# ParallelSTL

This is a testing project to play with N4354 Parallelism TS implementations.

Currently two implementation tested:

1. https://github.com/t-lutz/ParallelSTL
2. https://github.com/STEllAR-GROUP/hpx

Also there are tests of gcc parallel versions of standard algorythms:
http://gcc.gnu.org/onlinedocs/libstdc++/manual/parallel_mode.html

### Build
```cmake
cmake
make
ctest
```
