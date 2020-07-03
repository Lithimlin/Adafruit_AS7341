
#include <Adafruit_AS7341.h>
#include <Wire.h>
Adafruit_AS7341 as7341;
void setup() {

  // Initiate the Wire library and join the I2C bus as a master or slave
  Wire.begin();

  // communication with the host computer serial monitor
  Serial.begin(9600);
  while (!Serial) {
    delay(1);
  }
}

void loop() {

  // Sets the Atime for integration time from 0 to 255 in register (0x81),
  // integration time = (ATIME + 1) * (ASTEP + 1) * 2.78µS

  as7341.setATIME(byte(0x64));

  // Sets the Astep for integration time from 0 to 65535 in register (0xCA[7:0])
  // and (0xCB[15:8]), integration time = (ATIME + 1) * (ASTEP + 1) * 2.78µS
  as7341.setASTEP(byte(0xE7), byte(0x03));

  // Sets the Spectral Gain in CFG1 Register (0xAA) in [4:0] bit
  as7341.setGAIN(byte(0x09));

  // Function defined to read out channels with SMUX configration 1- F1-F4,
  // Clear, NIR
  as7341.readRawValuesMode1();

  // Function defined to read out channels with SMUX configration 2- F5-F8,
  // Clear, NIR
  as7341.readRawValuesMode2();
  delay(1000);

  //  // Function detects Flicker for 100 and 120 Hz
  //  flickerDetection();

  as7341.flickerDetection1K();

  // reading the flicker status in FD_STATUS register 0xDB
  // data=0x2c=0b00101100  FD_STATUS(fd_measurement_valid=1
  // fd_120Hz_flicker_valid=1 fd_100Hz_flicker_valid=1) data=0x2d=0b00101101
  // FD_STATUS(fd_measurement_valid=1 fd_1200Hz_flicker_valid=1
  // fd_1000Hz_flicker_valid=1 fd_1000Hz_flicker) data=0x2e=0b00101110
  // FD_STATUS(fd_measurement_valid=1 fd_1200Hz_flicker_valid=1
  // fd_1000Hz_flicker_valid=1 fd_1200Hz_flicker)

  int flicker_value = as7341.readRegister(byte(0xDB));
  //             Serial.print("Flicker Status-");
  //             Serial.println(flicker_value);

  Serial.println("** 1k flicker test **");
  if (flicker_value == 44) {
    Serial.println("Unknown frequency");
  } else if (flicker_value == 45) {
    Serial.println("1000 Hz detected");
  } else if (flicker_value == 46) {
    Serial.println("1200 Hz detected");
  } else {
    Serial.println("Error in reading");
  }

  Serial.println("** 1k flicker test end **");
  delay(500);
}
