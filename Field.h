#pragma once
#include "gameObject.h"
class Filed : public GameObject{
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	float GetHeight(D3DXVECTOR3 pos);
	
private:
	//D3DXVECTOR3 m_Position;
	//D3DXVECTOR3 m_Rotation;
	//D3DXVECTOR3 m_Scale;
	int sizeX = 0;
	int sizeY = 0;

	ID3D11Buffer*				m_VertexBuffer = NULL;
	ID3D11Buffer*               m_IndexBuffer = NULL;
	ID3D11ShaderResourceView*	m_Texture = NULL;

	VERTEX_3D* ve;

	ID3D11VertexShader*		m_VertexShader = NULL;
	ID3D11PixelShader*		m_PixelShader = NULL;
	ID3D11InputLayout*		m_VertexLayout = NULL;
};