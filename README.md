# PS - Motor Controller Interface

## Board Specification
- Handles power conversion and fan control for the Prohelion WaveSculptor 22.

## Confluence
- https://cloud.wikis.utexas.edu/wiki/spaces/LHRSOLAR/pages/484769832/Motor+Controller+Interface+Board

## File Structure
- Core
- Drivers
- Tasks
- Tests
- Embedded-Sharepoint
- Makefile

## Building / Testing
- nix develop ./Embedded-Sharepoint (from firmware directory)
- Run ``` make ``` to build main files.
- Run ``` make TEST=test_name ```, where ```test_name``` is the name of the test without the .c suffix.

## Flashing (UART)
- Make sure switch is set to EXT, then flash via UART (``` make flash-uart ```). 
- Set the switch to USR then press RESET button to execute code.

- Note: flashing to this board (not psom issue) is really difficult. have fun spamming reset
## Flashing (ST-Link)
- To flash an STM32 microcontroller using an ST-Link, wire the device to the ST-Link (SWDIO, SWCLK, GND, 3.3V) and use software like STM32CubeProgrammer or STM32 ST-LINK Utility to erase, load, and verify the .bin or .hex firmware file. 
