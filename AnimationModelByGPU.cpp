#include "main.h"
#include "renderer.h"
#include "AnimationModelByGPU.h"
#include <io.h>

void AnimationModelByGPU::CreateBone(aiNode * node)
{
	BONE_GPU bone;
	m_Bone[node->mName.C_Str()] = bone;
	m_NameID[node->mName.C_Str()] = boneP;
	
	boneP++;

	for (unsigned int n = 0; n < node->mNumChildren; n++) {
		CreateBone(node->mChildren[n]);
	}

}

void AnimationModelByGPU::UpdateBoneMatrix(aiNode * node, aiMatrix4x4 matrix)
{
	BONE_GPU* bone = &m_Bone[node->mName.C_Str()];

	//マトリクスの乗算順番に注意（ここは列優先）
	aiMatrix4x4 worldMatrix;

	worldMatrix = matrix;   //親のマトリクス
	worldMatrix *= bone->AnimationMatrix; //自分マトリクス 

	bone->Matrix = worldMatrix;
	bone->Matrix *= bone->OffsetMatrix;

	int BoneNum = m_NameID[node->mName.C_Str()];
	m_BoneMatrix[BoneNum].Matrix = bone->Matrix;
	//m_BoneMatrix[BoneNum].Matrix = matrix;

	for (unsigned int n = 0; n < node->mNumChildren; n++) {
		UpdateBoneMatrix(node->mChildren[n], worldMatrix);
	}
}

void AnimationModelByGPU::CreateVertexShader(const char* fileName)
{
	FILE* file;
	long int fsize;

	file = fopen(fileName, "rb");
	fsize = _filelength(_fileno(file));
	unsigned char* buffer = new unsigned char[fsize];
	fread(buffer, fsize, 1, file);
	fclose(file);

	Renderer::GetDevice()->CreateVertexShader(buffer, fsize, NULL, &m_SkinVertexShader);

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 4 * 6, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, 4 * 10, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 4 * 14, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 2, DXGI_FORMAT_R32G32_FLOAT, 0, 4 * 18, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT numElements = ARRAYSIZE(layout);
	Renderer::GetDevice()->CreateInputLayout(layout,
		numElements,
		buffer,
		fsize,
		&m_InputLayout);
	delete[] buffer;
}

void AnimationModelByGPU::CreateComputeShader(const char * fileName, ID3D11ComputeShader** ComputeShader)
{
	FILE* file;
	long int fsize;

	file = fopen(fileName, "rb");
	fsize = _filelength(_fileno(file));
	unsigned char* buffer = new unsigned char[fsize];
	fread(buffer, fsize, 1, file);
	fclose(file);

	Renderer::GetDevice()->CreateComputeShader(buffer, fsize, nullptr, ComputeShader);
	delete[] buffer;
}

D3DXMATRIX AnimationModelByGPU::getMatrix(aiMatrix4x4 aiMat)
{
	D3DXMATRIX mat;
	mat._11 = aiMat.a1;
	mat._21 = aiMat.a2;
	mat._31 = aiMat.a3;
	mat._41 = aiMat.a4;
	mat._12 = aiMat.b1;
	mat._22 = aiMat.b2;
	mat._32 = aiMat.b3;
	mat._42 = aiMat.b4;
	mat._13 = aiMat.c1;
	mat._23 = aiMat.c2;
	mat._33 = aiMat.c3;
	mat._43 = aiMat.c4;
	mat._14 = aiMat.d1;
	mat._24 = aiMat.d2;
	mat._34 = aiMat.d3;
	mat._44 = aiMat.d4;
	return mat;
}

void AnimationModelByGPU::Load(const char * FileName)
{
	CreateComputeShader("skinningShaderCS.cso", &m_ComputeShader);
	const std::string modelPath(FileName);

	m_AiScene = aiImportFile(FileName, aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_ConvertToLeftHanded);
	assert(m_AiScene);

	boneP = 0;
	CreateBone(m_AiScene->mRootNode);

	m_BoneMatrix = new BONE_MATRIX[boneP];
	memset(m_BoneMatrix, 0, sizeof(BONE_MATRIX)* boneP);

	D3D11_BUFFER_DESC desc;
	memset(&desc, 0, sizeof(desc));
	desc.Usage = D3D11_USAGE_DYNAMIC;
	//desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags =  D3D11_BIND_SHADER_RESOURCE;
	//desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.ByteWidth = boneP * sizeof(BONE_MATRIX);
	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	desc.StructureByteStride = sizeof(BONE_MATRIX);
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	//D3D11_SUBRESOURCE_DATA boneData;
	//ZeroMemory(&boneData, sizeof(boneData));
	//boneData.pSysMem = m_BoneMatrix;

	Renderer::GetDevice()->CreateBuffer(&desc, nullptr, &m_BoneMatrixBuffer);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	memset(&srvDesc, 0, sizeof(srvDesc));
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	srvDesc.BufferEx.FirstElement = 0;
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.BufferEx.NumElements = boneP;

	Renderer::GetDevice()->CreateShaderResourceView(m_BoneMatrixBuffer, &srvDesc, &m_BoneSRV);

	m_VertexBuffer = new ID3D11Buffer*[m_AiScene->mNumMeshes];
	m_IndexBuffer = new ID3D11Buffer*[m_AiScene->mNumMeshes];
	m_DeformVertexBuffer = new ID3D11Buffer*[m_AiScene->mNumMeshes];
	m_ResultVertexBuffer = new ID3D11Buffer*[m_AiScene->mNumMeshes];
	m_DeformSRV = new ID3D11ShaderResourceView*[m_AiScene->mNumMeshes];
	m_ResultUAV = new ID3D11UnorderedAccessView*[m_AiScene->mNumMeshes];
	//m_BoneMatrixBuffer = new ID3D11Buffer*[m_AiScene]
	m_DeformVertex = new DEFORM_VERTEX_GPU*[m_AiScene->mNumMeshes];
	m_ReadBackBuffer = new ID3D11Buffer*[m_AiScene->mNumMeshes];

	
	for (unsigned int m = 0; m < m_AiScene->mNumMeshes; m++) {
		aiMesh* mesh = m_AiScene->mMeshes[m];

		m_DeformVertex[m] = new DEFORM_VERTEX_GPU[mesh->mNumVertices];
		//変形後頂点データ初期化
		for (unsigned int v = 0; v < mesh->mNumVertices; v++) {
			//DEFORM_VERTEX_GPU deformVertex;
			m_DeformVertex[m][v].Position = mesh->mVertices[v];
			m_DeformVertex[m][v].Normal = mesh->mNormals[v];
			m_DeformVertex[m][v].BoneNum = 0;

			for (unsigned int b = 0; b < 4; b++) {
				//deformVertex.BoneName[b] = "";
				m_DeformVertex[m][v].BoneIndex[b] = 0;
				m_DeformVertex[m][v].BoneWeight[b] = 0.0f;
			}

			//m_DeformVertex[m].push_back(deformVertex);
		}

		//ボーンデータ初期化
		for (unsigned int b = 0; b < mesh->mNumBones; b++) {
			aiBone* bone = mesh->mBones[b];
			m_Bone[bone->mName.C_Str()].OffsetMatrix = bone->mOffsetMatrix;

			//変形後頂点にボーンデータ格納
			for (unsigned int w = 0; w < bone->mNumWeights; w++) {
				aiVertexWeight weight = bone->mWeights[w];
				int num = m_DeformVertex[m][weight.mVertexId].BoneNum;

				m_DeformVertex[m][weight.mVertexId].BoneWeight[num] = weight.mWeight;
				//m_DeformVertex[m][weight.mVertexId].BoneName[num] = bone->mName.C_Str();
				m_DeformVertex[m][weight.mVertexId].BoneIndex[num] = m_NameID[bone->mName.C_Str()];
				//m_DeformVertex[m][weight.mVertexId].BoneIndex[num] = 34;
				m_DeformVertex[m][weight.mVertexId].BoneNum++;

				assert(m_DeformVertex[m][weight.mVertexId].BoneNum <= 4);

			}

		}

		D3D11_BUFFER_DESC desc;
		memset(&desc, 0, sizeof(desc));
		//desc.Usage = D3D11_USAGE_DEFAULT;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.ByteWidth = mesh->mNumVertices * sizeof(DEFORM_VERTEX_GPU);
		desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		desc.StructureByteStride = sizeof(DEFORM_VERTEX_GPU);
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		D3D11_SUBRESOURCE_DATA deformData;
		ZeroMemory(&deformData, sizeof(deformData));
		deformData.pSysMem = m_DeformVertex[m];

		Renderer::GetDevice()->CreateBuffer(&desc, &deformData, &m_DeformVertexBuffer[m]);


		D3D11_BUFFER_DESC uabDesc;
		memset(&uabDesc, 0, sizeof(uabDesc));
		//uabDesc.Usage = D3D11_USAGE_STAGING;
		uabDesc.Usage = D3D11_USAGE_DEFAULT;
		uabDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
		//uabDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		//uabDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG;
		uabDesc.ByteWidth = mesh->mNumVertices * sizeof(DEFORM_VERTEX_GPU);
		//uabDesc.ByteWidth = sizeof(DEFORM_VERTEX_GPU);
		uabDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		uabDesc.StructureByteStride = sizeof(DEFORM_VERTEX_GPU);


		Renderer::GetDevice()->CreateBuffer(&uabDesc, nullptr, &m_ResultVertexBuffer[m]);

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDescDefor;
		memset(&srvDescDefor, 0, sizeof(srvDescDefor));
		srvDescDefor.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
		srvDescDefor.BufferEx.FirstElement = 0;
		srvDescDefor.Format = DXGI_FORMAT_UNKNOWN;
		srvDescDefor.BufferEx.NumElements = mesh->mNumVertices;

		Renderer::GetDevice()->CreateShaderResourceView(m_DeformVertexBuffer[m], &srvDescDefor, &m_DeformSRV[m]);

		D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
		memset(&uavDesc, 0, sizeof(uavDesc));
		uavDesc.Format = DXGI_FORMAT_UNKNOWN;
		//uavDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		//uavDesc.ViewDimension = D3D11_UAV_DIMENSION_UNKNOWN;
		//uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
		//uavDesc.Texture2D.MipSlice = 0;
		uavDesc.Buffer.FirstElement = 0;		
		uavDesc.Buffer.NumElements = mesh->mNumVertices;
		//uavDesc.Buffer.NumElements = 1;
		//uavDesc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_COUNTER;
		
		//ID3D11UnorderedAccessView* mUAV;
		/*ID3D11Texture2D* tex2d;
		D3D11_TEXTURE2D_DESC  texDesc;
		texDesc.Width = 512;
		texDesc.Height = 512;
		texDesc.MipLevels = 1;
		texDesc.ArraySize = 1;
		texDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		texDesc.SampleDesc.Count = 1;
		texDesc.SampleDesc.Quality = 0;
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
		texDesc.CPUAccessFlags = 0;
		texDesc.MiscFlags = 0;*/

		//Renderer::GetDevice()->CreateTexture2D(&texDesc, nullptr, &tex2d);

		Renderer::GetDevice()->CreateUnorderedAccessView(m_ResultVertexBuffer[m], &uavDesc, &m_ResultUAV[m]);

		D3D11_BUFFER_DESC readBackDesc = {};
		readBackDesc.ByteWidth = mesh->mNumVertices * sizeof(DEFORM_VERTEX_GPU);
		readBackDesc.Usage = D3D11_USAGE_STAGING;
		readBackDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		readBackDesc.StructureByteStride = sizeof(DEFORM_VERTEX_GPU);

		
		Renderer::GetDevice()->CreateBuffer(&readBackDesc, nullptr, &m_ReadBackBuffer[m]);
		//頂点バッファ生成
		//{
		VERTEX_3D* vertex = new VERTEX_3D[mesh->mNumVertices];
		for (unsigned int v = 0; v < mesh->mNumVertices; v++) {
			vertex[v].Position = D3DXVECTOR3(mesh->mVertices[v].x, mesh->mVertices[v].y, mesh->mVertices[v].z);
			vertex[v].Normal = D3DXVECTOR3(mesh->mNormals[v].x, mesh->mNormals[v].y, mesh->mNormals[v].z);
			if (mesh->HasTextureCoords(m)) {
				vertex[v].TexCoord = D3DXVECTOR2(mesh->mTextureCoords[0][v].x, mesh->mTextureCoords[0][v].y);
			}
			else {
				vertex[v].TexCoord = D3DXVECTOR2(0.0f, 0.0f);
			}

			vertex[v].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
			
		}

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(VERTEX_3D) * mesh->mNumVertices;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = vertex;

		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer[m]);

		delete[] vertex;
		//}
		//インデックスバッファ生成
		{
			unsigned int* index = new unsigned int[mesh->mNumFaces * 3];
			for (unsigned int f = 0; f < mesh->mNumFaces; f++) {
				const aiFace* face = &mesh->mFaces[f];
				assert(face->mNumIndices == 3);
				index[f * 3 + 0] = face->mIndices[0];
				index[f * 3 + 1] = face->mIndices[1];
				index[f * 3 + 2] = face->mIndices[2];
			}

			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(unsigned int) * mesh->mNumFaces * 3;
			bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bd.CPUAccessFlags = 0;

			D3D11_SUBRESOURCE_DATA sd;
			ZeroMemory(&sd, sizeof(sd));
			sd.pSysMem = index;

			Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_IndexBuffer[m]);
			delete[] index;
		}
		
	}


	//テクスチャ読み込み
	for (unsigned int m = 0; m < m_AiScene->mNumMaterials; m++) {
		aiString path;
		if (m_AiScene->mMaterials[m]->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS) {
			if (path.data[0] == '*') {

				//FBXファイル内から読み込み
				if (m_Texture[path.data] == NULL) {
					ID3D11ShaderResourceView* texture;
					int id = atoi(&path.data[1]);

					D3DX11CreateShaderResourceViewFromMemory(
						Renderer::GetDevice(),
						(const unsigned char*)m_AiScene->mTextures[id]->pcData,
						m_AiScene->mTextures[id]->mWidth,
						NULL,
						NULL,
						&texture,
						NULL
					);

					//m_Texture.emplace(&texture);
					m_Texture[path.data] = texture;
				}
			}
			else {

			}

		}
		else {
			m_Texture[path.data] = NULL;
		}
	}
}

void AnimationModelByGPU::LoadAnimation(const char * FileName, const char * Name)
{
	const std::string modelPath(FileName);

	const aiScene* m_AiScene2 = aiImportFile(FileName, aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_ConvertToLeftHanded);
	assert(m_AiScene2);

	m_Animation[Name] = m_AiScene2;
	//const std::string modelPath(FileName);

	//m_AiScene = aiImportFile(FileName, aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_ConvertToLeftHanded);
	//assert(m_AiScene);

	//m_VertexBuffer = new ID3D11Buffer*[m_AiScene->mNumMeshes];
	//m_IndexBuffer = new ID3D11Buffer*[m_AiScene->mNumMeshes];

	////変形頂点データ
	//m_DeformVertex = new std::vector<DEFORM_VERTEX_GPU>[m_AiScene->mNumMeshes];

	////m_Skinbuffers = new std::vector<SkinBuffer>[m_AiScene->mNumMeshes];

	////再帰的にボーン生成
	//boneP = 0;
	//CreateBone(m_AiScene->mRootNode);


	//for (unsigned int m = 0; m < m_AiScene->mNumMeshes; m++) {
	//	aiMesh* mesh = m_AiScene->mMeshes[m];

	//	//変形後頂点データ初期化
	//	for (unsigned int v = 0; v < mesh->mNumVertices; v++) {
	//		DEFORM_VERTEX_GPU deformVertex;
	//		deformVertex.Position = mesh->mVertices[v];
	//		deformVertex.Normal = mesh->mNormals[v];
	//		deformVertex.BoneNum = 0;

	//		for (unsigned int b = 0; b < 4; b++) {
	//			deformVertex.BoneName[b] = "";
	//			deformVertex.BoneWeight[b] = 0.0f;
	//		}

	//		m_DeformVertex[m].push_back(deformVertex);
	//	}
	//	//ボーンデータ初期化
	//	for (unsigned int b = 0; b < mesh->mNumBones; b++) {
	//		aiBone* bone = mesh->mBones[b];
	//		m_Bone[bone->mName.C_Str()].OffsetMatrix = bone->mOffsetMatrix;
	//		
	//		//変形後頂点にボーンデータ格納
	//		for (unsigned int w = 0; w < bone->mNumWeights; w++) {
	//			aiVertexWeight weight = bone->mWeights[w];
	//			int num = m_DeformVertex[m][weight.mVertexId].BoneNum;

	//			m_DeformVertex[m][weight.mVertexId].BoneWeight[num] = weight.mWeight;
	//			m_DeformVertex[m][weight.mVertexId].BoneName[num] = bone->mName.C_Str();
	//			m_DeformVertex[m][weight.mVertexId].BoneNum++;

	//			assert(m_DeformVertex[m][weight.mVertexId].BoneNum <= 4);

	//		}

	//	}
	//	//頂点バッファ生成
	//	//{
	//		SKIN_VERTEX_3D* vertex = new SKIN_VERTEX_3D[mesh->mNumVertices];
	//		for (unsigned int v = 0; v < mesh->mNumVertices; v++) {
	//			vertex[v].Position = D3DXVECTOR3(mesh->mVertices[v].x, mesh->mVertices[v].y, mesh->mVertices[v].z);
	//			vertex[v].Normal = D3DXVECTOR3(mesh->mNormals[v].x, mesh->mNormals[v].y, mesh->mNormals[v].z);
	//			if (mesh->HasTextureCoords(m)) {
	//				vertex[v].TexCoord = D3DXVECTOR2(mesh->mTextureCoords[0][v].x, mesh->mTextureCoords[0][v].y);
	//			}
	//			else {
	//				vertex[v].TexCoord = D3DXVECTOR2(0.0f, 0.0f);
	//			}

	//			vertex[v].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);

	//			int boneIndex0 = m_NameID[m_DeformVertex[m][v].BoneName[0]];
	//			int boneIndex1 = m_NameID[m_DeformVertex[m][v].BoneName[1]];
	//			int boneIndex2 = m_NameID[m_DeformVertex[m][v].BoneName[2]];
	//			int boneIndex3 = m_NameID[m_DeformVertex[m][v].BoneName[3]];
	//			vertex[v].SkinIndex = D3DXVECTOR4(boneIndex0, boneIndex1, boneIndex2, boneIndex3);

	//			float bWeight0 = m_DeformVertex[m][v].BoneWeight[0];
	//			float bWeight1 = m_DeformVertex[m][v].BoneWeight[1];
	//			float bWeight2 = m_DeformVertex[m][v].BoneWeight[2];
	//			float bWeight3 = m_DeformVertex[m][v].BoneWeight[3];
	//			vertex[v].SkinWeight = D3DXVECTOR4(bWeight0, bWeight1, bWeight2, bWeight3);
	//		}

	//		D3D11_BUFFER_DESC bd;
	//		ZeroMemory(&bd, sizeof(bd));
	//		bd.Usage = D3D11_USAGE_DYNAMIC;
	//		bd.ByteWidth = sizeof(SKIN_VERTEX_3D) * mesh->mNumVertices;
	//		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	//		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	//		D3D11_SUBRESOURCE_DATA sd;
	//		ZeroMemory(&sd, sizeof(sd));
	//		sd.pSysMem = vertex;

	//		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer[m]);

	//		delete[] vertex;
	//	//}

	//	

	//	
	//}

	////テクスチャ読み込み
	//for (unsigned int m = 0; m < m_AiScene->mNumMaterials; m++) {
	//	aiString path;
	//	if (m_AiScene->mMaterials[m]->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS) {
	//		if (path.data[0] == '*') {

	//			//FBXファイル内から読み込み
	//			if (m_Texture[path.data] == NULL) {
	//				ID3D11ShaderResourceView* texture;
	//				int id = atoi(&path.data[1]);

	//				D3DX11CreateShaderResourceViewFromMemory(
	//					Renderer::GetDevice(),
	//					(const unsigned char*)m_AiScene->mTextures[id]->pcData,
	//					m_AiScene->mTextures[id]->mWidth,
	//					NULL,
	//					NULL,
	//					&texture,
	//					NULL
	//				);

	//				//m_Texture.emplace(&texture);
	//				m_Texture[path.data] = texture;
	//			}
	//		}
	//		else {

	//		}

	//	}
	//	else {
	//		m_Texture[path.data] = NULL;
	//	}
	//}

	//m_Animation[Name] = m_AiScene;
}



void AnimationModelByGPU::Unload()
{
	for (unsigned int m = 0; m < m_AiScene->mNumMeshes; m++) {
		m_VertexBuffer[m]->Release();
		m_IndexBuffer[m]->Release();
	}

	delete[] m_VertexBuffer;
	delete[] m_IndexBuffer;

	for (unsigned int m = 0; m < m_AiScene->mNumMeshes; m++) {
		m_DeformSRV[m]->Release();
		m_ResultUAV[m]->Release();
		m_DeformVertexBuffer[m]->Release();
		m_ResultVertexBuffer[m]->Release();	
		m_ReadBackBuffer[m]->Release();
		delete[] m_DeformVertex[m];
	}
	delete[] m_DeformVertex;
	delete[] m_ReadBackBuffer;
	delete[] m_ResultVertexBuffer;
	delete[] m_DeformVertexBuffer;
	delete[] m_ResultUAV;
	delete[] m_DeformSRV;
	m_BoneSRV->Release();
	m_BoneMatrixBuffer->Release();
	delete[] m_BoneMatrix;

	for (std::pair<std::string, ID3D11ShaderResourceView*>pair : m_Texture) {
		pair.second->Release();
	}
	for (std::pair<std::string, const aiScene*>pair : m_Animation) {
		aiReleaseImport(pair.second);
	}
	//aiReleaseImport(m_AiScene);
	//m_ConstantSkinBuffer->Release();
}

void AnimationModelByGPU::Update(const char* AnimationName, const char* AnimationName2, float BlendRate, int Frame)
{
	if (!m_Animation[AnimationName]->HasAnimations())return;
	if (!m_Animation[AnimationName2]->HasAnimations())return;

	//アニメーションデータからボーンマトリクス算出
	//aiAnimation* animation = m_AiScene->mAnimations[0];
	aiAnimation* animation = m_Animation[AnimationName]->mAnimations[0];
	aiAnimation* animation2 = m_Animation[AnimationName2]->mAnimations[0];

	for (unsigned int c = 0; c < animation->mNumChannels; c++) {
		aiNodeAnim* nodeAnim = animation->mChannels[c];
		aiNodeAnim* nodeAnim2 = animation2->mChannels[c];
		BONE_GPU* bone = &m_Bone[nodeAnim->mNodeName.C_Str()];

		int f;
		f = Frame % nodeAnim->mNumRotationKeys; //簡易実装
		aiQuaternion rot = nodeAnim->mRotationKeys[f].mValue;
		f = Frame % nodeAnim->mNumPositionKeys;
		aiVector3D pos = nodeAnim->mPositionKeys[f].mValue;

		f = Frame % nodeAnim2->mNumRotationKeys; //簡易実装
		aiQuaternion rot2 = nodeAnim2->mRotationKeys[f].mValue;
		f = Frame % nodeAnim2->mNumPositionKeys;
		aiVector3D pos2 = nodeAnim2->mPositionKeys[f].mValue;

		aiVector3D posCom = pos * (1.0f - BlendRate) + pos2 * BlendRate;
		aiQuaternion rotCom;
		aiQuaternion::Interpolate(rotCom, rot, rot2, BlendRate);//球面線形補間

		bone->AnimationMatrix = aiMatrix4x4(aiVector3D(1.0, 1.0f, 1.0f), rotCom, posCom);
	}

	//再帰的にボーンマトリクスを更新
	UpdateBoneMatrix(m_AiScene->mRootNode, aiMatrix4x4());
//**************************************************************
	D3D11_MAPPED_SUBRESOURCE bs;
	Renderer::GetDeviceContext()->Map(m_BoneMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &bs);
	BONE_MATRIX* boneMat = (BONE_MATRIX*)bs.pData;
	for (unsigned int b = 0; b < boneP; b++) {
		boneMat[b].Matrix = m_BoneMatrix[b].Matrix;
	}
	Renderer::GetDeviceContext()->Unmap(m_BoneMatrixBuffer, 0);
//********************************************************************************
	//コンピュートシェーダーで、計算を行う
	Renderer::GetDeviceContext()->CSSetShader(m_ComputeShader, nullptr, 0);
	Renderer::GetDeviceContext()->CSSetShaderResources(0, 1, &m_BoneSRV);

	for (unsigned int m = 0; m < m_AiScene->mNumMeshes; m++) {
		aiMesh* mesh = m_AiScene->mMeshes[m];

		Renderer::GetDeviceContext()->CSSetShaderResources(1, 1, &m_DeformSRV[m]);
		Renderer::GetDeviceContext()->CSSetUnorderedAccessViews(0, 1, &m_ResultUAV[m], nullptr);

		Renderer::GetDeviceContext()->Dispatch(256, 1, 1);
		
		ID3D11UnorderedAccessView* pUVA = NULL;
		Renderer::GetDeviceContext()->CSSetUnorderedAccessViews(0, 0, &pUVA, nullptr);

		Renderer::GetDeviceContext()->CopyResource(m_ReadBackBuffer[m], m_ResultVertexBuffer[m]);


//********************************************************************************
		//計算結果を格納
		D3D11_MAPPED_SUBRESOURCE ms;
		Renderer::GetDeviceContext()->Map(m_ReadBackBuffer[m], 0, D3D11_MAP_READ, 0, &ms);
		DEFORM_VERTEX_GPU* vertex = (DEFORM_VERTEX_GPU*)ms.pData;
		for (unsigned int v = 0; v < mesh->mNumVertices; v++) {
			m_DeformVertex[m][v] = vertex[v];
		}
		Renderer::GetDeviceContext()->Unmap(m_ReadBackBuffer[m], 0);

//*********************************************************************************

//***************************************************************************************
	//頂点バッファを更新
		D3D11_MAPPED_SUBRESOURCE ms2;
		Renderer::GetDeviceContext()->Map(m_VertexBuffer[m], 0, D3D11_MAP_WRITE_DISCARD, 0, &ms2);
		VERTEX_3D* ver = (VERTEX_3D*)ms2.pData;

		for (unsigned int v = 0; v < mesh->mNumVertices; v++) {
			ver[v].Position.x = m_DeformVertex[m][v].Position.x;
			ver[v].Position.y = m_DeformVertex[m][v].Position.y;
			ver[v].Position.z = m_DeformVertex[m][v].Position.z;
			ver[v].Normal.x = m_DeformVertex[m][v].Normal.x;
			ver[v].Normal.y = m_DeformVertex[m][v].Normal.y;
			ver[v].Normal.z = m_DeformVertex[m][v].Normal.z;
			ver[v].TexCoord.x = mesh->mTextureCoords[0][v].x;
			ver[v].TexCoord.y = mesh->mTextureCoords[0][v].y;

			ver[v].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);

			if (v == 20) {
				int b = 2;
			}
			
		}
		Renderer::GetDeviceContext()->Unmap(m_VertexBuffer[m],0);

//***************************************************************************************
	}
	//頂点変換(CPU　skinning)
	//for (unsigned int m = 0; m < m_AiScene->mNumMeshes; m++) {
	//	aiMesh* mesh = m_AiScene->mMeshes[m];

	//	D3D11_MAPPED_SUBRESOURCE ms;
	//	Renderer::GetDeviceContext()->Map(m_VertexBuffer[m], 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);

	//	VERTEX_3D * vertex = (VERTEX_3D*)ms.pData;
	//	for (unsigned int v = 0; v < mesh->mNumVertices; v++) {
	//		DEFORM_VERTEX* deformVertex = &m_DeformVertex[m][v];

	//		aiMatrix4x4 matrix[4];
	//		aiMatrix4x4 outMatrix;
	//		matrix[0] = m_Bone[deformVertex->BoneName[0]].Matrix;
	//		matrix[1] = m_Bone[deformVertex->BoneName[1]].Matrix;
	//		matrix[2] = m_Bone[deformVertex->BoneName[2]].Matrix;
	//		matrix[3] = m_Bone[deformVertex->BoneName[3]].Matrix;

	//		//ウェイトを考慮してマトリクス算出

	//		/*outMatrix.a1 = matrix[0]. * deformVertex->BoneWeight[0]
	//			+ matrix[1]. * deformVertex->BoneWeight[1]
	//			+ matrix[2]. * deformVertex->BoneWeight[2]
	//			+ matrix[3]. * deformVertex->BoneWeight[3];*/
	//		{
	//			outMatrix.a1 = matrix[0].a1 * deformVertex->BoneWeight[0]
	//				+ matrix[1].a1 * deformVertex->BoneWeight[1]
	//				+ matrix[2].a1 * deformVertex->BoneWeight[2]
	//				+ matrix[3].a1 * deformVertex->BoneWeight[3];

	//			outMatrix.a2 = matrix[0].a2 * deformVertex->BoneWeight[0]
	//				+ matrix[1].a2 * deformVertex->BoneWeight[1]
	//				+ matrix[2].a2 * deformVertex->BoneWeight[2]
	//				+ matrix[3].a2 * deformVertex->BoneWeight[3];

	//			outMatrix.a3 = matrix[0].a3 * deformVertex->BoneWeight[0]
	//				+ matrix[1].a3 * deformVertex->BoneWeight[1]
	//				+ matrix[2].a3 * deformVertex->BoneWeight[2]
	//				+ matrix[3].a3 * deformVertex->BoneWeight[3];

	//			outMatrix.a4 = matrix[0].a4 * deformVertex->BoneWeight[0]
	//				+ matrix[1].a4 * deformVertex->BoneWeight[1]
	//				+ matrix[2].a4 * deformVertex->BoneWeight[2]
	//				+ matrix[3].a4 * deformVertex->BoneWeight[3];

	//			outMatrix.b1 = matrix[0].b1 * deformVertex->BoneWeight[0]
	//				+ matrix[1].b1 * deformVertex->BoneWeight[1]
	//				+ matrix[2].b1 * deformVertex->BoneWeight[2]
	//				+ matrix[3].b1 * deformVertex->BoneWeight[3];
	//			outMatrix.b2 = matrix[0].b2 * deformVertex->BoneWeight[0]
	//				+ matrix[1].b2 * deformVertex->BoneWeight[1]
	//				+ matrix[2].b2 * deformVertex->BoneWeight[2]
	//				+ matrix[3].b2 * deformVertex->BoneWeight[3];
	//			outMatrix.b3 = matrix[0].b3 * deformVertex->BoneWeight[0]
	//				+ matrix[1].b3 * deformVertex->BoneWeight[1]
	//				+ matrix[2].b3 * deformVertex->BoneWeight[2]
	//				+ matrix[3].b3 * deformVertex->BoneWeight[3];
	//			outMatrix.b4 = matrix[0].b4 * deformVertex->BoneWeight[0]
	//				+ matrix[1].b4 * deformVertex->BoneWeight[1]
	//				+ matrix[2].b4 * deformVertex->BoneWeight[2]
	//				+ matrix[3].b4 * deformVertex->BoneWeight[3];

	//			outMatrix.c1 = matrix[0].c1 * deformVertex->BoneWeight[0]
	//				+ matrix[1].c1 * deformVertex->BoneWeight[1]
	//				+ matrix[2].c1 * deformVertex->BoneWeight[2]
	//				+ matrix[3].c1 * deformVertex->BoneWeight[3];
	//			outMatrix.c2 = matrix[0].c2 * deformVertex->BoneWeight[0]
	//				+ matrix[1].c2 * deformVertex->BoneWeight[1]
	//				+ matrix[2].c2 * deformVertex->BoneWeight[2]
	//				+ matrix[3].c2 * deformVertex->BoneWeight[3];
	//			outMatrix.c3 = matrix[0].c3 * deformVertex->BoneWeight[0]
	//				+ matrix[1].c3 * deformVertex->BoneWeight[1]
	//				+ matrix[2].c3 * deformVertex->BoneWeight[2]
	//				+ matrix[3].c3 * deformVertex->BoneWeight[3];
	//			outMatrix.c4 = matrix[0].c4 * deformVertex->BoneWeight[0]
	//				+ matrix[1].c4 * deformVertex->BoneWeight[1]
	//				+ matrix[2].c4 * deformVertex->BoneWeight[2]
	//				+ matrix[3].c4 * deformVertex->BoneWeight[3];

	//			outMatrix.d1 = matrix[0].d1 * deformVertex->BoneWeight[0]
	//				+ matrix[1].d1 * deformVertex->BoneWeight[1]
	//				+ matrix[2].d1 * deformVertex->BoneWeight[2]
	//				+ matrix[3].d1 * deformVertex->BoneWeight[3];
	//			outMatrix.d2 = matrix[0].d2 * deformVertex->BoneWeight[0]
	//				+ matrix[1].d2 * deformVertex->BoneWeight[1]
	//				+ matrix[2].d2 * deformVertex->BoneWeight[2]
	//				+ matrix[3].d2 * deformVertex->BoneWeight[3];
	//			outMatrix.d3 = matrix[0].d3 * deformVertex->BoneWeight[0]
	//				+ matrix[1].d3 * deformVertex->BoneWeight[1]
	//				+ matrix[2].d3 * deformVertex->BoneWeight[2]
	//				+ matrix[3].d3 * deformVertex->BoneWeight[3];
	//			outMatrix.d4 = matrix[0].d4 * deformVertex->BoneWeight[0]
	//				+ matrix[1].d4 * deformVertex->BoneWeight[1]
	//				+ matrix[2].d4 * deformVertex->BoneWeight[2]
	//				+ matrix[3].d4 * deformVertex->BoneWeight[3];
	//		}

	//		deformVertex->Position = mesh->mVertices[v];
	//		deformVertex->Position *= outMatrix;

	//		//法線変換用に移動成分を削除
	//		outMatrix.a4 = 0.0f;
	//		outMatrix.b4 = 0.0f;
	//		outMatrix.c4 = 0.0f;

	//		deformVertex->Normal = mesh->mNormals[v];
	//		deformVertex->Normal *= outMatrix;

	//		//頂点バッファへ書き込み
	//		vertex[v].Position.x = deformVertex->Position.x;
	//		vertex[v].Position.y = deformVertex->Position.y;
	//		vertex[v].Position.z = deformVertex->Position.z;

	//		vertex[v].Normal.x = deformVertex->Normal.x;
	//		vertex[v].Normal.y = deformVertex->Normal.y;
	//		vertex[v].Normal.z = deformVertex->Normal.z;

	//		vertex[v].TexCoord.x = mesh->mTextureCoords[0][v].x;
	//		vertex[v].TexCoord.y = mesh->mTextureCoords[0][v].y;

	//		vertex[v].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	//	}
	//	Renderer::GetDeviceContext()->Unmap(m_VertexBuffer[m], 0);
	//}
}


void AnimationModelByGPU::Draw()
{
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	material.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Renderer::SetMaterial(material);

	//Renderer::GetDeviceContext()->IASetInputLayout(m_InputLayout);

	//Renderer::GetDeviceContext()->VSSetShader(m_SkinVertexShader, NULL, 0);

	//Renderer::GetDeviceContext()->UpdateSubresource(m_ConstantSkinBuffer, 0, NULL, &m_Skinbuffer, 0, 0);

	for (unsigned int m = 0; m < m_AiScene->mNumMeshes; m++)
	{
		aiMesh* mesh = m_AiScene->mMeshes[m];

		aiMaterial* material = m_AiScene->mMaterials[mesh->mMaterialIndex];

		//テクスチャ設定
		aiString path;
		material->GetTexture(aiTextureType_DIFFUSE, 0, &path);
		Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture[path.data]);
		//頂点バッファ設定
		UINT stride = sizeof(VERTEX_3D);
		UINT offset = 0;
		Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer[m], &stride, &offset);

		Renderer::GetDeviceContext()->IASetIndexBuffer(m_IndexBuffer[m], DXGI_FORMAT_R32_UINT, 0);

		Renderer::GetDeviceContext()->DrawIndexed(mesh->mNumFaces * 3, 0, 0);
	}
}