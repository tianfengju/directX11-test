#include "main.h"
#include "renderer.h"
#include "input.h"
#include "Camera.h"
#include "player.h"
#include "manager.h"
#include "Scene.h"

void Camera::Init()
{
	m_Position = D3DXVECTOR3(0.0f, 2.0f, -5.0f);
	m_Target = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

void Camera::Uninit()
{
}

void Camera::Update()
{
	/*if (Input::GetKeyPress('Q')) {
		m_Position.y -= 0.03f;
	}*/
	Player* player = Manager::GetScene()->GetGameObject<Player>();
	m_Target = player->GetPosition();

	//m_Position = m_Target + D3DXVECTOR3(0.0f, 5.0f, -8.0f);
	D3DXVECTOR3 foward = -player->GetForward();
	/*if(Input::GetKeyPress('W'))
		m_Position = m_Target - foward * 5.0f + D3DXVECTOR3(0.0f, 3.0f, 0.0f);*/
	

	m_Position = m_Target - foward * 5.0f + D3DXVECTOR3(0.0f, 3.0f, 0.0f);
}

void Camera::Draw()
{
	//ビューマトリクス
	D3DXMATRIX viewMatrix;
	D3DXMatrixLookAtLH(&viewMatrix, &m_Position, &m_Target, &D3DXVECTOR3(0.0f, 1.0f, 0.0f));

	Renderer::SetViewMatrix(&viewMatrix);

	//プロジェクション(投影)
	D3DXMATRIX projectionMatrix;
	D3DXMatrixPerspectiveFovLH(&projectionMatrix, 1.0f, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);

	Renderer::SetProjectionMatrix(&projectionMatrix);
}
