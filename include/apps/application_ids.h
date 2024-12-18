//
// Copyright © 2024 Glydways, Inc.
// https://glydways.com/
//

#ifndef TBOARD_APPLICATION_IDS_H
#define TBOARD_APPLICATION_IDS_H

#include <vector>

#include "os/application.h"

namespace tboard::apps {

static const ApplicationId SPLASHSCREEN_ID{.id = 0, .name = "Splash Screen"};

static const ApplicationId COLORING_PAD_ID{.id = 1, .name = "Coloring Pad"};

static const ApplicationId RANDOM_COLOR_TOGGLE_ID{.id = 2, .name = "Random Color Toggle"};

static const ApplicationId APPLICATION_PICKER_ID{.id = 3, .name = "Application Picker"};

static const ApplicationId LIGHTS_OUT_ID{.id = 4, .name = "Lights Out"};

static const ApplicationId BATTERY_ID{.id = 5, .name = "Battery"};

static const ApplicationId TIC_TAC_TOE_ID{.id = 6, .name = "Tic-Tac-Toe"};

} // namespace tboard::apps
#endif // TBOARD_APPLICATION_IDS_H