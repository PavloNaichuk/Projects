struct InputData
{
	float4 screenSpacePos : SV_Position;
	float2 texCoords : TexCoords;
};

Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);

float4 Main(InputData inputData) : SV_Target
{
	return g_Texture.Sample(g_SamplerState, inputData.texCoords);
}