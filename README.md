# Chip-8 Emulator
This is a simple Chip-8 Emulator made in C++.
## Usage
```
.\Chip8Emulator.exe [-h | -f <file-path> | -c <cycle-delay> | -w <window-scale>]

Allowed options:
  -h [ --help ]             Print this help message
  -f [ --file-path ] arg    Path to input file
  -c [ --cycle-delay ] arg  Set the cycle delay, default = 8(ms)
  -w [ --window-scale ] arg Set the window scale, default = 10
```
## Controls
### Keypad Controls
![alt text](https://github.com/Natan822/chip8-emulator/blob/2558f9950dc3bdde3e4b0aca0a39ef2190108c0e/images/keypad.jpg)

### Additional Controls
LShift - Decrease Cycle Delay by 1(increase emulation speed)  
LCtrl - Increase Cycle Delay by 1(decrease emulation speed)

## Build Requirements
- SDL2 v2.30.7: Used for rendering.
- Boost.ProgramOptions v1.86.0: Used for execution arguments handling.
> Note: Building should also work with different SDL2 and Boost versions, however it's not been tested.
