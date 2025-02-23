#pragma once
#include "System/Math/Vector4.h"

class ColorEditor{
	float r_;
    float g_;
    float b_;
    float a_;

public:
	ColorEditor(Vector4);

    Vector4 GetColor() const;


    ColorEditor& AddRed(float r);
    ColorEditor& AddGreen(float g);
    ColorEditor& AddBlue(float b);
    ColorEditor& AddAlpha(float a);

    ColorEditor& SubRed(float r);
    ColorEditor& SubGreen(float g);
    ColorEditor& SubBlue(float b);
    ColorEditor& SubAlpha(float a);





};

