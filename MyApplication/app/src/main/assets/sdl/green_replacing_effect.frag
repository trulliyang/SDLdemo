
vec4 FUNCNAME(vec2 tc) {
	vec4 color = INPUT(tc);
        vec4 x = color;

        // test the color value
        float dtValue = 0.004;

        float alpha = 1.0;
        float th = 0.2;//waterfall sccj01
//        float th = 1.0;//zombia sccj02

        if (color.g>(color.r+0.03125*th) && color.g>(color.b+0.03125*th)) {
            vec4 tColor = vec4(PREFIX(targetColorRed), PREFIX(targetColorGreen), PREFIX(targetColorBlue), PREFIX(targetColorAlpha));

            // calculate diff
            float redMean = (tColor.r + color.r)*0.5;

            float deltaR = tColor.r - color.r;
            float deltaG = tColor.g - color.g;
            float deltaB = tColor.b - color.b;

            float partR = (2.0 + redMean)*deltaR*deltaR;
            float partG = 4.0*deltaG*deltaG;
            float partB = (3.0 - redMean)*deltaB*deltaB;

            float delta = sqrt(partR + partG + partB)*0.111111;

    //        if (delta < PREFIX(diffMin)) {
    //            alpha = 0.0;
    //        } else if (delta < PREFIX(diffMax)) {
    //            alpha = 0.5;
    //        }

            if (delta <= PREFIX(diff)*1.0) {
                alpha = 0.0;
                x = vec4(0.0, 0.0, 0.0, 1.0);
            } else {
                alpha = 0.5;

                float max = 0.0;
                if (color.r > max) {
                    max = color.r;
                }
                if (color.g > max) {
                    max = color.g;
                }
                if (color.b > max) {
                    max = color.b;
                }
                color = vec4(max, max, max, 1.0-max);
//                color = vec4(max, max, max, 0.0);

            }

        }

    return color;

}
