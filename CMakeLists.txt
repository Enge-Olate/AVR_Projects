cmake_minimum_required(VERSION 3.20)

project(ArduinoUno C)

# Configuração da placa
set(MCU atmega328p)
set(F_CPU 16000000UL)
set(BAUD 115200)
set(PROGRAMER arduino)
set(PORT /dev/ttyACM0) # Ajuste conforme seu sistema.
set(SERIAL_PORT /dev/ttyACM0)

include_directories(include)

# Englobando todos os arquivos .c.
file(GLOB SOURCES
    "src/*.c"
)

set(PROJECT_SOURCES
	main.c
	${SOURCES}
)
# Executável ELF
add_executable(${PROJECT_NAME}.elf
    ${PROJECT_SOURCES}
)
# Compilação
target_compile_options(${PROJECT_NAME}.elf PRIVATE
    -mmcu=${MCU}
    -DF_CPU=${F_CPU}
    -DBAUD=${BAUD}
    -Os
    -Wall
    -Wextra
    -std=c11
)

# Linkagem
target_link_options(${PROJECT_NAME}.elf PRIVATE
    -mmcu=${MCU}
)

# Localiza ferramentas AVR
find_program(AVR_OBJCOPY avr-objcopy REQUIRED)
find_program(AVR_SIZE avr-size REQUIRED)
find_program(AVRDUDE avrdude REQUIRED)

# Gera HEX
add_custom_command(
    TARGET ${PROJECT_NAME}.elf
    POST_BUILD

    COMMAND ${AVR_OBJCOPY}
    -O ihex
    -R .eeprom
    $<TARGET_FILE:${PROJECT_NAME}.elf>
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.hex

    COMMENT "Gerando arquivo HEX"

)

# Mostra tamanho do programa
add_custom_command(
    TARGET ${PROJECT_NAME}.elf
    POST_BUILD

    COMMAND ${AVR_SIZE}
    --format=avr
    --mcu=${MCU}
    ${PROJECT_NAME}.elf
)

add_custom_target(flash
    COMMAND ${AVRDUDE}
        -patmega328p
        -carduino
        -P${SERIAL_PORT}
        -b115200
        -D
        -Uflash:w:${PROJECT_NAME}.hex:i
    DEPENDS ${PROJECT_NAME}.elf
    COMMENT "Gravando Arduino UNO"
)