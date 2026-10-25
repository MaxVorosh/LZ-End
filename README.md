# LZ-End Triplets Tool

Reads triplets, computes LZ-End parsing, and stores/loads triplets.

## Build

```sh
make -f Makefile
```

Binary: `build/triplets`.

## Run

Write triplets from stdin:

```sh
printf "2\n1 2 a\n3 4 b\n" | ./build/triplets write out.bin
```

Load triplets from file:

```sh
./build/triplets load out.bin
```

Compute triplets from text file:

```sh
./build/triplets compute input.txt
```

Clean:

```sh
make -f Makefile clean
```