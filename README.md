# bigtreetech SKR-Pico

example setup to use the bigtreetech SKR-Pico Stepper-Controller Board with CircuitPython and Arduino

## Pin Mapping

| GPIO        | Schematic          | connector | connector name | function                                                                                                             |
| ----------- | ------------------ | --------- | -------------- | -------------------------------------------------------------------------------------------------------------------- |
| GPIO0       | TXD0               | P5        | 5V_POWER       | can be used to connect to RPI                                                                                        |
| GPIO1       | RXD0               | P5        | 5V_POWER       | can be used to connect to RPI                                                                                        |
| GPIO2       | ZEN                |           |                | TMC2209 `Z`                                                                                                          |
| GPIO3       | Y-STOP             | P4        | Y-STOP         | physical limit-switch OR driver DIAG                                                                                 |
| GPIO4       | X-STOP             | P1        | X-STOP         | physical limit-switch OR driver DIAG                                                                                 |
| GPIO5       | YDIR               |           |                | TMC2209 `Y`                                                                                                          |
| GPIO6       | YSTP               |           |                | TMC2209 `Y`                                                                                                          |
| GPIO7       | YEN                |           |                | TMC2209 `Y`                                                                                                          |
| GPIO8       | TX4                |           |                | all TMC2209 UART                                                                                                     |
| GPIO9       | RX4                |           |                | all TMC2209 UART                                                                                                     |
| GPIO10      | XDIR               |           |                | TMC2209 `X`                                                                                                          |
| GPIO11      | XSTP               |           |                | TMC2209 `X`                                                                                                          |
| GPIO12      | XEN                |           |                | TMC2209 `X`                                                                                                          |
| GPIO13      | E0DIR              |           |                | TMC2209 `E0`                                                                                                         |
| GPIO14      | E0STP              |           |                | TMC2209 `E0`                                                                                                         |
| GPIO15      | E0EN               |           |                | TMC2209 `E0`                                                                                                         |
| GPIO16      | E0-STOP            | P8        | E0-STOP        | physical limit-switch OR driver DIAG                                                                                 |
| GPIO17      | FAN1_PWM           | P15       | FAN1           | `ES 3400 MOSFET` 5V switched output (should be fine for about 1A)                                                    |
| GPIO18      | FAN2_PWM           | P16       | FAN2           | `ES 3400 MOSFET` 5V switched output (should be fine for about 1A)                                                    |
| GPIO19      | ZSTP               |           |                | TMC2209 `Z`                                                                                                          |
| GPIO20      | FAN3_PWM LASER_PWM | P17 P12   | FAN3 Laser     | used for both outputs: `ES 3400 MOSFET` 5V switched output (should be fine for about 1A) AND LASER (direkt)          |
| GPIO21      | HB_PWM             |           |                | heat bed                                                                                                             |
| GPIO22      | P_2                | P21       | jumper         | select for `P13 PROBE` OR proximity Switch `P14 WD-DET`                                                              |
| GPIO23      | HE_PWM             |           |                | heat extruder                                                                                                        |
| GPIO24      | RGB                | P24       | RGB            | also `LED7` internal RGB-LED                                                                                         |
| GPIO25      | Z-STOP             | P6        | Z-STOP         | physical limit-switch OR driver DIAG                                                                                 |
| GPIO26_ADC0 | THB                | P3        | THB            | Thermistor Bed                                                                                                       |
| GPIO27_ADC1 | TH0                | P7        | TH0            | Thermistor Extruder                                                                                                  |
| GPIO28_ADC2 | ZDIR               |           |                | TMC2209 `Z`                                                                                                          |
| GPIO29_ADC3 | SERVOS ADC3        | P11       | SERVOS         | goes directly to the SERVOS pin. NO PROTECTION! (additionally it is prepared but not placed to measure the 5V rail?) |
|             |                    |           |                |                                                                                                                      |
|             | RUN                | P9.1      | SWD            |                                                                                                                      |
| SWCLK       | SWCLK              | P9.2      | SWD            |                                                                                                                      |
|             | GND                | P9.3      | SWD            |                                                                                                                      |
| SWDIO       | SWDIO              | P9.4      | SWD            |                                                                                                                      |
|             | 3.3V               | P9.5      | SWD            |                                                                                                                      |
|             |                    |           |                |                                                                                                                      |

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
not so simple..
you can get the 3.3V Power from the SWD pin-header (`P9 SWD`).
PIN 3 is GND; PIN 5 is 3.3V
you could map 
    - GPIO0 for I2C0_SDA
    - GPIO1 for I2C0_SCL
these are available on `P5 5V_POWER` (the connector designed for connecting a RPI)
https://electronics.stackexchange.com/a/687771/13800

## Helpfull links

- https://www.analog.com/media/en/technical-documentation/data-sheets/TMC2209_datasheet_rev1.08.pdf
- https://bigtreetech-beta.github.io/docs/SKR%20Pico.html
- https://github.com/bigtreetech/SKR-Pico/
    - https://github.com/bigtreetech/SKR-Pico/blob/master/Hardware/BTT%20SKR%20Pico%20V1.0-SCH.pdf
- https://www.reddit.com/r/BIGTREETECH/comments/1aqu1ub/skr_pico_for_nonprinter_projects/
- 
where to buy?
https://www.tinytronics.nl/en/development-boards/3d-printing/bigtreetech-skr-pico-v1.0-3d-printer-motherboard?sort=p.price&order=ASC


