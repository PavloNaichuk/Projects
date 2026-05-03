struct TransformData
{
	matrix viewProjMatrix;
	float texSpaceImageWidth;
	float3 padding0;
	float4 padding1;
	float4 padding2;
	float4 padding3;
};

struct InstanceData
{
	float2 worldSpaceCenter;
	float2 halfSize;
	uint textureId;
};

cbuffer TransformDataBuffer : register(b0)
{
	TransformData g_TransformData;
};
StructuredBuffer<InstanceData> g_InstanceDataBuffer : register(t0);

struct InputData
{
	uint instanceId : SV_InstanceID;
	uint vertexId : SV_VertexID;
};

struct OutputData
{
	float4 clipSpacePos : SV_Position;
	float2 texCoords : TexCoords;
};

static const float2 g_QuadCornerScale[] =
{
	float2(-1.0f, -1.0f),
	float2(-1.0f,  1.0f),
	float2( 1.0f, -1.0f),
	float2( 1.0f,  1.0f)
};

static const float2 g_QuadTexCoords[] =
{
	float2(0.0f, 1.0f),
	float2(0.0f, 0.0f),
	float2(1.0f, 1.0f),
	float2(1.0f, 0.0f)
};

OutputData Main(InputData inputData)
{
	InstanceData instanceData = g_InstanceDataBuffer[inputData.instanceId];
	float2 worldSpaceCorner = instanceData.worldSpaceCenter + g_QuadCornerScale[inputData.vertexId] * instanceData.halfSize;
	float2 texCoords = g_QuadTexCoords[inputData.vertexId];

	OutputData outputData;
	outputData.clipSpacePos = mul(float4(worldSpaceCorner, 0.0f, 1.0f), g_TransformData.viewProjMatrix);
	outputData.texCoords = float2((instanceData.textureId + texCoords.x) * g_TransformData.texSpaceImageWidth, texCoords.y);
	
	return outputData;
}