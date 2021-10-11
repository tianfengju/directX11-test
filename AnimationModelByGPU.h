#pragma once
#include <unordered_map>
#include "assimp/cimport.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/matrix4x4.h"
#pragma comment (lib,"assimp.lib")

//変形後頂点構造体
struct DEFORM_VERTEX_GPU {
	aiVector3D Position;
	aiVector3D Normal;
	
	//std::string BoneName[4];//本来はボーンインデックスで管理するべき
	unsigned int         BoneIndex[4];
	float       BoneWeight[4];
	int         BoneNum;
};

struct BONE_GPU
{
	aiMatrix4x4 Matrix;
	aiMatrix4x4 AnimationMatrix;
	aiMatrix4x4 OffsetMatrix;
};

struct BONE_MATRIX
{
	aiMatrix4x4 Matrix;
};

//struct SkinBuffer
//{
//	D3DXMATRIX boneMatrix[64];
//};

//struct SKIN_VERTEX_3D
//{
//	D3DXVECTOR3 Position;
//	D3DXVECTOR3 Normal;
//	D3DXVECTOR4 Diffuse;
//	D3DXVECTOR4 SkinIndex;
//	D3DXVECTOR4 SkinWeight;
//	D3DXVECTOR2 TexCoord;
//};


class AnimationModelByGPU {
private:
	const aiScene* m_AiScene = NULL;
	std::unordered_map<std::string, const aiScene*> m_Animation;
	ID3D11Buffer** m_VertexBuffer;
	ID3D11Buffer** m_IndexBuffer;

	ID3D11VertexShader* m_SkinVertexShader;
	ID3D11InputLayout* m_InputLayout;
	//ID3D11Buffer* m_ConstantSkinBuffer;
	ID3D11ComputeShader* m_ComputeShader;

	std::unordered_map<std::string, ID3D11ShaderResourceView*> m_Texture;

	//変形後頂点データ
	//std::vector<DEFORM_VERTEX_GPU>* m_DeformVertex;
	DEFORM_VERTEX_GPU** m_DeformVertex;
	ID3D11Buffer** m_DeformVertexBuffer;
	ID3D11Buffer** m_ResultVertexBuffer;
	ID3D11Buffer** m_ReadBackBuffer;
	ID3D11ShaderResourceView** m_DeformSRV;
	ID3D11UnorderedAccessView** m_ResultUAV;
	BONE_MATRIX* m_BoneMatrix;
	ID3D11Buffer* m_BoneMatrixBuffer = NULL;
	ID3D11ShaderResourceView* m_BoneSRV;
	//ボーンデータ（名前で参照）
	std::unordered_map<std::string, BONE_GPU> m_Bone;
	//ボーンデータ（インデックスで参照）
	std::unordered_map<std::string,int> m_NameID;
	//std::unordered_map<std::string> m_NameID;
	//SkinBuffer m_Skinbuffer;
	int boneP = 0;

	void CreateBone(aiNode* node);

	void UpdateBoneMatrix(aiNode* node, aiMatrix4x4 matrix);

	void CreateVertexShader(const char* fileName);

	void CreateComputeShader(const char* fileName,ID3D11ComputeShader** ComputeShader);

	D3DXMATRIX getMatrix(aiMatrix4x4 aiMat);

public:
	void Load(const char* FileName);
	void LoadAnimation(const char* FileName, const char* Name);
	//void LoadAnimationToGPU(const char* FileName, const char* Name);
	void Unload();
	void Update(const char* AnimationName, const char* AnimationName2, float BlendRate, int Frame);
	void Draw();

};