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
    if (colorRGB.r >= colorRGB.g && colorRGB.r >= colorRGB.b) {
        float v = colorRGB.r;
        float minC = 0.0;
        if (colorRGB.g >= colorRGB.b) {
            minC = colorRGB.b;
        } else {
            minC = colorRGB.g;
        }
        float s = 0.0;
        if (v > 0.0) {
            s = 1.0 - minC/v;
        } else {
            s = 0.0;
        }
        float h = 60.0*(colorRGB.g - colorRGB.b)/(v-minC);
        if (h<0.0) {
            h = h+360.0;
        }
        return vec4(h,s,v,colorRGB.a);
    }
    else if (colorRGB.g >= colorRGB.r && colorRGB.g >=colorRGB.b) {
        float v = colorRGB.g;
        float minC = 0.0;
        if (colorRGB.r >= colorRGB.b) {
            minC = colorRGB.b;
        } else {
            minC = colorRGB.r;
        }
        float s = 0.0;
        if (v > 0.0) {
            s = 1.0 - minC/v;
        } else {
            s = 0.0;
        }
        float h = 120.0+60.0*(colorRGB.b - colorRGB.r)/(v-minC);
        if (h<0.0) {
            h = h+360.0;
        }
        return vec4(h,s,v,colorRGB.a);
    }
    else if (colorRGB.b >= colorRGB.r && colorRGB.b >=colorRGB.g) {
        float v = colorRGB.b;
        float minC = 0.0;
        if (colorRGB.r >= colorRGB.g) {
            minC = colorRGB.g;
        } else {
            minC = colorRGB.r;
        }
        float s = 0.0;
        if (v > 0.0) {
            s = 1.0 - minC/v;
        } else {
            s = 0.0;
        }
        float h = 240.0+60.0*(colorRGB.r - colorRGB.g)/(v-minC);
        if (h < 0.0) {
           h = h+360.0;
        }
        return vec4(h,s,v,colorRGB.a);
    }
}

int PREFIX(getColorIndex)(vec4 colorHSV)
{
    int index = 0;
    float h = colorHSV.r;
    float s = colorHSV.g;
    float v = colorHSV.b;

    if (v <= PREFIX(blackValueMax)) {
        index = 0;
    } else if (s <= PREFIX(graySaturationMax)) {
        if (v <= PREFIX(grayValueMax)) {
            index = 1;
        } else {
            index = 2;
        }
    } else {
        if (PREFIX(redHueMin) <= h || h <= PREFIX(redHueMax)) {
            index = 3;
        } else if (PREFIX(orangeHueMin) <= h && h <= PREFIX(orangeHueMax)) {
            index = 4;
        } else if (PREFIX(yellowHueMin) <= h && h <= PREFIX(yellowHueMax)) {
            index = 5;
        } else if (PREFIX(greenHueMin) <= h && h <= PREFIX(greenHueMax)) {
            index = 6;
        } else if (PREFIX(cyanHueMin) <= h && h <= PREFIX(cyanHueMax)) {
            index = 7;
        } else if (PREFIX(blueHueMin) <= h && h <= PREFIX(blueHueMax)) {
            index = 8;
        } else if (PREFIX(purpleHueMin) <= h && h <= PREFIX(purpleHueMax)) {
            index = 9;
        } else if (PREFIX(pinkHueMin) <= h && h <= PREFIX(pinkHueMax)) {
            index = 10;
        }
    }

    return index;
}

int PREFIX(getBackGroundColorIndex)()
{
    int colorDistribution[11];
    colorDistribution[0] = 0;
    colorDistribution[1] = 0;
    colorDistribution[2] = 0;
    colorDistribution[3] = 0;
    colorDistribution[4] = 0;
    colorDistribution[5] = 0;
    colorDistribution[6] = 0;
    colorDistribution[7] = 0;
    colorDistribution[8] = 0;
    colorDistribution[9] = 0;
    colorDistribution[10] = 0;
    int halfnum = 20;

    float j = 0.0;
    for (int i=0; i<10; i++) {
        float tx = 0.01;
        float ty = 0.99-0.015*j;
        vec4 colorLeft = INPUT(vec2(tx,ty));
        vec4 colorLeftHSV = PREFIX(rgb2hsv)(colorLeft);
        int index = PREFIX(getColorIndex)(colorLeftHSV);
        colorDistribution[index]++;
        if (colorDistribution[index] >= halfnum) {
            return index;
        }
        j = j + 1.0;
    }

    j = 0.0;
    for (int i=0; i<20; i++) {
        float tx = 0.01+0.04*j;
        float ty = 0.99;
        vec4 colorTop = INPUT(vec2(tx,ty));
        vec4 colorTopHSV = PREFIX(rgb2hsv)(colorTop);
        int index = PREFIX(getColorIndex)(colorTopHSV);
        colorDistribution[index]++;
        if (colorDistribution[index] >= halfnum) {
            return index;
        }
        j = j + 1.0;
    }

    j = 0.0;
    for (int i=0; i<10; i++) {
        float tx = 0.99;
        float ty = 0.99-0.015*j;
        vec4 colorRight = INPUT(vec2(tx,ty));
        vec4 colorRightHSV = PREFIX(rgb2hsv)(colorRight);
        int index = PREFIX(getColorIndex)(colorRightHSV);
        colorDistribution[index]++;
        if (colorDistribution[index] >= halfnum) {
            return index;
        }
        j = j + 1.0;
    }
    return -1;
}

bool PREFIX(isBackGroundColorIndex)(int index, vec4 color)
{
    vec4 colorHSV = PREFIX(rgb2hsv)(color);
    int colorIndex = PREFIX(getColorIndex)(colorHSV);
    if (colorIndex == index) {
        return true;
    } else {
        return false;
    }
}

vec4 FUNCNAME(vec2 tc)
{
    vec4 color = INPUT(tc);
    int colorBGIndex = 6;//PREFIX(getBackGroundColorIndex)();
    bool isBG = PREFIX(isBackGroundColorIndex)(colorBGIndex, color);
    vec4 x;
    if (isBG) {
        //gl_FragColor = vec4(color.r, color.g, color.b, 0.0);
        x = vec4(1.0, 1.0, 0.0, 1.0);
    } else {
        //gl_FragColor = vec4(0.0, 0.0, 1.0, 1.0);
        x = color;
    }

    //gl_FragColor = vec4(0.0, 0.0, 1.0, 1.0);
    return x;
}
