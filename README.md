# 🌕 Moonlight kernel

Moonlight kernel is a toy kernel that made by [Gazach](https://github.com/Gazach/) just for fun.

this only an experiment project and feel free to see the source code.

Don't expect a production-ready operating system — this is simply a fun project for exploring how computers work under the hood.

## Build

### Target platform
x86_64

<hr>

### install dependencies
Use ubuntu (or **WSL** is working) :
```bash
sudo apt install clang lld nasm qemu-system-x86 grub-pc-bin xorriso
```

to build the project :

```bash
git clone https://github.com/Gazach/moonlight-kernel
```
```bash
cd moonlight-kernel/
```
```bash
make
```

if you want to clean and rebuild :

```bash
make clean
```

```bash
make
```

### Run the project :

```bash
qemu-system-i386 -cdrom build/moonlight.iso
```

## Project inspired

This project is inspired after reading OSdev Wiki and it's inspired me to make my own little kernel.

click [here](https://wiki.osdev.org/) if you also intrested.