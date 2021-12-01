#version 330
// A texture is expected as program attribute
uniform sampler2D Texture;

// Direction of light
uniform vec3 LightDirection;

uniform vec3 LightDirection2 = {0, 1.0f, 1.0f};

// (optional) Transparency
uniform float Transparency;

// (optional) Texture offset
uniform vec2 TextureOffset;

uniform vec3 MaterialDiff = {1,1,1};

uniform vec3 LightColor = {1,1,1};

uniform vec3 LightColor2 = {0,0,0};

uniform mat3 Filtr = {
{0,0,0},
{0,1,0},
{0,0,0}
};
// The vertex shader will feed this input
in vec2 texCoord;

// Wordspace normal passed from vertex shader
in vec4 normal;

// The final color
out vec4 FragmentColor;

void main() {
  /*
  float[25] kernel = float[] (
  0.0,  1.0,  1.0,  1.0, 1.0,
  -1.0,  0.0,  1.0,  1.0, 1.0,
  -1.0, -1.0,  0.0,  1.0, 1.0,
  -1.0, -1.0, -1.0,  0.0, 1.0,
  -1.0, -1.0, -1.0, -1.0, 0.0);

  */

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

  // Compute diffuse lighting
  float diffuse = max(dot(normal, vec4(normalize(LightDirection), 1.0f)), 0.0f);

  float diffuse2 = max(dot(normal, vec4(normalize(LightDirection2), 1.0f)), 0.0f);

  // Lookup the color in Texture on coordinates given by texCoord
  // NOTE: Texture coordinate is inverted vertically for compatibility with OBJ
  vec4 c1 = color * vec4(LightColor*(MaterialDiff * diffuse), 1);
  vec4 c2 = color * vec4(LightColor2*(MaterialDiff * diffuse2), 1);
  FragmentColor = c1 + c2;
  FragmentColor.a = Transparency;
}
