//
// Copyright © 2024 Glydways, Inc.
// https://glydways.com/
//

#ifndef TBOARD_APPLICATION_IDS_H
#define TBOARD_APPLICATION_IDS_H

#include "os/application.h"

namespace tboard::apps {

static const ApplicationId SPLASHSCREEN_ID{
    .id = 0,
    .name = "Splash Screen"
};

static const ApplicationId COLORING_PAD_ID{
    .id = 1,
    .name = "Coloring Pad"
};

static const ApplicationId RANDOM_COLOR_TOGGLE_ID{
    .id = 2,
    .name = "Random Color Toggle"
};
} // namespace tboard::apps
#endif // TBOARD_APPLICATION_IDS_H