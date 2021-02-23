# mcformat

a `cat`-like utility for parsing Minecraft formatting codes into ANSI escape codes for viewing on a terminal.

## features

- `cat`-like - supports reading from stdin or files as arguments
- supports all 16 color codes
- supports all 6 format codes
- supports Minecraft 1.16 RGB color codes (§x)
- supports 24-bit color terminals and converts RGB codes to 256-color otherwise
- written in C for speed

## building

requires CMake.

```
mkdir build
cd build
cmake ..
make
```

if you are on a rush and cannot use CMake, you may compile this using any compiler, e.g.:

```
gcc -o mcformat main.c
```

## usage

```
./mcformat [FILES ...]
```

FILES is optional.

if FILES is not provided, reads from standard input; otherwise reads from the specified files.

## FAQ

> supported encodings?

UTF-8 only.

> does this support ampersand format codes too?

no, it does not. it only supports Minecraft standard section sign (§) formatting codes.

> does this support raw JSON text?

no, and it will not.
