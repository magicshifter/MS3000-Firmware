//
// Accelerometer modules - written specifically for MMA8452
//
#include <Wire.h> // Used for I2C
#include <math.h>
#include <Wire.h> // Used for I2C

//Define a few of the registers that we will be accessing on the MMA8452
#define OUT_X_MSB 0x01
#define XYZ_DATA_CFG  0x0E
#define WHO_AM_I   0x0D
#define CTRL_REG1  0x2A

#define GSCALE 8 // Sets full-scale range to +/-2, 4, or 8g. Used to calc real g values.

#define PIN_I2C_DATA 5 // 5 //blau // labeled 5 on esp12E!!!
#define PIN_I2C_CLOCK 4 //lila

#ifndef CONFIG_ENABLE_ACCEL
void resetAccelerometer() {};
void readAccelData(int *destination) {};
#endif

// normal MMA 
#ifdef CONFIG_MMA_NORMAL
  #define MMA8452_ADDRESS 0x1C
  #define MMA8452_ID 0x2A
#else
#ifdef CONFIG_MMA_FSOX
// magnet sensor version
    #define MMA8452_ADDRESS 0x1E
    #define MMA8452_ID 0x2A
  #endif
#endif


// TODO : Flesh this out?
class Accelerometer
{
  bool update();

  void setConfig();
  void getConfig();
  void defaulConfig(); // free running mode :)
  void getValues(float *accel); // 3 values
  void getValues(int *accel); // 3 values (fixed point almost raw data from sensor);

  void getAverage(int *accel);
};

void initAccelerometer()
{
  //Wire.pins(PIN_I2C_DATA, PIN_I2C_CLOCK);
  Wire.begin(PIN_I2C_DATA, PIN_I2C_CLOCK); //Join the bus as a master
  Wire.setClock(500000); // crashes if > 200000
}


#ifdef CONFIG_ENABLE_ACCEL
// Read bytesToRead sequentially, starting at addressToRead into the dest byte array
void readRegisters(byte addressToRead, int bytesToRead, byte * dest)
{
  Wire.beginTransmission(MMA8452_ADDRESS);
  Wire.write(addressToRead);
  Wire.endTransmission(false); //endTransmission but keep the connection active

  //delay(0);

  Wire.requestFrom(MMA8452_ADDRESS, bytesToRead); //Ask for bytes, once done, bus is released by default

  if (Wire.available() < bytesToRead)
  {
    // msSystem.logln("I2C not available. This should NEVER happen!");
    delay(10);
  }; //Hang out until we get the # of bytes we expect

  for(int x = 0 ; x < bytesToRead ; x++)
  {
    dest[x] = Wire.read();
    yield();
  }
}

// Read a single byte from addressToRead and return it as a byte
byte readRegister(byte addressToRead)
{
  Wire.beginTransmission(MMA8452_ADDRESS);
  Wire.write(addressToRead);
  Wire.endTransmission(false); //endTransmission but keep the connection active

  Wire.requestFrom(MMA8452_ADDRESS, 1); //Ask for 1 byte, once done, bus is released by default

  if (!Wire.available()){
    // msSystem.logln("I2C not available. This should NEVER happen!");
    delay(10);
  } ; //Wait for the data to come back
  return Wire.read(); //Return this one byte
}

// Writes a single byte (dataToWrite) into addressToWrite
void writeRegister(byte addressToWrite, byte dataToWrite)
{
  Wire.beginTransmission(MMA8452_ADDRESS);
  Wire.write(addressToWrite);
  Wire.write(dataToWrite);
  Wire.endTransmission(); //Stop transmitting
}


// Sets the MMA8452 to standby mode. It must be in standby to change most register settings
void MMA8452Standby()
{
  byte c = readRegister(CTRL_REG1);
  writeRegister(CTRL_REG1, c & ~(0x01)); //Clear the active bit to go into standby
}

// Sets the MMA8452 to active mode. Needs to be in this mode to output data
void MMA8452Active()
{
  byte c = readRegister(CTRL_REG1);
  writeRegister(CTRL_REG1, c | 0x01); //Set the active bit to begin detection
}

void readAccelData(int *destination)
{
  long _ti = micros();

  byte rawData[6];  // x/y/z accel register data stored here

  readRegisters(OUT_X_MSB, 6, rawData);  // Read the six raw data registers into data array


  // Loop to calculate 12-bit ADC and g value for each axis
  for(int i = 0; i < 3 ; i++)
  {
    int gCount = (rawData[i*2] << 8) | rawData[(i*2)+1];  //Combine the two 8 bit registers into one 12-bit number
    gCount >>= 4; //The registers are left align, here we right align the 12-bit integer

    // If the number is negative, we have to make it so manually (no 12-bit data type)
    if (rawData[i*2] > 0x7F)
    {
      gCount -= 0x1000;
    }

    destination[i] = gCount; //Record this gCount into the 3 int array
  }

  msGlobals.accelTime = micros() - _ti;
}

// Initialize the MMA8452 registers
// See the many application notes for more info on setting all of these registers:
// http://www.freescale.com/webapp/sps/site/prod_summary.jsp?code=MMA8452Q
void resetAccelerometer()
{
  byte success = 0;

  do {
    byte c = readRegister(WHO_AM_I);  // Read WHO_AM_I register
    if (c == MMA8452_ID) // WHO_AM_I should always be 0x2A
    {
      // msSystem.logln("MMA8452Q is online...");
      success = 1;
    }
    else
    {
      // msSystem.logln("Could not connect to MMA8452Q: expected 0x");
      // msSystem.logln(String(MMA8452_ID, HEX));
      // msSystem.logln(" but received 0x");
      // msSystem.logln(String(c, HEX));
      delay(10);
    }
    /* code */
  } while(0); //while(!success);

  MMA8452Standby();  // Must be in standby to change registers

  // Set up the full scale range to 2, 4, or 8g.
  byte fsr = GSCALE;
  if(fsr > 8) fsr = 8; //Easy error check
  fsr >>= 2; // Neat trick, see page 22. 00 = 2G, 01 = 4A, 10 = 8G
  writeRegister(XYZ_DATA_CFG, fsr);

  //The default data rate is 800Hz and we don't modify it in this example code

  MMA8452Active();  // Set to active to start reading
}
#endif