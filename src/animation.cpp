//
// Copyright © 2024 Glydways, Inc.
// https://glydways.com/
//

#include "animation.h"

namespace tboard {
Animation::Animation(TrellisDisplayPtr trellis_display) : trellis_display_(std::move(trellis_display)) { }

bool Animation::maybe_draw_next_frame(const Time& now) {
    current_frame_ = get_next_frame();
    current_frame_->start_time = now;
    if (!current_frame_.has_value()) {
        return false;
    }
    current_frame_->draw_frame_callback(trellis_display_);
    trellis_display_->show();
    return true;
}

bool Animation::tick(const Time& now) {
    if (!current_frame_.has_value() || now - current_frame_->start_time >= current_frame_->display_duration) {
        return maybe_draw_next_frame(now);
    }
    // Animation not finished, return true
    return true;
}

MultiFrameAnimation::MultiFrameAnimation(TrellisDisplayPtr trellis_display, const bool loop, std::vector<Frame> frames)
    : Animation(std::move(trellis_display)), loop_(loop), frames_(std::move(frames)) { }

tl::optional<Frame> MultiFrameAnimation::get_next_frame() {
    if (!loop_ && current_frame_index_ >= frames_.size()) {
        return tl::nullopt;
    }
    // Increment the frame index
    current_frame_index_++;
    if (loop_ && current_frame_index_ >= frames_.size()) {
        current_frame_index_ = 0;
    }
    const Frame next_frame = frames_[current_frame_index_];
    return next_frame;
}

void MultiFrameAnimation::add_frame(Frame frame) {
    frames_.emplace_back(std::move(frame));
}
} // namespace tboard