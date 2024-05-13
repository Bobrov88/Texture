#pragma once
#include "texture.h"
#include <memory>
#include <iostream>

// Поддерживаемые виды фигур: прямоугольник и эллипс
enum class ShapeType { RECTANGLE, ELLIPSE };

class Shape {
public:
    // Фигура после создания имеет нулевые координаты и размер,
    // а также не имеет текстуры
    explicit Shape(ShapeType type) {
        shapeType_ = std::move(type);
    }

    void SetPosition(Point pos) {
        pos_ = std::move(pos);
    }

    void SetSize(Size size) {
        size_ = std::move(size);
    }

    void SetTexture(std::shared_ptr<Texture> texture) {
        texture_ = std::move(texture);
    }

    // Рисует фигуру на указанном изображении
    // В зависимости от типа фигуры должен рисоваться либо эллипс, либо прямоугольник
    // Пиксели фигуры, выходящие за пределы текстуры, а также в случае, когда текстура не задана,
    // должны отображаться с помощью символа точка '.'
    // Части фигуры, выходящие за границы объекта image, должны отбрасываться.
    void Draw(Image& image) const {
        const auto shift_height = std::min(size_.height + pos_.y, static_cast<int>(image.size()));
        const auto shift_width = std::min(size_.width + pos_.x, static_cast<int>(image[0].size()));
        if (!texture_) {
            for (int i = pos_.y; i < shift_height; ++i) {
                for (int k = pos_.x; k < shift_width; ++k) {
                    image[i][k] = '.';
                }
            }
            return;
        }
        auto checkPointIn = (shapeType_ == ShapeType::RECTANGLE) ? IsPointInRectangle : IsPointInEllipse;
        for (int k = pos_.x; k < shift_width; k++) {
            for (int i = pos_.y; i < shift_height; i++) {
                bool inFig = checkPointIn(std::move(Point{ i - pos_.y, k - pos_.x }), std::move(Size{ size_.height, size_.width}));
                bool inText = IsPointInRectangle(std::move(Point{ i - pos_.y, k - pos_.x }), std::move(Size{ texture_->GetSize().height, texture_->GetSize().width }));
                if (inFig) {
                    if (inText) image[i][k] = texture_->GetPixelColor(std::move(Point{ i - pos_.y, k - pos_.x }));
                    else image[i][k] = '.';
                }
            }
        }
    }
    
private:
    ShapeType shapeType_;
    Point pos_;
    Size size_;
    std::shared_ptr<Texture> texture_;
};