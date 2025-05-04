# Pico W SNES serial over controller port

The goal is to use a Pico W board as a bridge between SNES controller port and a network resource.

This early version simply acts as a SNES controller that presses the DOWN button once every 60 polls.

## Pins

Given the pinout:
```
  -----------------        1: VCC       4: Data
 | 1 2 3 4 | 5 6 7 )       2: Clock     7: Ground
  -----------------        3: Latch
```

The Pico board is expected to be wired as follows:

```
 SNES         Pico
 ------------------
 1 VCC        -
 2 Clock      GP14
 3 Latch      GP13
 4 Data       GP15
 7 Ground     GND
```


## Build instructions

```
mkdir build
cd build
cmake -DPICO_SDK_PATH=/path/to/pico-sdk-2.1.1 -DPICO_BOARD=pico_w ..
make
```
