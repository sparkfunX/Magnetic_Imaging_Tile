/*
 This basic example shows how to interface with the Magnetic Imaging Tile V3
 with a Chipkit MAX32, with data displayed via the serial port.
 Simple serial commands allow viewing the data in a serial console, or
 sending the data to a program on a host system (such as the accompanying
 Processing example) for display.

 The code primarily uses the internal ADC of the Chipkit MAX32, but
 (slow) code is also provided for using the AD7940 on the tile as
 well.  The speed of the ADC and I/O clocking is essentially the
 limiting factor in framerate, and this should be kept in mind
 when porting to other platforms (e.g. the Arduino Uno).

 The Chipkit MAX32 provides ~128K of RAM, which is enough for a
 short framebuffer (up to about 500 frames) when recording
 high-speed data.

 This code was written in MPIDE 0023-windows-20140316 .
*/

//Define where to pipe serial print statements
//#define terminal Serial //Use with Uno and Chipkit
#define terminal SerialUSB //Use with SAMD21

#define CS_INACTIVE  1
#define CS_ACTIVE    0

//Hardware connections
const byte PIN_ANALOG = A1;

//On Arduino Unos and Red Boards this is the SPI pinout
const byte PIN_CLR = 8;
const byte PIN_CLK = 9;
const byte AD7940_SPI_MISO = 12;
const byte AD7940_SPI_CS = 10;
const byte AD7940_SPI_CLK = 13;

// Frames
#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)

//We have very limited RAM
#define MAX_BASE_FRAMES  2

#else

//Platforms like SAMD21, ChipKit, and Teensy have far more RAM
//#define MAX_BASE_FRAMES  500
//#define MAX_BASE_FRAMES  250
#define MAX_BASE_FRAMES  100
//#define _8BIT

#endif

#ifdef _8BIT
#define MAX_FRAMES      MAX_BASE_FRAMES*2
uint8_t frames[MAX_FRAMES][64];
#else
#define MAX_FRAMES      MAX_BASE_FRAMES
uint16_t frames[MAX_FRAMES][64];
#endif

// Frame variables
// Magnetic tile reading
int pixelOrder[] = {26, 27, 18, 19, 10, 11, 2, 3, 1, 0, 9, 8, 17, 16, 25, 24};
int subtileOrder[] = {0, 2, 1, 3};
int subtileOffset[] = {0, 4, 32, 36};
uint16_t frame[64];

int numFrames = 0;
int curFrame = 0;

#define MODE_IDLE          0
#define MODE_LIVE          1
#define MODE_HIGHSPEED1    2
#define MODE_HIGHSPEED2    3
#define MODE_HIGHSPEED3    4
#define MODE_HIGHSPEED4    5
#define MODE_PIXEL         6

int curMode = MODE_IDLE;

/*
   Analog Read
*/
int readMagnetometer() {
  //return 0;
  //return analogRead(PIN_ANALOG);
  //return readInternalADC();
  return readAD7940();
  //return read_ad7940();
}

// Take one measurement form the internal ADC
int readInternalADC() {
  int numSamples = 2;
  float sum = 0.0f;
  for (int i = 0; i < numSamples; i++) {
    int sensorValue = analogRead(PIN_ANALOG);
    sum += sensorValue;
  }
  sum /= numSamples;

  //return sensorValue;
  return int(floor(sum));
}


// Take one measurement from an external AD7940 14-bit ADC (SPI)
uint16_t readAD7940() {
  uint16_t value = 0;
  //uint16_t delay_time = 2;

  // Idle
  digitalWrite(AD7940_SPI_CS, HIGH);
  digitalWrite(AD7940_SPI_CLK, HIGH);

  // Enable
  digitalWrite(AD7940_SPI_CS, LOW);

  // Read 16 bits
  for (int i = 0; i < 16; i++) {
    char bit = digitalRead(AD7940_SPI_MISO);
    digitalWrite(AD7940_SPI_CLK, LOW);
    //    delayMicroseconds(delay_time);

    value = value << 1;
    value = value + (bit & 0x01);
    digitalWrite(AD7940_SPI_CLK, HIGH);
    //    delayMicroseconds(delay_time);
  }
  // Disable
  digitalWrite(AD7940_SPI_CS, HIGH);
  //  delayMicroseconds(delay_time);

  return value;
}

/*
   Magnetic Sensors
*/
void clearCounter() {
  digitalWrite(PIN_CLR, 0);
  //delay(10);
  digitalWrite(PIN_CLR, 1);
  //delay(10);
}

void incrementCounter() {
  digitalWrite(PIN_CLK, 1);
  //delay(1);
  digitalWrite(PIN_CLK, 0);
  //delay(1);
}

/*
   Capture one frame from imaging array
*/
void readTileFrame() {

  clearCounter();
  incrementCounter();

  for (int curSubtileIdx = 0; curSubtileIdx < 4; curSubtileIdx++) {
    for (int curIdx = 0; curIdx < 16; curIdx++) {
      // Read value
      int value = readMagnetometer();

      //terminal.println(value);
      //delay(10);

      // Store value in correct frame location
      int frameOffset = pixelOrder[curIdx] + subtileOffset[subtileOrder[curSubtileIdx]];
      //terminal.println(frameOffset);
      //delay(25);
      frame[frameOffset] = value;

      // Increment to next pixel
      incrementCounter();
    }
  }
}

// Display current frame on serial console
void displayCurrentFrame() {
  // Display frame
  //  terminal.println ("\nCurrent Frame");
  int idx = 0;
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      terminal.print( frame[idx] );
      terminal.print( " " );
      idx += 1;
    }
    terminal.println("");
  }
  terminal.println("*");
}

// Record MAX_FRAMES, as fast as possible
void recordHighSpeedFrames(int frameDelayTime) {
  long startTime = millis();

  for (int curFrame = 0; curFrame < MAX_FRAMES; curFrame++) {
    // Read one frame
    readTileFrame();

    // Store frame
#ifdef _8BIT
    for (int a = 0; a < 64; a++) {
      frames[curFrame][a] = frame[a] >> 2;
    }
#else
    for (int a = 0; a < 64; a++) {
      frames[curFrame][a] = frame[a];
    }
#endif

    if (frameDelayTime > 0) {
      delay(frameDelayTime);
    }
  }

  long endTime = millis();
  terminal.print("Framerate: ");
  terminal.println((float)MAX_FRAMES / ((float)(endTime - startTime) / 1000.0f));
}

// Playback the high speed frames stored
void playbackHighSpeedFrames() {

  for (int curFrame = 0; curFrame < MAX_FRAMES; curFrame++) {
    // Display frame
    //    terminal.print ("\nFrame ");
    //    terminal.println (curFrame);

    int idx = 0;
    for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 8; j++) {
        terminal.print( frames[curFrame][idx] );
        terminal.print( " " );
        idx += 1;
      }
      terminal.println("");
    }
    terminal.println("*");
    delay(50);
  }
}

long startTime = 0;

void setup() {
  // Setup pin modes
  pinMode(PIN_CLR, OUTPUT);
  pinMode(PIN_CLK, OUTPUT);

  // AD7940 ADC Pin modes
  pinMode(AD7940_SPI_CS, OUTPUT);
  pinMode(AD7940_SPI_CLK, OUTPUT);
  pinMode(AD7940_SPI_MISO, INPUT);

  // Setup initial states
  incrementCounter();
  clearCounter();

  // initialize serial:
  terminal.begin(115200);
  while(!terminal); //Wait for user to open this terminal. Useful for SAMD21 and boards that use CDC.

  terminal.println ("Initializing.... Press L (live) or H (high speed), 1, 2, 3, or S (idle)");

  clearCounter();
  incrementCounter();
  delay(100);

  startTime = millis();
}

void loop() {

  //Parse serial data (if any)
  if(terminal.available())
  {
    byte incoming = terminal.read();
    
    if (incoming == 'L') {
      terminal.println("Live");
      curMode = MODE_LIVE;

    } else if (incoming == 'H' || incoming == '1') {
      terminal.println("High-speed Save1");
      curMode = MODE_HIGHSPEED1;

    } else if (incoming == '2') {
      terminal.println("High-speed Save2 (1000hz)");
      curMode = MODE_HIGHSPEED2;

    } else if (incoming == '3') {
      terminal.println("High-speed Save3 (500hz)");
      curMode = MODE_HIGHSPEED3;

    } else if (incoming == '4') {
      terminal.println("High-speed Save4 (250hz)");
      curMode = MODE_HIGHSPEED4;

    } else if (incoming == 'S') {
      terminal.println("Idle");
      curMode = MODE_IDLE;

    } else if (incoming == 'P') {
      terminal.println("Read Pixel 0,0");
      curMode = MODE_PIXEL;
    }
  }

  /*
     Take action based on current mode
  */
  if (curMode == MODE_LIVE) {
    readTileFrame();
    displayCurrentFrame();

  } else if (curMode == MODE_HIGHSPEED1) {
    // Full/maximum speed
    recordHighSpeedFrames(0);
    playbackHighSpeedFrames();
    curMode = MODE_IDLE;

    terminal.println ("Initializing.... Press L (live) or H (high speed), 1, 2, 3, or S (idle)");

  } else if (curMode == MODE_HIGHSPEED2) {
    // ~1000Hz
    recordHighSpeedFrames(1);
    playbackHighSpeedFrames();
    curMode = MODE_IDLE;

    terminal.println ("Initializing.... Press L (live) or H (high speed), 1, 2, 3, or S (idle)");

  } else if (curMode == MODE_HIGHSPEED3) {
    // ~500Hz
    recordHighSpeedFrames(2);
    playbackHighSpeedFrames();
    curMode = MODE_IDLE;

    terminal.println ("Initializing.... Press L (live) or H (high speed), 1, 2, 3, or S (idle)");

  } else if (curMode == MODE_HIGHSPEED4) {
    // ~250Hz
    recordHighSpeedFrames(4);
    playbackHighSpeedFrames();
    curMode = MODE_IDLE;

    terminal.println ("Initializing.... Press L (live) or H (high speed), 1, 2, 3, or S (idle)");

  } else if (curMode == MODE_PIXEL) {
    //Read single pixel. Good for testing.
    
    curMode = MODE_IDLE;

    terminal.println ("Initializing.... Press L (live) or H (high speed), 1, 2, 3, or S (idle)");
  }

  //delay(10);
}

