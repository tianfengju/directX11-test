#pragma once
#include "gameObject.h"

class Camera :public  GameObject{
public:
	
	void Init();
	void Uninit();
	void Update();
	void Draw();

public:
	//D3DXVECTOR3 position;
	//D3DMATRIX matrix;
	//D3DXVECTOR3 up;
	//D3DXVECTOR3 foward;
	//D3DXVECTOR3 right;

	//D3DXVECTOR3 m_Position;
	D3DXVECTOR3 m_Target;
};