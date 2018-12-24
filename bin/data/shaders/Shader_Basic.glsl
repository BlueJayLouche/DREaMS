#define PI 3.14159

void mainImage(out vec4 fragColor, in vec2 fragCoord)
{
    vec2 xy = fragCoord.xy / iResolution.xy;
    float b = texture(iChannel0, xy * -0.5).r;
    float s = pow(texture(iChannel0, xy * 0.5).r, 0.7);
    vec4 v = texture(iChannel1, xy);
    fragColor = v*.4 * (5.*vec4(step(0.76, s),b,sin(1.3*iTime),1.));
}
