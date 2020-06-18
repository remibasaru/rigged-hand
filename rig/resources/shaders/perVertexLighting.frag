#version 330

in vec3 vColour;			// Interpolated colour using colour calculated in the vertex shader
in vec2 vTexCoord;			// Interpolated texture coordinate using texture coordinate from the vertex shader

in float bw1;
in float bw2;
in float bw3;

out vec4 vOutputColour;		// The output colour



uniform sampler2D gSampler;  // The texture sampler
uniform bool bUseTexture;    // A flag indicating if texture-mapping should be applied
uniform int iRenderMode;

void main()
{
	// Get the texel colour from the texture sampler
	vec4 vTexColour = texture(gSampler, vTexCoord);	

	if (iRenderMode == 0) {

		if (bUseTexture)
			{ 
		
		vOutputColour=vTexColour;  //vec4(gl_FragCoord.z,0,0,1.0);
		}
		else
			vOutputColour = vec4(vColour, 1.0f);	// Just use the colour instead
	} else {
		if (iRenderMode == 1)
			vOutputColour = vec4(bw1*vColour.r, 0, 0, 1);
		else if (iRenderMode == 2)
			vOutputColour = vec4(0, bw2*vColour.r, 0, 1);
		else 
			vOutputColour = vec4(0, 0, bw3*vColour.r, 1);

	}

	
	
}
