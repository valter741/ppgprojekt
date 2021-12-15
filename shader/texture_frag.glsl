#version 330
// A texture is expected as program attribute
uniform sampler2D Texture;

// (optional) Transparency
uniform float Transparency;

// (optional) Texture offset
uniform vec2 TextureOffset;

uniform vec3 LightColor = {1,1,1};

uniform mat3 Filtr = {
{0,0,0},
{0,1,0},
{0,0,0}
};


// The vertex shader will feed this input
in vec2 texCoord;

// The final color
out vec4 FragmentColor;

void main() {
  float[9] kernel = float[] (
  Filtr[0][0],  Filtr[0][1],  Filtr[0][2],
  Filtr[1][0],  Filtr[1][1],  Filtr[1][2],
  Filtr[2][0],  Filtr[2][1],  Filtr[2][2]);

  int index = 0;
  vec4 color = vec4(0);
  float factor = 1.0;
  float bias = 0.0;

  for (int i = -1; i <= 1; i++) {
    for (int j = -1; j <= 1; j++) {
      vec2 shift = vec2(i,j) / textureSize(Texture,0);
      color += kernel[index++] * texture(Texture, vec2(texCoord.x, 1.0 - texCoord.y) + TextureOffset + shift);
    }
  }

  // Lookup the color in Texture on coordinates given by texCoord
  // NOTE: Texture coordinate is inverted vertically for compatibility with OBJ
  FragmentColor = color * vec4(LightColor, 1);
  FragmentColor.a = Transparency;
}
