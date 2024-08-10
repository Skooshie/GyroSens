#include "mbed.h"
#include "math.h"
#include "LSM6DSLSensor.h"
#include "stdio.h"

// Setup value of PI
#define PI 3.141592654

// Setup device to use I2C
static DevI2C devI2c(PB_11, PB_10);
static LSM6DSLSensor acc_gyro(&devI2c, 0xD4, D4, D5); // high address

// Define the PWM period (you can adjust this if needed)
#define PWM_PERIOD 0.01f // 10 ms period (100 Hz)
#define FADE_DELAY 100ms  // Blinking rate in milliseconds (for fading effect)

// Initialize the LED1, LED2, + LED3 
PwmOut led1(LED1);
PwmOut led2(LED2);
PwmOut led3(LED3);

// Compute the angle using x, y, and z values
float computeAngle(int x, int y, int z) {
    float angle = atan2(y, sqrt(x * x + z * z)) * 180.0 / PI;
    return angle;
}

// Setup main function 
int main() {
    uint8_t id;
    int32_t axes[3];
    float res = 0;
    acc_gyro.init(NULL);

    acc_gyro.enable_x();
    acc_gyro.enable_g();

    printf("This is an accelerometer example running on Mbed OS %d.%d.%d.\n", MBED_MAJOR_VERSION, MBED_MINOR_VERSION, MBED_PATCH_VERSION);
    acc_gyro.read_id(&id);
    printf("LSM6DSL accelerometer & gyroscope = 0x%X\r\n", id);

    // Set the PWM period for all LEDs
    led1.period(PWM_PERIOD);
    led2.period(PWM_PERIOD);
    led3.period(PWM_PERIOD);

    while (1) {
        acc_gyro.get_x_axes(axes);
        res = computeAngle(axes[0], axes[1], axes[2]);

        // Print the accelerometer values and computed angle
        printf("LSM6DSL: %6d, %6d, %6d, %3.2f degrees\r\n", axes[0], axes[1], axes[2], res);

        // Set the LEDs brightness using the x, y, and z axes values//
        float brightness1 = fabs(axes[0] / 32768.0f); // Normalize x-axis value
        float brightness2 = fabs(axes[1] / 32768.0f); 
        float brightness3 = fabs(axes[2] / 32768.0f); 

        led1.write(brightness1); // Set the PWM duty cycle for LED1
        led2.write(brightness2);
        led3.write(brightness3); 

        // Add a small delay to visualize the changes
        ThisThread::sleep_for(FADE_DELAY);
    }
}
