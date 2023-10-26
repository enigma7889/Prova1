# Prova1

Prova 1 laboratorio secondo anno

# Compilation

Compile *only the library*:
```
cmake -S . -B build -DBUILD_TESTING=OFF
cmake --build build
```

Compile with testcases:

```
cmake -S . -B build -DBUILD_TESTING=ON
cmake --build build

```
Code execution:

```
cd build
./main

Note: in the last line "main" may be changed with test programs and so on...

```
