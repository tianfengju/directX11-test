#pragma once
#include "gameObject.h"
#include "model.h"

class Enemy : public GameObject {
public:
	//Cube(float size);
	void Init();
	void Uninit();
	void Update();
	void Draw();

	//void SetScale(D3DXVECTOR3 scale);
	//void SetPosition(D3DXVECTOR3 position);
	//void SetRotation(D3DXVECTOR3 rotation);
private:
	/*D3DXVECTOR3 m_Position;
	D3DXVECTOR3 m_Rotation;
	D3DXVECTOR3 m_Scale;
*/
	Model* m_Model;

	//ID3D11Buffer*				m_VertexBuffer = NULL;
	//ID3D11Buffer*               m_IndexBuffer = NULL;
	//ID3D11ShaderResourceView*	m_Texture = NULL;

	ID3D11VertexShader*		m_VertexShader = NULL;
	ID3D11PixelShader*		m_PixelShader = NULL;
	ID3D11InputLayout*		m_VertexLayout = NULL;
};