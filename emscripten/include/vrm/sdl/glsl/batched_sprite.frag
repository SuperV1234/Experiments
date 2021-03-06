#ifdef GL_ES
    precision mediump float;
#endif

uniform sampler2D u_texture;

varying vec4 v_color;
varying vec2 v_tex_coords;
varying float v_hue;


const mat3 rgb2yiq = mat3(          // .
    0.299, 0.587, 0.114,            // .
    0.595716, -0.274453, -0.321263, // .
    0.211456, -0.522591, 0.311135   // .
    );

const mat3 yiq2rgb = mat3( // .
    1.0, 0.9563, 0.6210,   // .
    1.0, -0.2721, -0.6474, // .
    1.0, -1.1070, 1.7046   // .
    );

void main()
{
    vec4 tx_color = texture2D(u_texture, v_tex_coords);

    vec3 yColor = rgb2yiq * (v_color.rgb * tx_color.rgb);

    float originalHue = atan(yColor.b, yColor.g);

    float finalHue = originalHue + v_hue;

    float chroma = sqrt(yColor.b * yColor.b + yColor.g * yColor.g);

    vec3 yFinalColor =
        vec3(yColor.r, chroma * cos(finalHue), chroma * sin(finalHue));

    gl_FragColor = vec4(yiq2rgb * yFinalColor, v_color.w * tx_color.w);
}
