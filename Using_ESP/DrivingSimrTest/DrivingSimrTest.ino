/*
 * Driving controller test
 */

#include <Arduino.h>
#include <BleGamepad.h>

#define numOfButtons 10
#define numOfHatSwitches 0
#define enableX false
#define enableY false
#define enableZ false
#define enableRX false
#define enableRY false
#define enableRZ false
#define enableSlider1 false
#define enableSlider2 false
#define enableRudder false
#define enableThrottle false
#define enableAccelerator true
#define enableBrake true
#define enableSteering true

const int potPinA = 34;               // Potentiometer is connected to GPIO 34 (Analog ADC1_CH6)
const int potPinS = 35;                
const int potPinB = 32; 
const int numberOfPotSamples = 5;     // Number of pot samples to take (to smooth the values)
const int delayBetweenSamples = 4;    // Delay in milliseconds between pot samples
const int delayBetweenHIDReports = 5; // Additional delay in milliseconds between HID reports


BleGamepad bleGamepad("BLE Driving Controller", "lemmingDev", 100);

void setup()
{
    Serial.begin(115200);
    Serial.println("Starting BLE work!");

    // Setup controller with 10 buttons, accelerator, brake and steering
    BleGamepadConfiguration bleGamepadConfig;
    bleGamepadConfig.setAutoReport(false);
    bleGamepadConfig.setControllerType(CONTROLLER_TYPE_GAMEPAD); // CONTROLLER_TYPE_JOYSTICK, CONTROLLER_TYPE_GAMEPAD (DEFAULT), CONTROLLER_TYPE_MULTI_AXIS
    bleGamepadConfig.setButtonCount(numOfButtons);
    bleGamepadConfig.setWhichAxes(enableX, enableY, enableZ, enableRX, enableRY, enableRZ, enableSlider1, enableSlider2);      // Can also be done per-axis individually. All are true by default
    bleGamepadConfig.setWhichSimulationControls(enableAccelerator, enableThrottle, enableRudder, enableBrake, enableSteering); // Can also be done per-control individually. All are false by default
    bleGamepadConfig.setHatSwitchCount(numOfHatSwitches);                                                                      // 1 by default
    // Some non-Windows operating systems and web based gamepad testers don't like min axis set below 0, so 0 is set by default
    bleGamepadConfig.setAxesMin(0x8001); // -32767 --> int16_t - 16 bit signed integer - Can be in decimal or hexadecimal
    bleGamepadConfig.setAxesMax(0x7FFF); // 32767 --> int16_t - 16 bit signed integer - Can be in decimal or hexadecimal 
    
	bleGamepad.begin(&bleGamepadConfig);
    // changing bleGamepadConfig after the begin function has no effect, unless you call the begin function again

    // Set accelerator and brake to min
    bleGamepad.setSlider1(-32767);
    bleGamepad.setSlider2(-32767);
    bleGamepad.setRudder(-32767);
    // Set steering to center
    bleGamepad.setSteering(0);
}

void loop()
{
    if (bleGamepad.isConnected())
    {    
        int potValuesA[numberOfPotSamples];
        int potValuesS[numberOfPotSamples]; // Array to store pot readings
        int potValuesB[numberOfPotSamples];
        int potValueA = 0;                  // Variable to store calculated pot reading average
        int potValueS = 0;
        int potValueB = 0;
        // Populate readings
        for (int i = 0; i < numberOfPotSamples; i++)
        {
            potValuesA[i] = analogRead(potPinA);
            potValueA += potValuesA[i];
            potValuesS[i] = analogRead(potPinS);
            potValueS += potValuesS[i];
            potValuesB[i] = analogRead(potPinB);
            potValueB += potValuesB[i];
            delay(delayBetweenSamples);
        }

        // Calculate the average
        potValueA = potValueA / numberOfPotSamples;
        potValueS = potValueS / numberOfPotSamples;
        potValueB = potValueB / numberOfPotSamples;
        // Map analog reading from 0 ~ 4095 to 32737 ~ 0 for use as an axis reading
        int adjustedValueA = map(potValueA, 0, 4095, -32767, 32767);
        int adjustedValueS = map(potValueS, 0, 4095, -32767, 32767);
        int adjustedValueB = map(potValueB, 0, 4095, -32767, 32767);

        bleGamepad.setSteering(adjustedValueS);
        bleGamepad.setZ(adjustedValueA);
        bleGamepad.setRX(adjustedValueB);
        bleGamepad.setRudder(adjustedValueA);
        bleGamepad.sendReport();
        delay(delayBetweenHIDReports);
    //     Serial.println("Press all buttons one by one");
    //     for (int i = 1; i <= numOfButtons; i += 1)
    //     {
    //         bleGamepad.press(i);
    //         bleGamepad.sendReport();
    //         delay(100);
    //         bleGamepad.release(i);
    //         bleGamepad.sendReport();
    //         delay(25);
    //     }

    //     Serial.println("Move steering from center to max");
    //     for (int i = 0; i > -32767; i -= 256)
    //     {
    //         bleGamepad.setSteering(i);
    //         bleGamepad.sendReport();
    //         delay(10);
    //     }

    //     Serial.println("Move steering from min to max");
    //     for (int i = -32767; i < 32767; i += 256)
    //     {
    //         bleGamepad.setSteering(i);
    //         bleGamepad.sendReport();
    //         delay(10);
    //     }

    //     Serial.println("Move steering from max to center");
    //     for (int i = 32767; i > 0; i -= 256)
    //     {
    //         bleGamepad.setSteering(i);
    //         bleGamepad.sendReport();
    //         delay(10);
    //     }
    //     bleGamepad.setSteering(0);
    //     bleGamepad.sendReport();

    //     Serial.println("Move accelerator from min to max");
    //     // for(int i = 32767 ; i > -32767 ; i -= 256)    //Use this for loop setup instead if accelerator is reversed
    //     for (int i = -32767; i < 32767; i += 256)
    //     {
    //         bleGamepad.setAccelerator(i);
    //         bleGamepad.sendReport();
    //         delay(10);
    //     }
    //     bleGamepad.setAccelerator(-32767);
    //     bleGamepad.sendReport();

    //     Serial.println("Move brake from min to max");
    //     for (int i = -32767; i < 32767; i += 256)
    //     {
    //         bleGamepad.setBrake(i);
    //         bleGamepad.sendReport();
    //         delay(10);
    //     }
    //     bleGamepad.setBrake(-32767);
    //     bleGamepad.sendReport();
     }
}