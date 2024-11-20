
#pragma once

#include <queue>
#include <utility>

#include "trellis_display.h"

namespace tboard {

struct Frame {
    Time start_time;
    Duration display_duration;
    std::function<void(const TrellisDisplayPtr&)> draw_frame_callback;
};

class Animation {
public:
    virtual ~Animation() = default;

    explicit Animation(TrellisDisplayPtr trellis_display);

    bool maybe_draw_next_frame(const Time& now);
    bool tick(const Time& now);

    virtual tl::optional<Frame> get_next_frame() = 0;

private:
    TrellisDisplayPtr trellis_display_;
    tl::optional<Frame> current_frame_;
};

class MultiFrameAnimation : public Animation {
public:
    explicit MultiFrameAnimation(TrellisDisplayPtr trellis_display,
                                 bool loop = false,
                                 std::vector<Frame> frames = {});

    tl::optional<Frame> get_next_frame() override;

    void add_frame(Frame frame);

private:
    const bool loop_;
    std::vector<Frame> frames_;
    int current_frame_index_{-1};
};

} // namespace tboard
