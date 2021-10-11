#include "main.h"
#include "renderer.h"
#include "Field.h"
#include "MapCreate.h"
//float g_FiledHeight[21][21] = {
//	{};
//}


void Filed::Init()
{
	

	sizeX = 250;
	sizeY = 250;

	CMapCreate* mapCretor = new CMapCreate(sizeX, sizeY);
	std::string FN = "asset/texture/xz.jpg";
	mapCretor->WorldMapMake(FN);
		
	float cellLengthX = 1.0f;
	float cellLengthY = 1.0f;

	float startX = -sizeX * 0.5f * cellLengthX;
	float startY = -sizeY * 0.5f * cellLengthY;
	//startX = -2.0f;
	//startY = -2.0f;

	ve = new VERTEX_3D[(sizeX + 1) * (sizeY + 1)];
	for (int i = 0; i < sizeY + 1; i++) {
		for (int j = 0; j < sizeX + 1; j++) {
			int index = i * (sizeX + 1) + j;
			float x = startX + j * cellLengthX;
			float y = startY + i * cellLengthY;
			ve[index].Position = D3DXVECTOR3(x, mapCretor->GetData(index), y);
			ve[index].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			ve[index].TexCoord = D3DXVECTOR2(j+0.5, i+0.5);
			ve[index].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		}
	}

	for (int i = 1; i < sizeY-1; i++) {
		for (int j = 1; j < sizeX-1; j++) {
			int index = i * (sizeX + 1) + j;
			D3DXVECTOR3 vx, vz, vn;
			int inb = index - 1;
			int inf = index + 1;
			int inu = index - (sizeX + 1);
			int ind = index + (sizeX + 1);

			vx = ve[inf].Position - ve[inb].Position;
			vz = ve[ind].Position - ve[inu].Position;

			D3DXVec3Cross(&vn, &vz, &vx);
			D3DXVec3Normalize(&vn, &vn);
			ve[index].Normal = vn;
		}
	}

	int indlen = sizeX * sizeY;
	unsigned int* ind = new  unsigned int[indlen * 6];
	for (int i = 0; i < sizeY; i++) {
		for (int j = 0; j < sizeX; j++) {
			int index = i * sizeX + j;
			ind[index * 6 + 0] = j + (sizeX + 1) * i;
			ind[index * 6 + 1] = j + (sizeX + 1) * (i + 1);
			ind[index * 6 + 2] = j + (sizeX + 1) * (i + 1) + 1;
			ind[index * 6 + 3] = j + (sizeX + 1) * i;
			ind[index * 6 + 4] = j + (sizeX + 1) * (i + 1) + 1;
			ind[index * 6 + 5] = j + (sizeX + 1) * i + 1;
		}
	}
	/*
	VERTEX_3D ver[21][21];
	for (int x = 0; x <= 20; x++) {
		for (int z = 0; z <= 20; z++) {
			ver[z][x].Position = D3DXVECTOR3((x - 10) * 5.0f, 0.0f, (z - 10) * -5.0f);
			ver[z][x].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			ver[z][x].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
			ver[z][x].TexCoord = D3DXVECTOR2(x * 0.5f, z * 0.5f);
		}
	}

	for (int x = 1; x < 19; x++) {
		for (int z = 1; z < 19; z++) {
			D3DXVECTOR3 vx, vz, vn;
			vx = ver[x + 1][z].Position - ver[x - 1][z].Position;
			vz = ver[x][z - 1].Position - ver[x][z + 1].Position;

			D3DXVec3Cross(&vn, &vz, &vx);
			D3DXVec3Normalize(&vn, &vn);
			ver[x][z].Normal = vn;
		}
	}*/
	//D3D11_BUFFER_DESC bd;
	//ZeroMemory(&bd, sizeof(bd));
	//bd.Usage = D3D11_USAGE_DEFAULT;
	//bd.ByteWidth = sizeof(VERTEX_3D) * 21 * 21;
	//bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	//bd.CPUAccessFlags = 0;

	//D3D11_SUBRESOURCE_DATA sd;
	//ZeroMemory(&sd, sizeof(sd));
	//sd.pSysMem = ver;

	//Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	//unsigned int index[(22 * 2) * 20 - 2];
	//int i = 0;
	//for (int x = 0; x < 20; x++) {
	//	for (int z = 0; z < 21; z++) {
	//		index[i] = x * 21 + z;
	//		i++;

	//		index[i] = (x + 1) * 21 + z;
	//		i++;
	//	}

	//	if (x == 19)break;

	//	//退縮ポリゴン
	//	index[i] = (x + 1) * 21 + 20;
	//	i++;

	//	index[i] = (x + 1) * 21;
	//	i++;
	//}

	//D3D11_BUFFER_DESC bd;
	//ZeroMemory(&bd, sizeof(bd));
	//bd.Usage = D3D11_USAGE_DEFAULT;
	//bd.ByteWidth = sizeof(unsigned int) * ((22 * 2) * 20 - 2);
	//bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	//bd.CPUAccessFlags = 0;

	//D3D11_BUFFER_DESC sd;
	//ZeroMemory(&sd, sizeof(sd));
	//sd.pSysMem = index;

	//Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_IndexBuffer);
	//
	/*VERTEX_3D vertex[4];

	vertex[0].Position = D3DXVECTOR3(-10.0f, 0.0f, 10.0f);
	vertex[0].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	vertex[1].Position = D3DXVECTOR3(10.0f, 0.0f, 10.0f);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(10.0f, 0.0f);

	vertex[2].Position = D3DXVECTOR3(-10.0f,  0.0f,-10.0f);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, 10.0f);

	vertex[3].Position = D3DXVECTOR3(10.0f, 0.0f, -10.0f);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = D3DXVECTOR2(10.0f, 10.0f);*/


	//頂点バッファ生成
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * (sizeX + 1) * (sizeY + 1);
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	//入れ込む頂点容器
	D3D11_SUBRESOURCE_DATA sd{};
	//sd.pSysMem = vertex;
	sd.pSysMem = ve;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);
	assert(m_VertexBuffer);

	//delete[] ve;

	D3D11_BUFFER_DESC ibd;
	ZeroMemory(&ibd, sizeof(ibd));
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.ByteWidth = sizeof(unsigned int) * sizeX * sizeY * 6;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA isd;
	ZeroMemory(&isd, sizeof(isd));
	isd.pSysMem = ind;

	Renderer::GetDevice()->CreateBuffer(&ibd, &isd, &m_IndexBuffer);
	assert(m_IndexBuffer);

	delete[] ind;

	//テクスチャ読み込め
	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
		"asset/texture/xd.jpg",
		NULL,
		NULL,
		&m_Texture,
		NULL);

	assert(m_Texture);

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "vertexLightingVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "vertexLightingPS.cso");

	m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
}

void Filed::Uninit()
{
	delete[] ve;
	m_VertexBuffer->Release();
	m_IndexBuffer->Release();
	m_Texture->Release();
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void Filed::Update()
{
}

void Filed::Draw()
{
	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout); //SVF相当

	//シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//マトリクス設定（Matrix）
	//Renderer::SetWorldViewProjection2D();
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.x, m_Rotation.y, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(&world);

	//頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);  //データサイズ
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);


	//マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	material.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Renderer::SetMaterial(material);

	//テクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	//プリミティブポロじ設定
	//Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	//Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	Renderer::GetDeviceContext()->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//ポリゴン描画
	Renderer::GetDeviceContext()->DrawIndexed(6 * sizeX * sizeY, 0, 0);
}

float Filed::GetHeight(D3DXVECTOR3 pos)
{
	int x, z;

	x = pos.x + sizeX * 0.5f;
	z = pos.z + sizeY * 0.5f;

	D3DXVECTOR3 pos0, pos1, pos2, pos3;
	pos0 = ve[x + z * (sizeX + 1)].Position;
	pos1 = ve[x + 1 + z * (sizeX + 1)].Position;
	pos2 = ve[x + (z + 1) * (sizeX + 1)].Position;
	pos3 = ve[x + 1 + (z + 1)* (sizeX + 1)].Position;

	D3DXVECTOR3 v12, v1p, c;
	v12 = pos2 - pos1;
	v1p = pos - pos1;

	D3DXVec3Cross(&c, &v12, &v1p);

	float py;
	D3DXVECTOR3 n;
	if (c.y > 0.0f) {
		D3DXVECTOR3 v10;
		v10 = pos0 - pos1;
		D3DXVec3Cross(&n, &v10, &v12);
	}
	else {
		D3DXVECTOR3 v13;
		v13 = pos3 - pos1;
		D3DXVec3Cross(&n, &v12, &v13);
	}
	py = -((pos.x - pos1.x) * n.x + (pos.z - pos1.z) * n.z) / n.y + pos1.y;

	return py;
}
