// It's actually (but surprisingly) not correct to do a mix() here;
// it would be if we had postmultiplied alpha and didn't have to worry
// about alpha in the bottom layer, but given that we use premultiplied
// alpha all over, top shouldn't actually be multiplied by anything.
//
// These formulas come from Wikipedia:
//
//   http://en.wikipedia.org/wiki/Alpha_compositing
//
// We use the associative version given. However, note that since we want
// _output_ to be premultiplied, C_o from Wikipedia is not what we want,
// but rather c_o (which is not explicitly given, but obviously is just
// C_o without the division by alpha_o).


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
        if (h<0.0) {
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
        if (h<0.0) {
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

bool PREFIX(isGreenColor)(vec4 colorHSV)
{
//    int index = -1;
//    float h = colorHSV.r;
//    float s = colorHSV.g;
//    float v = colorHSV.b;
//
////    if (v >= PREFIX(blackValueMin)) {
//    if (v <= PREFIX(blackValueMax)/2.5) {
//        index = 0;
//    } else if (s <= PREFIX(graySaturationMax)) {
//        if (v <= PREFIX(grayValueMax)) {
//            index = 1;
//        } else {
//            index = 2;
//        }
//    } else {
//        if (PREFIX(redHueMin) <= h || h <= PREFIX(redHueMax)) {
//            index = 3;
//        } else if (PREFIX(orangeHueMin) <= h && h <= PREFIX(orangeHueMax)) {
//            index = 4;
//        } else if (PREFIX(yellowHueMin) <= h && h <= PREFIX(yellowHueMax)) {
//            index = 5;
//        } else if (PREFIX(greenHueMin) <= h && h <= PREFIX(greenHueMax)) {
//            index = 6;
//        } else if (PREFIX(cyanHueMin) <= h && h <= PREFIX(cyanHueMax)) {
//            index = 7;
//        } else if (PREFIX(blueHueMin) <= h && h <= PREFIX(blueHueMax)) {
//            index = 8;
//        } else if (PREFIX(purpleHueMin) <= h && h <= PREFIX(purpleHueMax)) {
//            index = 9;
//        } else if (PREFIX(pinkHueMin) <= h && h <= PREFIX(pinkHueMax)) {
//            index = 10;
//        }
//    }

    bool isGreen = false;
    float h = colorHSV.r;
    float s = colorHSV.g;
    float v = colorHSV.b;

    if (v > 0.1) {
        if (s > 0.3) {
            if (80.0 <= h && h <= 180.5) {
                isGreen = true;
            }
        }
    }

    return isGreen;
}

vec4 PREFIX(replaceColor)(vec4 colorRGBA)
{
    vec4 colorHSV = PREFIX(rgb2hsv)(colorRGBA);
    bool isGreen = PREFIX(isGreenColor)(colorHSV);
    if (isGreen) {
        colorRGBA.a = 0.0;
    }
    return colorRGBA;
}

vec4 FUNCNAME(vec2 tc) {
// SWAP_INPUTS will be #defined to 1 if we want to swap the two inputs,
#if SWAP_INPUTS
	vec4 bottom = INPUT2(tc);
	vec4 top = INPUT1(tc);
#else
	vec4 bottom = INPUT1(tc);
	vec4 top = INPUT2(tc);
#endif

    vec4 topNew = PREFIX(replaceColor)(top);

	return topNew + (1.0 - topNew.a) * bottom;
}

#undef SWAP_INPUTS
