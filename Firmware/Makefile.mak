# ----------------------
# Toolchain
# ----------------------
CC      = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
SIZE    = arm-none-eabi-size

# ----------------------
# MCU & flags
# ----------------------
MCU       = cortex-m4
FPU       = fpv4-sp-d16
FLOAT_ABI = hard

CFLAGS = -mcpu=$(MCU) -mthumb -mfpu=$(FPU) -mfloat-abi=$(FLOAT_ABI) \
         -Wall -O0 -g -ffunction-sections -fdata-sections \
         -IApps/Inc \
         -IDrivers/STM32L4xx_HAL_Driver/Inc \
         -IDrivers/STM32L4xx_HAL_Driver/Inc/Legacy

LDFLAGS = -mcpu=$(MCU) -mthumb -mfpu=$(FPU) -mfloat-abi=$(FLOAT_ABI) \
          -TSTM32L431xx.ld -Wl,--gc-sections

# ----------------------
# Source and Object files
# ----------------------
# Automatically include all .c files in Apps/Src and Drivers Src
SRC = $(wildcard Apps/Src/*.c) \
      $(wildcard Drivers/STM32L4xx_HAL_Driver/Src/*.c)

OBJ = $(SRC:.c=.o)

# ----------------------
# Output
# ----------------------
TARGET = PS-MotorControllerPCB-Firmware.elf

# ----------------------
# Rules
# ----------------------
all: $(TARGET)

# Link object files
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)
	$(SIZE) $@

# Compile .c to .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Optional: create HEX and BIN if you decide later
hex: $(TARGET)
	$(OBJCOPY) -O ihex $(TARGET) $(TARGET:.elf=.hex)

bin: $(TARGET)
	$(OBJCOPY) -O binary $(TARGET) $(TARGET:.elf=.bin)

# Clean build files
clean:
	rm -f $(OBJ) $(TARGET) $(TARGET:.elf=.hex) $(TARGET:.elf=.bin)
