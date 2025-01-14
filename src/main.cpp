#include <M5Atom.h>

uint8_t max_result_value = 2;

// The colour of the dots which show the number.
const CRGB g_foregroundColour{ 0x303024 };

// The colour of all the other pixels.
const CRGB g_backgroundColour{ 0x190000 };

// Get a random number between 1 and 6 (inclusive).
uint8_t getRandomDiceRoll()
{
  // Any disconnected GPIO pin should work here:
  return (analogRead(32) % 6) + 1;
}

int getRandomNumber(int minVal, int maxVal) {
    if (minVal > maxVal) {
        // Swap minVal and maxVal if they are in the wrong order
        int temp = minVal;
        minVal = maxVal;
        maxVal = temp;
    }
    return minVal + esp_random() % (maxVal - minVal + 1);
}

// Display the specified dice number on the matrix, in the specified colour.
void drawNumber(const uint8_t number, const CRGB colour)
{
  switch (number)
  {
  case 1:
    M5.dis.drawpix(12, colour);
    break;
  
  case 2:
    M5.dis.drawpix(8, colour);
    M5.dis.drawpix(16, colour);
    break;

  case 3:
    M5.dis.drawpix(8, colour);
    M5.dis.drawpix(12, colour);
    M5.dis.drawpix(16, colour);
    break;

  case 4:
    M5.dis.drawpix(6, colour);
    M5.dis.drawpix(8, colour);
    M5.dis.drawpix(16, colour);
    M5.dis.drawpix(18, colour);
    break;

  case 5:
    M5.dis.drawpix(6, colour);
    M5.dis.drawpix(8, colour);
    M5.dis.drawpix(12, colour);
    M5.dis.drawpix(16, colour);
    M5.dis.drawpix(18, colour);
    break;

  case 6:
    M5.dis.drawpix(6, colour);
    M5.dis.drawpix(8, colour);
    M5.dis.drawpix(11, colour);
    M5.dis.drawpix(13, colour);
    M5.dis.drawpix(16, colour);
    M5.dis.drawpix(18, colour);
    break;
  }
}

void drawBackground(){
for (uint8_t i{ 0 }; i < 25; ++i)
      M5.dis.drawpix(i, g_backgroundColour);
}

void setup()
{
  // Enable the matrix display, and ensure it's initially empty.
  M5.begin(false, false, true);
  
  M5.IMU.Init(); // Initialize the MPU6886

  M5.dis.clear();

  drawBackground();

  drawNumber(max_result_value, g_foregroundColour);
  //M5.dis.setBrightness(32);
}

void loop()
{
  //M5.dis.setBrightness(18);
  // Clear the matrix display when the button is pushed down.
  if (M5.Btn.wasPressed())
  {
    M5.dis.clear();
  }

  // Show a random number when the button is released.
  if (M5.Btn.wasReleased())
  {

    max_result_value += 1;
    if (max_result_value > 6){
      max_result_value = 2;
    } 
    
    drawBackground();
    drawNumber(max_result_value, g_foregroundColour);
    //drawNumber(getRandomDiceRoll(), g_foregroundColour);
  }

  float accX, accY, accZ;
  static unsigned long lastShakeTime = 0;

  // Read acceleration values
  M5.IMU.getAccelData(&accX, &accY, &accZ);

  // Calculate magnitude of acceleration
  float magnitude = sqrt(accX * accX + accY * accY + accZ * accZ);

  // Detect shake if magnitude exceeds a threshold
  if (magnitude > 8.0 && millis() - lastShakeTime > 500) {
    //Serial.println("Shake detected!");
    lastShakeTime = millis();
    
    //M5.dis.setBrightness(64);
    uint8_t randVal = getRandomNumber(2, max_result_value);
    for (int i=0; i<=getRandomNumber(10, 30); i++){
      M5.dis.clear();
      
      if(i%2 == 0){
        drawBackground();
        randVal = getRandomNumber(1, max_result_value);
      }
      
      drawNumber(randVal, g_foregroundColour);

      delay(getRandomNumber(100, 200));
      M5.update();
    }
    
    drawBackground();
    drawNumber(getRandomNumber(1, max_result_value), g_foregroundColour);
    
  }
  
  delay(25);
  
  M5.update();
}