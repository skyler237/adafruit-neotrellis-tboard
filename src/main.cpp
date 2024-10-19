#include <Arduino.h>
/* This example shows basic usage of the
MultiTrellis object controlling an array of
NeoTrellis boards

As is this example shows use of two NeoTrellis boards
connected together with the leftmost board having the
default I2C address of 0x2E, and the rightmost board
having the address of 0x2F (the A0 jumper is soldered)
*/

#include "Adafruit_NeoTrellis.h"
#include "trellis_hw_interface.h"

// #define Y_DIM 4 //number of rows of key
// #define X_DIM 8 //number of columns of keys

// //create a matrix of trellis panels
// Adafruit_NeoTrellis t_array[Y_DIM/4][X_DIM/4] = {

//   { Adafruit_NeoTrellis(0x2E), Adafruit_NeoTrellis(0x2F) }

// };

// If you were using a 2x2 array of NeoTrellis boards, the above lines would be:

#define Y_DIM 8 // number of rows of key
#define X_DIM 8 // number of columns of keys

// create a matrix of trellis panels
Adafruit_NeoTrellis t_array[Y_DIM / 4][X_DIM / 4] = {

    {Adafruit_NeoTrellis(0x2F), Adafruit_NeoTrellis(0x2E)},

    {Adafruit_NeoTrellis(0x31), Adafruit_NeoTrellis(0x30)}
};

// pass this matrix to the multitrellis object
Adafruit_MultiTrellis trellis((Adafruit_NeoTrellis*)t_array, Y_DIM / 4, X_DIM / 4);

static TrellisHWInterface trellis_hw_interface = TrellisHWInterface::get_instance();

Adafruit_MultiTrellis& get_trellis() {
    // return trellis;
    return trellis_hw_interface.get_trellis();
}

// Input a value 0 to 255 to get a color value.
// The colors are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
    if (WheelPos < 85) {
        return seesaw_NeoPixel::Color(WheelPos * 3, 255 - WheelPos * 3, 0);
    } else if (WheelPos < 170) {
        WheelPos -= 85;
        return seesaw_NeoPixel::Color(255 - WheelPos * 3, 0, WheelPos * 3);
    } else {
        WheelPos -= 170;
        return seesaw_NeoPixel::Color(0, WheelPos * 3, 255 - WheelPos * 3);
    }
    return 0;
}

// define a callback for key presses
TrellisCallback blink(keyEvent evt) {
    if (evt.bit.EDGE == SEESAW_KEYPAD_EDGE_RISING) {
        get_trellis().setPixelColor(evt.bit.NUM, Wheel(map(evt.bit.NUM, 0, X_DIM * Y_DIM, 0, 255))); // on rising
    } else if (evt.bit.EDGE == SEESAW_KEYPAD_EDGE_FALLING) {
        get_trellis().setPixelColor(evt.bit.NUM, 0); // off falling
    }

    get_trellis().show();
    return 0;
}

void setup() {
    trellis_hw_interface.begin();

    // /* the array can be addressed as x,y or with the key number */
    // for(int i=0; i<Y_DIM*X_DIM; i+=X_DIM){
    //     get_trellis().setPixelColor(i, Wheel(map(i, 0, X_DIM*Y_DIM, 0, 255))); //addressed with keynum
    //     get_trellis().show();
    //     delay(50);
    // }
    //
    // for(int y=0; y<Y_DIM; y++){
    //   for(int x=0; x<X_DIM; x++){
    //     //activate rising and falling edges on all keys
    //     // get_trellis().activateKey(x, y, SEESAW_KEYPAD_EDGE_RISING, true);
    //     // get_trellis().activateKey(x, y, SEESAW_KEYPAD_EDGE_FALLING, true);
    //     // get_trellis().registerCallback(x, y, blink);
    //     get_trellis().setPixelColor(x, y, 0x000000); //addressed with x,y
    //     get_trellis().show(); //show all LEDs
    //     delay(50);
    //     break;
    //   }
    // }

    // trellis_hw_interface.begin();
    for (int i = 0; i < Y_DIM * X_DIM; i++) {
        int x = i % X_DIM;
        int y = i / X_DIM;
        trellis_hw_interface.set_pixel_color(x, y, wheel_rgb(map(i, 0, X_DIM * Y_DIM, 0, 255))); // addressed with
                                                                                                 // keynum
        trellis_hw_interface.show();
        delay(20);
    }

    for (int y = 0; y < Y_DIM; y++) {
        for (int x = 0; x < X_DIM; x++) {
            // activate rising and falling edges on all keys
            //  trellis_hw_interface.activateKey(x, y, SEESAW_KEYPAD_EDGE_RISING, true);
            //  trellis_hw_interface.activateKey(x, y, SEESAW_KEYPAD_EDGE_FALLING, true);
            //  trellis_hw_interface.registerCallback(x, y, blink);
            trellis_hw_interface.clear_pixel(x, y); // addressed with x,y
            trellis_hw_interface.show();            // show all LEDs
            delay(20);
            // break;
        }
    }
    TrellisHWInterface::get_instance().register_on_any_key_pressed_callback([](int x, int y, const Time&) {
        TrellisHWInterface::get_instance().set_pixel_color(x, y, wheel_rgb((y * 16) + (x * 2)));
        TrellisHWInterface::get_instance().show();
    });
    TrellisHWInterface::get_instance().register_on_any_key_released_callback([](int x, int y, const Time&) {
        TrellisHWInterface::get_instance().clear_pixel(x, y);
        TrellisHWInterface::get_instance().show();
    });

    TrellisHWInterface::get_instance().register_timer_callback(10, [](const Time& now) {
        // Go from 0 to 255 and back every second
        const uint16_t period = 2000;
        const uint16_t half_period = period / 2;
        const uint16_t color_period = period * 10;
        const uint16_t color_half_period = color_period / 2;

        uint16_t brightness_idx = now % period;
        if (brightness_idx > half_period) {
            brightness_idx = period - brightness_idx;
        }

        uint16_t color_idx = now % color_period;
        if (color_idx > color_half_period) {
            color_idx = color_period - color_idx;
        }

        const uint8_t brightness = static_cast<uint8_t>(map(brightness_idx, 0, half_period, 0, 255));
        const uint8_t color = static_cast<uint8_t>(map(color_idx, 0, color_half_period, 0, 255));
        TrellisHWInterface::get_instance().set_pixel_color(0, 0, wheel_rgb(color, brightness));
        TrellisHWInterface::get_instance().show();
        // Serial.printf("Timer event @ %.3f s\n", static_cast<double>(now) / 1000);
    });
}

void loop() {
    TrellisHWInterface::get_instance().tick();
}
