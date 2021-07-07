# cproc
A small C preprocessor, written in C.
```c

#define A 4                 // New code
int main() {                int main() {
    int x = A;      ->          int x = 4;
}                           }
```

## build
```
./scripts/build.sh
```

## run
```
./build/cproc
```
