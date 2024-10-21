#include <Arduino.h>
/* This example shows basic usage of the
MultiTrellis object controlling an array of
NeoTrellis boards

As is this example shows use of two NeoTrellis boards
connected together with the leftmost board having the
default I2C address of 0x2E, and the rightmost board
having the address of 0x2F (the A0 jumper is soldered)
*/

#include <Prandom.h>

#include "Adafruit_NeoTrellis.h"
#include "trellis_controller.h"
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

Prandom RAND;

// create a matrix of trellis panels
Adafruit_NeoTrellis t_array[Y_DIM / 4][X_DIM / 4] = {

    {Adafruit_NeoTrellis(0x2F), Adafruit_NeoTrellis(0x2E)},

    {Adafruit_NeoTrellis(0x31), Adafruit_NeoTrellis(0x30)}
};

// pass this matrix to the multitrellis object
Adafruit_MultiTrellis trellis((Adafruit_NeoTrellis*)t_array, Y_DIM / 4, X_DIM / 4);

static TrellisHWInterfacePtr trellis_hw_interface = TrellisHWInterface::get_instance();
static tboard::TrellisController trellis_controller(trellis_hw_interface);

Adafruit_MultiTrellis& get_trellis() {
    // return trellis;
    return trellis_hw_interface->get_trellis();
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

void turn_on_random_pixel(const tboard::TrellisController& controller) {
    if (trellis_controller.display()->are_all_pixels_on()) {
        return;
    }

    bool found_blank_pixel = false;
    int x = RAND.randint(0, X_DIM - 1);;
    int y = RAND.randint(0, Y_DIM - 1);;
    while (!found_blank_pixel) {
        if (!controller.display()->is_pixel_on(x, y)) {
            found_blank_pixel = true;
        }
        // Try again
        x = RAND.randint(0, X_DIM - 1);
        y = RAND.randint(0, Y_DIM - 1);
    }
    controller.display()->set_pixel_color(x, y, wheel_rgb(RAND.random(255)));
}

void setup() {
    trellis_hw_interface->begin();

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

    // trellis_hw_interface->begin();
    for (int i = 0; i < Y_DIM * X_DIM; i++) {
        int x = i % X_DIM;
        int y = i / X_DIM;
        trellis_controller.display()->set_pixel_color(x, y, wheel_rgb(map(i, 0, X_DIM * Y_DIM, 0, 255))); // addressed
                                                                                                          // with keynum
        trellis_controller.display()->show();
        delay(20);
    }

    for (int y = 0; y < Y_DIM; y++) {
        for (int x = 0; x < X_DIM; x++) {
            // activate rising and falling edges on all keys
            //  trellis_hw_interface->activateKey(x, y, SEESAW_KEYPAD_EDGE_RISING, true);
            //  trellis_hw_interface->activateKey(x, y, SEESAW_KEYPAD_EDGE_FALLING, true);
            //  trellis_hw_interface->registerCallback(x, y, blink);
            trellis_controller.display()->set_pixel_off(x, y); // addressed with x,y
            trellis_controller.display()->show();              // show all LEDs
            delay(20);
            // break;
        }
    }
    trellis_controller.set_on_any_key_pressed_callback([&](int x, int y, const Time&) {
        // Penalize pressing keys that aren't lit up
        if (!trellis_controller.display()->is_pixel_on(x, y)) {
            turn_on_random_pixel(trellis_controller);
        }
        trellis_controller.display()->set_pixel_off(x, y);
        trellis_controller.display()->show();
        // TrellisHWInterface::get_instance()->clear_pixel(x, y);
        // TrellisHWInterface::get_instance()->show();
    });
    // TrellisHWInterface::get_instance()->register_on_any_key_released_callback([](int x, int y, const Time&) {
    //     TrellisHWInterface::get_instance()->clear_pixel(x, y);
    //     TrellisHWInterface::get_instance()->show();
    // });

    trellis_controller.add_timer_callback(1000, [&](const Time& now) -> tl::optional<Duration> {
        static uint16_t timer_period_ms = 1000;
        const uint16_t MIN_PERIOD = 150;

        static bool is_game_over = false;

        if (is_game_over) {
            // Blink red
            if (trellis_controller.display()->are_all_pixels_on()) {
                trellis_controller.display()->clear();
            }
            else {
                trellis_controller.display()->fill(RGBA{255, 0, 0});
            }
            return tl::nullopt;
        }

        // Check for GAME OVER
        if (trellis_controller.display()->are_all_pixels_on()) {
            is_game_over = true;
            trellis_controller.display()->fill(RGBA{255, 0, 0});
            trellis_controller.display()->show();
            TrellisHWInterface::get_instance()->set_timer_period(500);
            return 500;
        }

        turn_on_random_pixel(trellis_controller);
        trellis_controller.display()->show();
        // Serial.printf("Timer event @ %.3f s\n", static_cast<double>(now) / 1000);
        if (timer_period_ms > MIN_PERIOD) {
            timer_period_ms -= 5;
        }

        return timer_period_ms;
    });
}

void loop() {
    TrellisHWInterface::get_instance()->tick();
}
