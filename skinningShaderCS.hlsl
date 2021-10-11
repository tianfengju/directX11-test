

struct Bone
{
	float4x4 pose;
};

struct DeformStruct
{
	float3 position;
	float3 normal;
	uint4 boneIndex;
	float4 weight;
	uint boneNum;
};

StructuredBuffer<Bone> boneBuffer : register(t0);
StructuredBuffer<DeformStruct> deformVertex : register(t1);
RWStructuredBuffer<DeformStruct> deformOut : register(u0);
//ByteAddressBuffer vertexBuffer_POS;  //T-Pose pos
//ByteAddressBuffer vertexBuffer_NOR;  //T-Pose normal
//ByteAddressBuffer vertexBuffer_WEI;  // bone weights
//ByteAddressBuffer vertexBuffer_BON;  // bone Index
//RWByteAddressBuffer streamoutBuffer_POS;
//RWByteAddressBuffer streamoutBuffer_NOR;
//RWByteAddressBuffer streamoutBuffer_PRE; //previous frame skinned pos

//inline void Skinning(inout float4 pos, inout float4 nor, in float4 inBon, in float4 inWei)
//{
//	float4 p = 0, pp = 0;
//	float3 n = 0;
//	float4x4 m;
//	float3x3 m3;
//	float weisum = 0;
//
//	[loop]
//
//	for (uint i = 0; ((i &lt; 4)&amp; &amp; (weisum&lt; 1.0f)); ++i)
//	{
//		m = boneBuffer[(uint)inBon[i]].pose;
//		m3 = (float3x3)m;
//		p += mul(float4(pos.xyz, 1), m) * inWei[i];
//		m += mul(nor.xyz, m3) * inWei[i];
//		weisum += inWei[i];
//	}
//
//	bool w = any(inWei);
//	pos.xyz = w ? p.xyz : pos.xyz;
//	nor.xyz = w ? n : nor.xyz;
//}

[numthreads(256,1,1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
	uint index = DTid.x;
	DeformStruct ds = deformVertex[index];
	//DeformStruct outDs;

	matrix mat = boneBuffer[ds.boneIndex.x].pose * ds.weight.x
		+ boneBuffer[ds.boneIndex.y].pose * ds.weight.y
		+ boneBuffer[ds.boneIndex.z].pose * ds.weight.z
		+ boneBuffer[ds.boneIndex.w].pose * ds.weight.w;


	deformOut[index].position = mul(float4(ds.position,1), mat);
	float3x3 nmat = (float3x3)mat;
	deformOut[index].normal = mul(ds.normal, nmat);

	
}