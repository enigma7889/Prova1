# Prova1

Prova 1 laboratorio secondo anno

# Compilation of Prova1 source

Compile *only the source*:
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
./particle.t

Note: in the last line "" may be changed with test programs and so on...

```
Compile and execute on root:

```
root -l -b -q Main.cpp+g
root .L read.cpp+
root read();

```


