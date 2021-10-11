#pragma once
//#include "model.h"
#include "gameObject.h"

class Player : public GameObject{

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
	//Model* m_Model;
	class AnimationModelByGPU* m_Model;
	class AnimationModel* m_Model2;
	int m_Frame = 0;
	float m_Blend = 0.0f;
	//ID3D11Buffer*				m_VertexBuffer = NULL;
	//ID3D11Buffer*               m_IndexBuffer = NULL;
	//ID3D11ShaderResourceView*	m_Texture = NULL;

	ID3D11VertexShader*		m_VertexShader = NULL;
	ID3D11PixelShader*		m_PixelShader = NULL;
	ID3D11InputLayout*		m_VertexLayout = NULL;
};