#include "main.h"
#include "renderer.h"
#include "Cube.h"


void Cube::Init()
{
	VERTEX_3D vertex[24];
	
	{
		//上面
		vertex[0].Position = D3DXVECTOR3(-0.5f, 0.5f, 0.5f);
		vertex[0].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

		vertex[1].Position = D3DXVECTOR3(0.5f, 0.5f, 0.5f);
		vertex[1].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

		vertex[2].Position = D3DXVECTOR3(-0.5f, 0.5f, -0.5f);
		vertex[2].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

		vertex[3].Position = D3DXVECTOR3(0.5f, 0.5f, -0.5f);
		vertex[3].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);

		//前面
		vertex[4].Position = D3DXVECTOR3(-0.5f, 0.5f, -0.5f);
		vertex[4].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		vertex[4].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[4].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

		vertex[5].Position = D3DXVECTOR3(0.5f, 0.5f, -0.5f);
		vertex[5].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		vertex[5].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[5].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

		vertex[6].Position = D3DXVECTOR3(-0.5f, -0.5f, -0.5f);
		vertex[6].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		vertex[6].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[6].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

		vertex[7].Position = D3DXVECTOR3(0.5f, -0.5f, -0.5f);
		vertex[7].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		vertex[7].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[7].TexCoord = D3DXVECTOR2(1.0f, 1.0f);

		//右
		vertex[8].Position = D3DXVECTOR3(0.5f, 0.5f, -0.5f);
		vertex[8].Normal = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		vertex[8].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[8].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

		vertex[9].Position = D3DXVECTOR3(0.5f, 0.5f, 0.5f);
		vertex[9].Normal = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		vertex[9].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[9].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

		vertex[10].Position = D3DXVECTOR3(0.5f, -0.5f, -0.5f);
		vertex[10].Normal = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		vertex[10].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[10].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

		vertex[11].Position = D3DXVECTOR3(0.5f, -0.5f, 0.5f);
		vertex[11].Normal = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		vertex[11].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[11].TexCoord = D3DXVECTOR2(1.0f, 1.0f);

		//下
		vertex[12].Position = D3DXVECTOR3(-0.5f, -0.5f, -0.5f);
		vertex[12].Normal = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		vertex[12].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[12].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

		vertex[13].Position = D3DXVECTOR3(0.5f, -0.5f, -0.5f);
		vertex[13].Normal = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		vertex[13].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[13].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

		vertex[14].Position = D3DXVECTOR3(-0.5f, -0.5f, 0.5f);
		vertex[14].Normal = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		vertex[14].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[14].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

		vertex[15].Position = D3DXVECTOR3(0.5f,-0.5f, 0.5f);
		vertex[15].Normal = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		vertex[15].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[15].TexCoord = D3DXVECTOR2(1.0f, 1.0f);

		//後ろ
		vertex[16].Position = D3DXVECTOR3(0.5f, 0.5f, 0.5f);
		vertex[16].Normal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		vertex[16].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[16].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

		vertex[17].Position = D3DXVECTOR3(-0.5f, 0.5f, 0.5f);
		vertex[17].Normal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		vertex[17].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[17].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

		vertex[18].Position = D3DXVECTOR3(0.5f, -0.5f, 0.5f);
		vertex[18].Normal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		vertex[18].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[18].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

		vertex[19].Position = D3DXVECTOR3(-0.5f, -0.5f, 0.5f);
		vertex[19].Normal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		vertex[19].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[19].TexCoord = D3DXVECTOR2(1.0f, 1.0f);

		//左
		vertex[20].Position = D3DXVECTOR3(-0.5f, 0.5f, 0.5f);
		vertex[20].Normal = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
		vertex[20].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[20].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

		vertex[21].Position = D3DXVECTOR3(-0.5f, 0.5f, -0.5f);
		vertex[21].Normal = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
		vertex[21].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[21].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

		vertex[22].Position = D3DXVECTOR3(-0.5f, -0.5f, 0.5f);
		vertex[22].Normal = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
		vertex[22].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[22].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

		vertex[23].Position = D3DXVECTOR3(-0.5f, -0.5f, -0.5f);
		vertex[23].Normal = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
		vertex[23].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[23].TexCoord = D3DXVECTOR2(1.0f, 1.0f);
	}


	//頂点バッファ生成
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * 24;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	//入れ込む頂点容器
	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);


	/*D3D11_BUFFER_DESC inbd{};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof()*/

	//テクスチャ読み込め
	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
		"asset/texture/xz.jpg",
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

void Cube::Uninit()
{
	m_VertexBuffer->Release();
	m_Texture->Release();
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void Cube::Update()
{
}

void Cube::Draw()
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
	//Renderer::GetDeviceContext()->IASetIndexBuffer()
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);


	//マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Renderer::SetMaterial(material);

	//テクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	//プリミティブポロじ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//ポリゴン描画
	Renderer::GetDeviceContext()->Draw(24, 0);
}

void Cube::SetScale(D3DXVECTOR3 scale)
{
	m_Scale = scale;
}

void Cube::SetPosition(D3DXVECTOR3 position)
{
	m_Position = position;
}

void Cube::SetRotation(D3DXVECTOR3 rotation)
{
	m_Rotation = rotation;
}
