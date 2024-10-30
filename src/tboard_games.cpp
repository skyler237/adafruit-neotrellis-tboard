//
// Copyright © 2024 Glydways, Inc.
// https://glydways.com/
//

// #include "includes.h"
#include "tboard_games.h"


#include "apps/application_ids.h"
#include "apps/splash_screen.h"
#include "apps/coloring_pad.h"
#include "apps/random_color_toggle.h"

void tboard::run_games(TrellisInterfacePtr trellis_interface) {
    TBoardOS tboard(std::move(trellis_interface));
    tboard.register_app<apps::SplashScreen>();
    tboard.register_app<apps::RandomColorToggle>();
    tboard.register_app<apps::ColoringPad>();
    tboard.set_startup_app(apps::COLORING_PAD_ID);
    tboard.run();
    // QApplication::exec();
}