#pragma once
#include "common.h"

class Texture {
public:
    explicit Texture(Image image)
        : image_(std::move(image)) {
    }

    Size GetSize() const {
        const auto width = static_cast<int>(image_[0].size());
        const auto height = static_cast<int>(image_.size());
        return { width, height };
    }

    char GetPixelColor(Point p) const {
        return image_.at(p.x).at(p.y);
    }

private:
    Image image_;
};