#version 120
uniform sampler2DRect colorMap;
uniform sampler2DRect font;

// 0.5 = same width as original pixel	1.0-1.2 seems nice
#define SPOT_WIDTH	1.2
// Shape of the spots	1.0 = circle, 4.0 = ellipse with width = 2*height  ************/
#define X_SIZE_ADJUST	2.0
/******************************** To increase bloom / luminosity play with this parameter ************/
#define FACTOR_ADJUST 2.5
#define SCALE	21.0

vec2 sourceSize = vec2(640,400);

// Constants
vec4 luminosity_weights = vec4( 0.2126, 0.7152, 0.0722, 0.0 );		//  Y = 0.2126 R + 0.7152 G + 0.0722 B

vec2 onex = vec2( 1.0 / sourceSize.x, 0.0 );
vec2 oney = vec2( 0.0, 1.0 / sourceSize.y);

float factor( float lumi, vec2 dxy)
{
    float dist = sqrt( dxy.x * dxy.x + dxy.y * dxy.y * X_SIZE_ADJUST  ) / SCALE;
    return (2.0 + lumi ) * (1.0 - smoothstep( 0.0, SPOT_WIDTH, dist ) ) / FACTOR_ADJUST ;
}

int modi(int a, int b) { return a - ((a / b) * b); }

vec4 getColor(vec2 nuv) {
    
    vec2 uv = nuv * vec2(80,50);

    vec4 clr = texture2DRect(colorMap, uv);
    
    int o = int(clr.b * 256.0);
    int sx =  (modi(o,16) * 8) + int(mod(uv.x * 8.0f, 8.0f));
    int sy =  (int(o/16) * 8) + int(mod(uv.y * 8.0f, 8.0f));
    vec2 fontuv = vec2(sx,sy);

    float cv = texture2DRect(font, fontuv).r;
    
    int fc = int(clr.r * 256.0f);
    
    float fr = float(modi(fc / 36,6)) / 5.0f;
    float fg = float(modi(fc / 6, 6)) / 5.0f;
    float fb = float(modi(fc, 6)) / 5.0f;
    
    vec4 foreground = vec4(fr,fg,fb,1.0f);
    int bc = int(clr.g * 256.0f);
    
    float br = float(modi(bc/36,6)) / 5.0f;
    float bg = float(modi(bc/6,6)) / 5.0f;
    float bb = float(modi(bc,6)) / 5.0f;
    
    vec4 background = vec4(br,bg,bb,1.0f);
    
    vec4 final = vec4(1,1,1,1);
    
    final.rgb = mix(background.rgb, foreground.rgb, cv);
    
    return final;
}

void main(){

    vec2 uv = gl_TexCoord[0].st / vec2(80,50);

    
    vec2 coords_scaled = floor( uv * sourceSize * SCALE );
    vec2 coords_snes = floor( coords_scaled / SCALE );
    vec2 coords_texture = ( coords_snes + vec2(0.5) ) / sourceSize;
    
    vec2 ecart = coords_scaled - ( SCALE * coords_snes + vec2( SCALE * 0.5 - 0.5 ) ) ;
    
    vec4 color = getColor(coords_texture);
    
    float luminosity = dot( color, luminosity_weights );
    
    color *= factor( luminosity, ecart );
    
    // RIGHT NEIGHBOUR
    vec4 pcol = getColor(coords_texture + onex);
    luminosity = dot( pcol, luminosity_weights );
    color += pcol * factor( luminosity, ecart + vec2( -SCALE , 0.0) );
    
    // LEFT NEIGHBOUR
    pcol = getColor(coords_texture - onex);
    luminosity = dot( pcol, luminosity_weights );
    color += pcol * factor( luminosity, ecart + vec2( SCALE , 0.0) );
    
    /*
    
    // TOP
    pcol = getColor( coords_texture + oney);
    luminosity = dot( pcol, luminosity_weights );
    color += pcol * factor( luminosity, ecart + vec2( 0.0, -SCALE) );
    
    // TOP-LEFT
    pcol = getColor( coords_texture + oney - onex);
    luminosity = dot( pcol, luminosity_weights );
    color += pcol * factor( luminosity, ecart + vec2( SCALE, -SCALE) );
    
    // TOP-RIGHT
    pcol = getColor( coords_texture + oney + onex);
    luminosity = dot( pcol, luminosity_weights );
    color += pcol * factor( luminosity, ecart + vec2( -SCALE, -SCALE) );
    
    // BOTTOM
    pcol = getColor( coords_texture - oney);
    luminosity = dot( pcol, luminosity_weights );
    color += pcol * factor( luminosity, ecart + vec2( 0.0, SCALE) );
    
    // BOTTOM-LEFT
    pcol = getColor( coords_texture - oney - onex);
    luminosity = dot( pcol, luminosity_weights );
    color += pcol * factor( luminosity, ecart + vec2( SCALE, SCALE) );
    
    // BOTTOM-RIGHT
    pcol = getColor( coords_texture - oney + onex);
    luminosity = dot( pcol, luminosity_weights );
    color += pcol * factor( luminosity, ecart + vec2( -SCALE, SCALE) );
    
     */
    
    //vec4 final = getColor(uv);
    //final.rgb = pow(final.rgb, vec3(1.8f));

    
    gl_FragColor = clamp( color, 0.0, 1.0 );

	
	
}