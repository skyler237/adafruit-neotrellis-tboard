# Third Party Edits
* Arduino.h
  * Commented out some of the `abs` and `round` macros that were causing issues
* NColor `ColorConverters.cpp`
  * Fixed `convertToRGBA`
    * Handle the hue offsets properly (+120 for green and +240 for blue)
    * Handle saturation properly (x100 to get proper range)

# TODO
- [] Refactor interface callbacks to allow for multiple callbacks (use vectors)
- [] Add a way to keep track of button press duration
- [] Finish implementing the button held callback
- 