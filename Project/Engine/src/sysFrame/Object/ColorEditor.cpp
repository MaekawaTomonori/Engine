#include "ColorEditor.h"

ColorEditor::ColorEditor(Vector4 v) {
    r_ = v.x;
    g_ = v.y;
    b_ = v.z;
    a_ = v.w;
}

Vector4 ColorEditor::GetColor() const {
    return {r_, g_, b_, a_};
}

ColorEditor& ColorEditor::AddRed(float r = 1) {
    if (1 < r){
        r /= 255;
    }

    r_ += r;
    return *this;
}

ColorEditor& ColorEditor::AddGreen(float g = 1) {
    if (1 < g){
        g /= 255;
    }
	g_ += g;
    return *this;
}

ColorEditor& ColorEditor::AddBlue(float b = 1) {
    if (1 < b){
        b /= 255;
    }
    b_ += b;
    return *this;
}

ColorEditor& ColorEditor::AddAlpha(float a = 1) {
    if (1 < a){
        a /= 255;
    }
	a_ += a;
    return *this;
}

ColorEditor& ColorEditor::SubRed(float r = 1) {
    if (1 < r){
        r /= 255;
    }
    r_ -= r;
    return *this;
}

ColorEditor& ColorEditor::SubGreen(float g = 1) {
    if (1 < g){
        g /= 255;
    }
    g_ -= g;
    return *this;
}

ColorEditor& ColorEditor::SubBlue(float b = 1) {
    if (1 < b){
        b /= 255;
    }
    b_ -= b;
    return *this;
}

ColorEditor& ColorEditor::SubAlpha(float a = 1) {
    if (1 < a){
        a /= 255;
    }
    a_ -= a;
    return *this;
}
