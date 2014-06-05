//
// Skinned Mesh Effect file 
// Copyright (c) 2000-2002 Microsoft Corporation. All rights reserved.
//

float4 lhtDir = {0.0f, 0.0f, -1.0f, 1.0f};    //light Direction 
float4 lightDiffuse = {0.6f, 0.6f, 0.6f, 1.0f}; // Light Diffuse
float4 MaterialAmbient : MATERIALAMBIENT = {0.1f, 0.1f, 0.1f, 1.0f};
float4 MaterialDiffuse : MATERIALDIFFUSE = {0.8f, 0.8f, 0.8f, 1.0f};

// Matrix Pallette
static const int MAX_MATRICES = 26;
float4x3    mWorldMatrixArray[MAX_MATRICES] : WORLDMATRIXARRAY;
float4x4    mViewProj : VIEWPROJECTION;
float4x4    mWorld : WORLD;

texture	   mTexture;
texture	   mTexture2;
texture	   mAlphaMap;

//--------------------------------------------------------------------------------------
// Texture samplers
//--------------------------------------------------------------------------------------
sampler g_samScene =
sampler_state
{
	Texture = <mTexture>;
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Point;
};

sampler g_samScene3 =
sampler_state
{
	Texture = <mTexture2>;
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Point;
};

sampler g_samScene2 =
sampler_state
{
	Texture = <mAlphaMap>;
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Point;
};

///////////////////////////////////////////////////////
struct VS_INPUT
{
    float4  Pos             : POSITION;
    float4  BlendWeights    : BLENDWEIGHT;
    float4  BlendIndices    : BLENDINDICES;
    float3  Normal          : NORMAL;
    float3  Tex0            : TEXCOORD0;
};

struct VS_OUTPUT
{
    float4  Pos     : POSITION;
    float4  Diffuse : COLOR;
    float2  Tex0    : TEXCOORD0;
};

struct VSG_OUTPUT
{
	float4  Pos     : POSITION;
	float4  Diffuse : COLOR;
	float2  Tex0    : TEXCOORD0;
	float2  Tex1	   : TEXCOORD1;
	float2  Tex2	   : TEXCOORD2;
};

///////////////////////////////////////////////////////

VSG_OUTPUT VertScene(float4 Pos : POSITION,
	float4 Diffuse : COLOR,
	float3 Normal : NORMAL,
	float2 Tex0 : TEXCOORD0,
	float2 Tex1 : TEXCOORD1,
	float2 Tex2 : TEXCOORD2)
{
	VSG_OUTPUT o;

	o.Pos = mul(Pos, mWorld);
	o.Pos = mul(o.Pos, mViewProj);
	o.Tex0 = Tex0;
	o.Tex1 = Tex1;
	o.Tex2 = Tex2;
	float3 N = normalize(mul(Normal, (float3x3)mWorld));

	o.Diffuse = Diffuse;

	return o;
}

float4 PixScene(	float4 Diffuse : COLOR0,
				float2 Tex0 : TEXCOORD0,
				float2 Tex1 : TEXCOORD1,
				float2 Tex2 : TEXCOORD2) : COLOR0
{
	float4 white = { 1.0f, 1.0f, 1.0f, 1.0f };
	float4 splate1 = tex2D(g_samScene, Tex0) * tex2D(g_samScene2, Tex1);
	float4 splate2 = tex2D(g_samScene3, Tex2) * (white - tex2D(g_samScene2, Tex1));

	return splate1 + splate2;
	//return tex2D(g_samScene, Tex0);
}

float3 Diffuse(float3 Normal)
{
    float CosTheta;
    
    // N.L Clamped
    CosTheta = max(0.0f, dot(Normal, lhtDir.xyz));
       
    // propogate scalar result to vector
    return (CosTheta);
}


VS_OUTPUT VShade(VS_INPUT i, uniform int NumBones)
{
    VS_OUTPUT   o;
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

    // Shade (Ambient + etc.)
    o.Diffuse.xyz = MaterialAmbient.xyz + Diffuse(Normal) * MaterialDiffuse.xyz;
    o.Diffuse.w = 1.0f;

    // copy the input texture coordinate through
    o.Tex0  = i.Tex0.xy;

    return o;
}

int CurNumBones = 2;
VertexShader vsArray[4] = { compile vs_2_0 VShade(1), 
                            compile vs_2_0 VShade(2),
                            compile vs_2_0 VShade(3),
                            compile vs_2_0 VShade(4)
                          };


//////////////////////////////////////
// Techniques specs follow
//////////////////////////////////////
technique t0
{
    pass p0
    {
        VertexShader = (vsArray[CurNumBones]);
    }
}

technique t1 
{
	pass p0 
	{
		VertexShader = compile vs_2_0 VertScene();
		PixelShader = compile ps_2_0 PixScene();
	}
}

