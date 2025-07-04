# bigtreetech SKR-Pico

example setup & information collection to use the bigtreetech SKR-Pico Stepper-Controller Board with CircuitPython and Arduino

![simple test setup](<images/20250628_222219.jpg>)

## Pin Mapping

| GPIO        | Schematic | connector | connector name | pin   | arduino  | function                                                                                 |
| ----------- | --------- | --------- | -------------- | ----- | -------- | ---------------------------------------------------------------------------------------- |
| GPIO0       | TXD0      | P5        | 5V_POWER       | pin 0 | PIN_TXD0 | can be used to connect to RPI                                                            |
| GPIO1       | RXD0      | P5        | 5V_POWER       | pin 1 | PIN_RXD0 | can be used to connect to RPI                                                            |
| GPIO2       | ZEN       |           |                |       |          | TMC2209 `Z`                                                                              |
| GPIO3       | Y-STOP    | P4        | Y-STOP         |       |          | physical limit-switch OR driver DIAG                                                     |
| GPIO4       | X-STOP    | P1        | X-STOP         |       |          | physical limit-switch OR driver DIAG                                                     |
| GPIO5       | YDIR      |           |                |       |          | TMC2209 `Y`                                                                              |
| GPIO6       | YSTP      |           |                |       |          | TMC2209 `Y`                                                                              |
| GPIO7       | YEN       |           |                |       |          | TMC2209 `Y`                                                                              |
| GPIO8       | TX4       |           |                |       |          | all TMC2209 UART                                                                         |
| GPIO9       | RX4       |           |                |       |          | all TMC2209 UART                                                                         |
| GPIO10      | XDIR      |           |                |       |          | TMC2209 `X`                                                                              |
| GPIO11      | XSTP      |           |                |       |          | TMC2209 `X`                                                                              |
| GPIO12      | XEN       |           |                |       |          | TMC2209 `X`                                                                              |
| GPIO13      | E0DIR     |           |                |       |          | TMC2209 `E0`                                                                             |
| GPIO14      | E0STP     |           |                |       |          | TMC2209 `E0`                                                                             |
| GPIO15      | E0EN      |           |                |       |          | TMC2209 `E0`                                                                             |
| GPIO16      | E0-STOP   | P8        | E0-STOP        |       |          | physical limit-switch OR driver DIAG                                                     |
| GPIO17      | FAN1_PWM  | P15       | FAN1           |       |          | `ES 3400 MOSFET` 5V switched output (should be fine for about 1A)                        |
| GPIO18      | FAN2_PWM  | P16       | FAN2           |       |          | `ES 3400 MOSFET` 5V switched output (should be fine for about 1A)                        |
| GPIO19      | ZSTP      |           |                |       |          | TMC2209 `Z`                                                                              |
| GPIO20      | FAN3_PWM  | P17       | FAN3           |       |          | used for both outputs: `ES 3400 MOSFET` 5V switched output (should be fine for about 1A) |
| GPIO20      | LASER_PWM | P12       | Laser          | pin 1 | PIN_SDA  | direct - NO PROTECTION - only inline 100R Resistor                                       |
| GPIO21      | HB_PWM    |           |                |       |          | heat bed                                                                                 |
| GPIO22      | P_2       | P21       | jumper         | pin 2 |          | jumper to select for `P13 PROBE` OR proximity Switch `P14 WD-DET`                        |
| GPIO22      | PROBE     | P13       | PROBE          | pin 2 |          | direct - NO PROTECTION                                                                   |
| GPIO22      | P_S       | P14       | WD-DET         | pin 1 |          | via opto-coupler → attention: jumper JP2 has VCC (12/24V) level!                         |
| GPIO23      | HE_PWM    |           |                |       |          | heat extruder                                                                            |
| GPIO24      | RGB       | P24       | RGB            | pin 2 |          | also `LED7` internal RGB-LED    (signal level-shifted to 5V)                             |
| GPIO25      | Z-STOP    | P6        | Z-STOP         |       |          | physical limit-switch OR driver DIAG                                                     |
| GPIO26_ADC0 | THB       | P3        | THB            |       |          | Thermistor Bed                                                                           |
| GPIO27_ADC1 | TH0       | P7        | TH0            |       |          | Thermistor Extruder                                                                      |
| GPIO28_ADC2 | ZDIR      |           |                |       |          | TMC2209 `Z`                                                                              |
| GPIO29_ADC3 | SERVOS    | P11       | SERVOS         | pin 3 | PIN_SCL  | goes directly to the SERVOS pin. NO PROTECTION!                                          |
| GPIO29_ADC3 | ADC3      |           |                |       |          | **unpopulated**:  measure the 5V rail ??                                                 |
|             |           |           |                |       |          |                                                                                          |
|             | RUN       | P9.1      | SWD            |       |          |                                                                                          |
| SWCLK       | SWCLK     | P9.2      | SWD            |       |          |                                                                                          |
|             | GND       | P9.3      | SWD            |       |          |                                                                                          |
| SWDIO       | SWDIO     | P9.4      | SWD            |       |          |                                                                                          |
|             | 3.3V      | P9.5      | SWD            |       |          |                                                                                          |
|             |           |           |                |       |          |                                                                                          |

### TMC2209 stepper driver

| signal             | X      | Y     | Z      | E0     |
| ------------------ | ------ | ----- | ------ | ------ |
| UART TX            | GPIO8  | GPIO8 | GPIO8  | GPIO8  |
| UART RX            | GPIO9  | GPIO9 | GPIO9  | GPIO9  |
| DIR                | GPIO10 | GPIO5 | GPIO28 | GPIO13 |
| STP                | GPIO11 | GPIO6 | GPIO19 | GPIO14 |
| EN                 | GPIO12 | GPIO7 | GPIO2  | GPIO15 |
| DIAG               | GPIO04 | GPIO3 | GPIO25 | GPIO16 |
| MS1_AD0 bit0 (LSB) | GND    | GND   | 3.3V   | 3.3V   |
| MS2_AD1 bit1       | GND    | 3.3V  | GND    | 3.3V   |
| UART address       | 0      | 1     | 2      | 3      |
|                    |        |       |        |        |

TODO: verify UART address


### how to connect I2C sensor  
you can get the 3.3V Power from the SWD pin-header (`P9 SWD`).  
PIN 3 is GND; PIN 5 is 3.3V  
you could map 

    - GPIO0 for I2C0_SDA
    - GPIO1 for I2C0_SCL

these are available on `P5 5V_POWER` (the connector designed for connecting a RPI)  
https://electronics.stackexchange.com/a/687771/13800

### other IO options
raw uC signals available:
(available functions [from official datasheet](https://datasheets.raspberrypi.com/rp2040/rp2040-datasheet.pdf)
| GPIO        | pin-names          | F1 SPI   | F2 UART   | F3 I2C   |
| ----------- | ------------------ | -------- | --------- | -------- |
| GPIO0       | I2C0_SDA, PIN_TXD0 | SPI0 RX  | UART0 TX  | I2C0 SDA |
| GPIO1       | I2C0_SCL, PIN_RXD0 | SPI0 CSn | UART0 RX  | I2C0 SCL |
| GPIO20      | LASER_PWM          | SPI0 RX  | UART1 TX  | I2C0 SDA |
| GPIO22      | P_2                | SPI0 SCK | UART1 CTS | I2C1 SDA |
| GPIO29_ADC3 | SERVOS             | SPI1 CSn | UART0 RX  | I2C0 SCL |
|             |                    |          |           |          |

from this table the only meaningful usable combination could be:

- GPIO0 PIN_TXD0
- GPIO1 PIN_RXD0
- GPIO20 PIN_SDA
- GPIO29_ADC3 PIN_SCL

we sadly have no SPI Pins available.
(without hacking / soldering somewhere to the board..)

> [!CAUTION]
> the I2C on GPIO20 and GPIO29_ADC3 are untested!!
> currently PIN_SDA and PIN_SCL map to GPIO0 and GPIO1.

TODO: test I2C on GPIO20 and GPIO29_ADC3!

## Arduino support
[supported by `arduino-pico`](https://github.com/earlephilhower/arduino-pico) as of [release 4.6.0](https://github.com/earlephilhower/arduino-pico/releases/tag/4.6.0)
🥳

for available pin names have a look at the [`pins_arduino.h`](https://github.com/earlephilhower/arduino-pico/blob/master/variants/bigtreetech_SKR_Pico/pins_arduino.h) file.

## CircuitPython
TODO!

## Helpfull links

- https://www.analog.com/media/en/technical-documentation/data-sheets/TMC2209_datasheet_rev1.08.pdf
- https://bigtreetech-beta.github.io/docs/SKR%20Pico.html
- https://github.com/bigtreetech/SKR-Pico/
    - https://github.com/bigtreetech/SKR-Pico/blob/master/Hardware/BTT%20SKR%20Pico%20V1.0-SCH.pdf
- https://www.reddit.com/r/BIGTREETECH/comments/1aqu1ub/skr_pico_for_nonprinter_projects/
- 
where to buy?
https://www.tinytronics.nl/en/development-boards/3d-printing/bigtreetech-skr-pico-v1.0-3d-printer-motherboard?sort=p.price&order=ASC


