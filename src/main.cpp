/**
* NOTE: had to comment out abs and round macros from Arduino.h to get to compile...
*/
#include <Arduino.h>
/* This example shows basic usage of the
MultiTrellis object controlling an array of
NeoTrellis boards

As is this example shows use of two NeoTrellis boards
connected together with the leftmost board having the
default I2C address of 0x2E, and the rightmost board
having the address of 0x2F (the A0 jumper is soldered)
*/

#include <apps/application_picker.h>
#include <apps/battery.h>
#include <apps/lights_out.h>
#include <apps/tic_tac_toe.h>

#include <Prandom.h>

#include "Adafruit_NeoTrellis.h"
#include "apps/application_ids.h"
#include "apps/coloring_pad.h"
#include "apps/random_color_toggle.h"
#include "apps/splash_screen.h"
#include "tboard_games.h"
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

constexpr int BRIGHTNESS = 20;

// create a matrix of trellis panels
Adafruit_NeoTrellis t_array[Y_DIM / 4][X_DIM / 4] = {

    {Adafruit_NeoTrellis(0x2F), Adafruit_NeoTrellis(0x2E)},

    {Adafruit_NeoTrellis(0x31), Adafruit_NeoTrellis(0x30)}
};

// pass this matrix to the multitrellis object
Adafruit_MultiTrellis trellis((Adafruit_NeoTrellis*)t_array, Y_DIM / 4, X_DIM / 4);

static TrellisHWInterfacePtr trellis_hw_interface = TrellisHWInterface::get_instance();
static tboard::TrellisController trellis_controller(trellis_hw_interface);
static tboard::TBoardOS tboard_os(trellis_hw_interface);

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
    // Exit early if all pixels are already on
    if (trellis_controller.display()->are_all_pixels_on()) {
        return;
    }

    const int max_attempts = 100;
    int x = RAND.randint(0, X_DIM - 1);
    int y = RAND.randint(0, Y_DIM - 1);
    int attempt_cnt = 0;
    while (attempt_cnt < max_attempts) {
        if (!controller.display()->is_pixel_on(x, y)) {
            break;
        }
        // Try again
        x = RAND.randint(0, X_DIM - 1);
        y = RAND.randint(0, Y_DIM - 1);
        attempt_cnt++;
    }
    controller.display()->set_pixel_color(x, y, wheel_rgb(RAND.random(255), BRIGHTNESS));
}

void setup() {
    trellis_hw_interface->begin();


    tboard_os.register_app<tboard::apps::SplashScreen>();
    tboard_os.register_app<tboard::apps::ApplicationPicker>();
    tboard_os.register_app<tboard::apps::RandomColorToggle>();
    tboard_os.register_app<tboard::apps::LightsOut>();
    tboard_os.register_app<tboard::apps::Battery>();
    tboard_os.register_app<tboard::apps::ColoringPad>();
    tboard_os.register_app<tboard::apps::TicTacToe>();
    tboard_os.set_startup_app(tboard::apps::SPLASHSCREEN_ID);
    tboard_os.start();
}

void loop() {
    TrellisHWInterface::get_instance()->tick();
}
