//
// Skinned Mesh Effect file 
// Copyright (c) 2000-2002 Microsoft Corporation. All rights reserved.
//

float3 lightPos = { 10.0f, 10.0f, 10.0f };
//배열로 만들어야함
float3 lightObjPos = { 10.0f, 2.0f, 10.0f };

float3 lightDiffuse = { 0.2f, 0.2f, 0.2f }; // Light Diffuse
float3 lightAmbient = { 0.05f, 0.05f, 0.05f };
float3 lightSpecular = { 1.0f, 1.0f, 1.0f };
float  lightDistance = 100.f;
float  lightSpecularPower = 25.0f;

// Matrix Pallette
static const int MAX_MATRICES = 26;
float4x3    mWorldMatrixArray[MAX_MATRICES] : WORLDMATRIXARRAY;
float4x4    mViewProj : VIEWPROJECTION;
float4x4    mWorld : WORLD;
float3	   mCamaraPos = { 0.0f, 5.0f, 0.0f };

float3 fogColor = {0.3f, 0.3f, 0.3f};
float fogStart = 0.f;
float fogEnd = 200.f;

texture	   mTexture;
texture	   mTexture2;
texture	   mAlphaMap;

//--------------------------------------------------------------------------------------
// Texture samplers
//--------------------------------------------------------------------------------------
sampler samSkyBox = 
sampler_state
{
	Texture = <mTexture>;
};

sampler samTex01 =
sampler_state
{
	Texture = <mTexture>;
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Point;
};

sampler samTex02 =
sampler_state
{
	Texture = <mTexture2>;
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Point;
};

sampler samAlpha01 =
sampler_state
{
	Texture = <mAlphaMap>;
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Point;
};

///////////////////////////////////////////////////////
struct VS_INPUT_ANI
{
    float4  Pos             : POSITION;
    float4  BlendWeights    : BLENDWEIGHT;
    float4  BlendIndices    : BLENDINDICES;
    float3  Normal          : NORMAL;
    float3  Tex0            : TEXCOORD0;
};

struct VS_OUTPUT_ANI
{
    float4  Pos     : POSITION;
    float2  Tex0    : TEXCOORD0;
	float3  Normal  : TEXCOORD1;
	float3  WorldPos : TEXCOORD2;
};

struct VS_INPUT_GROUND 
{
	float4 Pos : POSITION;
	float3 Normal : NORMAL;
	float2 Tex0 : TEXCOORD0;
	float2 Tex1 : TEXCOORD1;
	float2 Tex2 : TEXCOORD2;
};

struct VS_OUTPUT_GROUND
{
	float4  Pos     : POSITION;
	float2  Tex0    : TEXCOORD0;
	float2  Tex1	   : TEXCOORD1;
	float2  Tex2	   : TEXCOORD2;
	float3  Normal  : TEXCOORD3;
	float3  WorldPos : TEXCOORD4;
};

struct VS_INPUT_MESH
{
	float4 Pos : POSITION;
	float3 Normal : NORMAL;
	float2 Tex0 : TEXCOORD0;
};

struct VS_OUTPUT_MESH
{
	float4  Pos     : POSITION;
	float2  Tex0    : TEXCOORD0;
	float3  Normal  : TEXCOORD1;
	float3  WorldPos : TEXCOORD2;
};

///////////////////////////////////////////////////////

VS_OUTPUT_GROUND VShadeGround( VS_INPUT_GROUND Input )
{
	VS_OUTPUT_GROUND o;

	float4 posWorld = mul(Input.Pos, mWorld);
	o.Pos = mul(posWorld, mViewProj);
	o.Normal = mul(Input.Normal, (float3x3)mWorld);
	o.WorldPos = posWorld;

	o.Tex0 = Input.Tex0;
	o.Tex1 = Input.Tex1;
	o.Tex2 = Input.Tex2;

	return o;
}

float4 PShadeGround(	
				float2 Tex0 : TEXCOORD0,
				float2 Tex1 : TEXCOORD1,
				float2 Tex2 : TEXCOORD2,
				float3 Normal : TEXCOORD3,
				float3 WorldPos : TEXCOORD4) : COLOR0
{

	float3 lightDir = normalize(WorldPos - lightPos); // per pixel diffuse lighting

	// Note: Non-uniform scaling not supported
	float diffuseLighting = saturate(dot(Normal, -lightDir));

	// Introduce fall-off of light intensity
	diffuseLighting *= (lightDistance / dot(lightPos - WorldPos, lightPos - WorldPos));

	// Using Blinn half angle modification for perofrmance over correctness
	//float3 h = normalize(normalize(mCamaraPos - WorldPos) - lightDir);
	//float specLighting = pow(saturate(dot(h, Normal)), lightSpecularPower);
	
	float4 white = { 1.0f, 1.0f, 1.0f, 1.0f };
	float4 splate1 = tex2D(samTex01, Tex0) * tex2D(samAlpha01, Tex1);
	float4 splate2 = tex2D(samTex02, Tex2) * (white - tex2D(samAlpha01, Tex1));
	float4 BaseColor = splate1 + splate2;
	//float4 texel = tex2D(texsampler, input.TexCoords);

	float4 TotalAmbient = float4(lightAmbient * BaseColor, 1.f);

	
	//lightobj
	float3 lightDir2 = normalize(WorldPos - lightObjPos); // per pixel diffuse lighting

	// Note: Non-uniform scaling not supported
	float diffuseLighting2 = saturate(dot(Normal, -lightDir2));

	// Introduce fall-off of light intensity
	diffuseLighting2 *= (lightDistance / dot(lightObjPos - WorldPos, lightObjPos - WorldPos));

	// Using Blinn half angle modification for perofrmance over correctness
	//float3 h2 = normalize(normalize(mCamaraPos - WorldPos) - lightDir2);
	//float specLighting2 = pow(saturate(dot(h2, Normal)), lightSpecularPower);
	
	/*
	float4 totalColor = float4(saturate(TotalAmbient +
		(BaseColor.xyz * lightDiffuse * diffuseLighting * 0.6)
		+ (BaseColor.xyz * lightDiffuse * diffuseLighting2 * 0.6)), 1.f);
		*/

	float3 totalColor = TotalAmbient +
		(BaseColor.xyz * lightDiffuse * diffuseLighting * 0.6)
		+ (BaseColor.xyz * lightDiffuse * diffuseLighting2 * 0.6);

	float ViewpointDistance = length(mCamaraPos - WorldPos.xyz);
	//선형안개
	float fogFactor = saturate((fogEnd - ViewpointDistance) / (fogEnd - fogStart));

	float3 fogColorFactor = fogColor*(1.0f - fogFactor);
	//마지막에 안개공식을 적용
	float4 finalColor = float4(saturate(totalColor*fogFactor + fogColorFactor), 1.f);

	return finalColor;
}

//////////////////////////////////////////////////////

VS_OUTPUT_ANI VShadeAni(VS_INPUT_ANI i, uniform int NumBones)
{
	VS_OUTPUT_ANI   o;
    float3      Pos = 0.0f;
    float3      Normal = 0.0f;    
    float       LastWeight = 0.0f;
     
    // Compensate for lack of UBYTE4 on Geforce3
    int4 IndexVector = D3DCOLORtoUBYTE4(i.BlendIndices);

    // cast the vectors to arrays for use in the for loop below
    float BlendWeightsArray[4] = (float[4])i.BlendWeights;
    int   IndexArray[4]        = (int[4])IndexVector;
    
    // calculate the pos/normal using the "normal" weights 
    //        and accumulate the weights to calculate the last weight
    for (int iBone = 0; iBone < NumBones-1; iBone++)
    {
        LastWeight = LastWeight + BlendWeightsArray[iBone];
        
        Pos += mul(i.Pos, mWorldMatrixArray[IndexArray[iBone]]) * BlendWeightsArray[iBone];
        Normal += mul(i.Normal, mWorldMatrixArray[IndexArray[iBone]]) * BlendWeightsArray[iBone];
    }
    LastWeight = 1.0f - LastWeight; 

    // Now that we have the calculated weight, add in the final influence
    Pos += (mul(i.Pos, mWorldMatrixArray[IndexArray[NumBones-1]]) * LastWeight);
    Normal += (mul(i.Normal, mWorldMatrixArray[IndexArray[NumBones-1]]) * LastWeight); 
    
    // transform position from world space into view and then projection space
    o.Pos = mul(float4(Pos.xyz, 1.0f), mViewProj);

    // normalize normals
    Normal = normalize(Normal);
	o.Normal = Normal.xyz;

    // copy the input texture coordinate through
    o.Tex0  = i.Tex0.xy;

	//float4x4 World = mWorldMatrixArray[IndexArray[NumBones - 1]];
	float4 posWorld = mul(i.Pos, mWorld);
	o.WorldPos = posWorld.xyz;

    return o;
}

float4 PShadeAni(float4  Pos     : POSITION,
				float2  Tex0    : TEXCOORD0,
				float3  Normal  : TEXCOORD1,
				float3  WorldPos : TEXCOORD2) : COLOR0
{
	float3 lightDir = normalize(WorldPos - lightPos); // per pixel diffuse lighting

	// Note: Non-uniform scaling not supported
	float diffuseLighting = saturate(dot(Normal, -lightDir));

	// Introduce fall-off of light intensity
	diffuseLighting *= (lightDistance / dot(lightPos - WorldPos, lightPos - WorldPos));

	// Using Blinn half angle modification for perofrmance over correctness
	//float3 h = normalize(normalize(mCamaraPos - WorldPos) - lightDir);
	//float specLighting = pow(saturate(dot(h, Normal)), lightSpecularPower);

	float4 texel = tex2D(samTex01, Tex0);

	float4 TotalAmbient = float4(lightAmbient * texel, 1.f);

	//return texel;
	
	return float4(saturate(
		TotalAmbient +
		(texel.xyz * lightDiffuse * diffuseLighting * 0.6)
		), texel.w);
}

float4 PShadeLightObj(	float2  Tex0 : TEXCOORD0,
						float3  WorldPos : TEXCOORD2) : COLOR0
{
	float4 texel = tex2D(samTex01, Tex0);

	float ViewpointDistance = length(mCamaraPos - WorldPos.xyz);
	//선형안개
	float fogFactor = saturate((fogEnd - ViewpointDistance) / (fogEnd - fogStart));

	float3 fogColorFactor = fogColor*(1.0f - fogFactor);
	//마지막에 안개공식을 적용
	float4 finalColor = float4(saturate(texel*fogFactor + fogColorFactor), texel.w);

	return finalColor;
}

int CurNumBones = 2;
VertexShader vsArray[4] = { compile vs_2_0 VShadeAni(1),
                            compile vs_2_0 VShadeAni(2),
							compile vs_2_0 VShadeAni(3),
							compile vs_2_0 VShadeAni(4)
                          };

//////////////////////////////////////////////////////

VS_OUTPUT_MESH VShadeMesh(VS_INPUT_MESH Input)
{
	VS_OUTPUT_MESH o;

	float4 posWorld = mul(Input.Pos, mWorld);
	o.Pos = mul(posWorld, mViewProj);
	o.Normal = mul(Input.Normal, (float3x3)mWorld);
	o.WorldPos = posWorld;

	o.Tex0 = Input.Tex0;

	return o;
}

float4 PShadeMesh(
	float2 Tex0 : TEXCOORD0,
	float3 Normal : TEXCOORD1,
	float3 WorldPos : TEXCOORD2) : COLOR0
{
	float3 lightDir = normalize(WorldPos - lightPos); // per pixel diffuse lighting

	// Note: Non-uniform scaling not supported
	float diffuseLighting = saturate(dot(Normal, -lightDir));

	// Introduce fall-off of light intensity
	diffuseLighting *= (lightDistance / dot(lightPos - WorldPos, lightPos - WorldPos));

	// Using Blinn half angle modification for perofrmance over correctness
	//float3 h = normalize(normalize(mCamaraPos - WorldPos) - lightDir);
	//float specLighting = pow(saturate(dot(h, Normal)), lightSpecularPower);

	float4 texel = tex2D(samTex01, Tex0);

	float4 TotalAmbient = float4(lightAmbient * texel, 1.f);

	//return texel;

	float4 totalColor = float4(saturate(
		TotalAmbient +
		(texel.xyz * lightDiffuse * diffuseLighting * 0.6)
		), texel.w);

	float ViewpointDistance = length(mCamaraPos - WorldPos.xyz);
	//선형안개
	float fogFactor = saturate((fogEnd - ViewpointDistance) / (fogEnd - fogStart));

	float3 fogColorFactor = fogColor*(1.0f - fogFactor);
	//마지막에 안개공식을 적용
	float4 finalColor = float4(saturate(totalColor*fogFactor + fogColorFactor), totalColor.w);

	return finalColor;
}

//////////////////////////////////////////////////////

VS_OUTPUT_MESH VShadeSkyBox(VS_INPUT_MESH Input) 
{
	VS_OUTPUT_MESH o;

	float4 posWorld = mul(Input.Pos, mWorld);
	o.Pos = mul(posWorld, mViewProj);
	o.Normal = mul(Input.Normal, (float3x3)mWorld);
	o.WorldPos = posWorld;

	o.Tex0 = Input.Tex0;

	return o;
}

float4 PShadeSkyBox(
		float2 Tex0 : TEXCOORD0) : COLOR0
{
	return tex2D(samSkyBox, Tex0);
}

//////////////////////////////////////////////////////

VS_OUTPUT_MESH VShadeBillBord(VS_INPUT_MESH Input)
{
	VS_OUTPUT_MESH o;

	float4 posWorld = mul(Input.Pos, mWorld);
		o.Pos = mul(posWorld, mViewProj);
	o.Normal = mul(Input.Normal, (float3x3)mWorld);
	o.WorldPos = posWorld;

	o.Tex0 = Input.Tex0;

	return o;
}

float4 PShadeBillBord(
	float2 Tex0 : TEXCOORD0,
	float3 WorldPos : TEXCOORD2) : COLOR0
{
	float4 texel = tex2D(samSkyBox, Tex0);
	float4 TotalAmbient = float4(lightAmbient * texel, texel.w);

	float ViewpointDistance = length(mCamaraPos - WorldPos.xyz);
	//선형안개
	float fogFactor = saturate((fogEnd - ViewpointDistance) / (fogEnd - fogStart));

	float3 fogColorFactor = fogColor*(1.0f - fogFactor);
	//마지막에 안개공식을 적용
	float4 finalColor = float4(saturate(TotalAmbient*fogFactor + fogColorFactor), TotalAmbient.w);

	return finalColor;
}

//////////////////////////////////////
// Techniques specs follow
//////////////////////////////////////

//skinnedmesh용
technique t0
{
    pass p0
    {
        VertexShader = (vsArray[CurNumBones]);
		PixelShader = compile ps_2_0 PShadeAni();
    }
}

//그라운드용
technique t1 
{
	pass p0 
	{
		VertexShader = compile vs_2_0 VShadeGround();
		PixelShader = compile ps_2_0 PShadeGround();
	}
}

//x매쉬용
technique t2
{
	pass p0
	{
		VertexShader = compile vs_2_0 VShadeMesh();
		PixelShader = compile ps_2_0 PShadeMesh();
	}
}

//스카이박스용
technique t3
{
	pass p0
	{
		VertexShader = compile vs_2_0 VShadeSkyBox();
		PixelShader = compile ps_2_0 PShadeSkyBox();
	}
}

//빌보드용
technique t4
{
	pass p0
	{
		VertexShader = compile vs_2_0 VShadeBillBord();
		PixelShader = compile ps_2_0 PShadeBillBord();
	}
}

//라이트오브용
//뭔가 버그가 있는데 잡을 시간이 없음ㅠㅠ
technique t5
{
	pass p0
	{
		VertexShader = (vsArray[CurNumBones]);
		PixelShader = compile ps_2_0 PShadeLightObj();
	}
}