// Iain Melvin 2014

//#define RADIAL
#define REFLECT

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    vec2 uv = fragCoord.xy / iResolution.xy;
    float a = 1.0;
#ifdef REFLECT
    uv=abs(2.0*(uv-0.5));
#endif
    
#ifdef RADIAL
    float theta = 1.0*(1.0/(3.14159/2.0))*atan(uv.x,uv.y);
    float r = length(uv);
    a=1.0-r;//vignette
    uv = vec2(theta,r);
#endif
    
    vec4 t1 = texture(iChannel0, vec2(uv[0],0.1) );
    vec4 t2 = texture(iChannel0, vec2(uv[1],0.1) );
    float fft = t1[0]*t2[0]*a;
    fragColor = vec4( sin(fft*3.141*2.5), sin(fft*3.141*2.0),sin(fft*3.141*1.0),1.0);
}
