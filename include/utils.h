//
// Copyright © 2024 Glydways, Inc.
// https://glydways.com/
//

#pragma once

#include "optional.h"

class LowPassFilter {
public:
        LowPassFilter(float alpha) : alpha_(alpha) { }

        float filter(float input) {
            if (!output_) {
                output_ = input;
                return input;
            }
            output_ = alpha_ * input + (1 - alpha_) * output_.value();
            return output_.value();
        }

    private:
        float alpha_;
        tl::optional<float> output_;
    };