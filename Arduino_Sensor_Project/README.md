# Arduino Sensor Monitoring System

A modular Arduino project using a PIR sensor, DHT11, gas sensor and I2C LCD, with an optional Python program for reading the Arduino's serial output.

## Project Structure

```text
Arduino_Sensor_Project/
├── Arduino/
│   ├── Arduino_Sensor_Project.ino
│   ├── config/
│   │   └── Config.h
│   ├── sensors/
│   │   ├── PIRSensor.h
│   │   ├── PIRSensor.cpp
│   │   ├── DHTSensor.h
│   │   ├── DHTSensor.cpp
│   │   ├── GasSensor.h
│   │   └── GasSensor.cpp
│   └── display/
│       ├── LCDDisplay.h
│       └── LCDDisplay.cpp
│
├── Python/
│   ├── serial_reader.py
│   ├── sensor_data.py
│   └── requirements.txt
│
└── README.md
```

## Hardware Connections

| Component | Arduino |
|---|---|
| PIR OUT | D2 |
| DHT11 DATA | D3 |
| Gas Sensor AO | A0 |
| LCD SDA | A4 |
| LCD SCL | A5 |
| LCD VCC | 5V |
| LCD GND | GND |

## Arduino Libraries

Install through Arduino IDE Library Manager:

- DHT sensor library
- Adafruit Unified Sensor
- LiquidCrystal I2C

The LCD address is configured as `0x27`. If your LCD has a different I2C address, change `LCD_ADDRESS` in `Arduino/config/Config.h`.

## Uploading

Open:

`Arduino/Arduino_Sensor_Project.ino`

in the Arduino IDE and upload it to the board.

The `.h` and `.cpp` files are included in the same project directory, so Arduino IDE can compile the modules together.

## Python Setup

Open a terminal in the `Python` folder:

```bash
pip install -r requirements.txt
```

Then edit `serial_reader.py`:

```python
PORT = "COM3"
```

Replace `COM3` with the COM port assigned to your Arduino.

Run:

```bash
python serial_reader.py
```

## Serial Format

The Arduino sends machine-readable data in this format:

```text
DATA,motion,temperature,humidity,gas
```

Example:

```text
DATA,1,26.5,62.0,315
```

Meaning:

- `1` = motion detected
- `26.5` = temperature in Celsius
- `62.0` = humidity percentage
- `315` = raw gas sensor analog value

## Important

The gas threshold of `400` is only an example. MQ-series gas sensors should be calibrated for the specific sensor and environment.
