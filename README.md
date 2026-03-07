# PS - Motor Controller Interface

### Board Specification
- Handles power conversion and fan control for the Prohelion WaveSculptor 22.

### Confluence
- https://cloud.wikis.utexas.edu/wiki/spaces/LHRSOLAR/pages/484769832/Motor+Controller+Interface+Board

### File Structure
- Core
- Drivers
- Tasks
- Tests
- Embedded-Sharepoint
- Makefile

### Building / Testing
- Run nix develop in Embedded-Sharepoint folder, then cd into Firmware
- Run ``` make ``` to build main files.
- Run ``` make TEST=test_name ```, where ```test_name``` is the name of the test without the .c suffix.

### Flashing
- Make sure switch is set to EXT, then flash via UART (``` make flash-uart ```). 
- Set the switch to USR then press RESET button to execute code.