// use newColor to replace targetColor in originColor

//uniform float blackValueMin;
//uniform float blackValueMax;
//
//uniform float graySaturationMin;
//uniform float graySaturationMax;
//uniform float grayValueMin;
//uniform float grayValueMax;
//
//uniform float whiteSaturationMin;
//uniform float whiteSaturationMax;
//uniform float whiteValueMin;
//uniform float whiteValueMax;
//
//uniform float redHueMin;
//uniform float redHueMax;
//
//uniform float orangeHueMin;
//uniform float orangeHueMax;
//
//uniform float yellowHueMin;
//uniform float yellowHueMax;
//
//uniform float greenHueMin;
//uniform float greenHueMax;
//
//uniform float cyanHueMin;
//uniform float cyanHueMax;
//
//uniform float blueHueMin;
//uniform float blueHueMax;
//
//uniform float purpleHueMin;
//uniform float purpleHueMax;
//
//uniform float pinkHueMin;
//uniform float pinkHueMax;

//bool isTest()
//{
//    if (v_texcoord0.y > v_texcoord0.x) {
//        return true;
//    }
//    return false;
//}

vec4 PREFIX(rgb2hsv)(vec4 colorRGB)
{
    float h,s,v;
    if (colorRGB.r >= colorRGB.g && colorRGB.r >= colorRGB.b) {
        v = colorRGB.r;
        float minC = 0.0;
        if (colorRGB.g >= colorRGB.b) {
            minC = colorRGB.b;
        } else {
            minC = colorRGB.g;
        }
        s = 0.0;
        if (v > 0.0) {
            s = 1.0 - minC/v;
        } else {
            s = 0.0;
        }
        h = 60.0*(colorRGB.g - colorRGB.b)/(v-minC);
        if (h < 0.0) {
            h = h+360.0;
        }
        return vec4(h,s,v,colorRGB.a);
    }
    else if (colorRGB.g >= colorRGB.r && colorRGB.g >= colorRGB.b) {
        v = colorRGB.g;
        float minC = 0.0;
        if (colorRGB.r >= colorRGB.b) {
            minC = colorRGB.b;
        } else {
            minC = colorRGB.r;
        }
        s = 0.0;
        if (v > 0.0) {
            s = 1.0 - minC/v;
        } else {
            s = 0.0;
        }
        h = 120.0+60.0*(colorRGB.b - colorRGB.r)/(v-minC);
        if (h < 0.0) {
            h = h+360.0;
        }
        return vec4(h,s,v,colorRGB.a);
    }
    else if (colorRGB.b >= colorRGB.r && colorRGB.b >= colorRGB.g) {
        v = colorRGB.b;
        float minC = 0.0;
        if (colorRGB.r >= colorRGB.g) {
            minC = colorRGB.g;
        } else {
            minC = colorRGB.r;
        }
        s = 0.0;
        if (v > 0.0) {
            s = 1.0 - minC/v;
        } else {
            s = 0.0;
        }
        h = 240.0+60.0*(colorRGB.r - colorRGB.g)/(v-minC);
        if (h < 0.0) {
           h = h+360.0;
        }
        return vec4(h,s,v,colorRGB.a);
    }
    else {
        return vec4(1.0, 1.0, 1.0, 1.0);
    }
}


// h greenHueMin~greenHueMax->0.0~360.0
// s graySaturationMax~1.0->0.0~1.0
// v blackValueMax~1.0->0.0~1.0

bool PREFIX(isGreen)(vec4 colorHSV)
{
    float h = colorHSV.r;
    float s = colorHSV.g;
    float v = colorHSV.b;

    float alpha = PREFIX(threshold);

    float vMin = (1.0 - alpha)*vMin0 + alpha*vMin1;
    float vMax = (1.0 - alpha)*vMax0 + alpha*vMax1;
    if (v < vMin || v > vMax) {
        return false;
    }

    float sMin = (1.0 - alpha)*sMin0 + alpha*sMin1;
    float sMax = (1.0 - alpha)*sMax0 + alpha*sMax1;
    if (s < sMin || s > sMax) {
        return false;
    }

    float hMin = (1.0 - alpha)*hMin0 + alpha*hMin1;
    float hMax = (1.0 - alpha)*hMax0 + alpha*hMax1;
    if (h < hMin || h > hMax) {
        return false;
    }

    return true;
}

vec4 FUNCNAME(vec2 tc)
{
    vec4 color = INPUT(tc);
    int colorBGIndex = 6;//PREFIX(getBackGroundColorIndex)();
    vec4 colorHSV = PREFIX(rgb2hsv)(color);
    bool isGreen = PREFIX(isGreen)(colorHSV);
    vec4 x;
    if (isGreen) {
        color.a = 0.5;
        color = vec4(1.0, 1.0, 1.0, 1.0);
    }

    return color;
}
