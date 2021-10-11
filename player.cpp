#include "main.h"

#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "AnimationModel.h"
#include "AnimationModelByGPU.h"
#include "player.h"
#include "Scene.h"

float offrot = 3.14f;

void Player::Init()
{
	m_Blend = 0.0f;
	m_Model = new AnimationModelByGPU();
	//m_Model2 = new AnimationModel();
	m_Model->Load("asset\\model\\Akai_Idle.fbx");
	m_Model->LoadAnimation("asset\\model\\Akai_Idle.fbx", "Idle");
	m_Model->LoadAnimation("asset\\model\\Akai_Run.fbx", "Run");

	//m_Model2->Load("asset\\model\\Akai_Idle.fbx");
	//m_Model2->LoadAnimation("asset\\model\\Akai_Idle.fbx", "Idle");
	//m_Model2->LoadAnimation("asset\\model\\Akai_Run.fbx", "Run");
	//m_Model->Load("asset\\model\\Spider.fbx");

	m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(0.01f, 0.01f, 0.01f);

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "vertexLightingVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "vertexLightingPS.cso");
}

void Player::Uninit()
{
	m_Model->Unload();
	delete m_Model;
	

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void Player::Update()
{
	////if (Input::GetKeyPress(VK_SHIFT))
	//if (Input::GetKeyPress('A')) {
	//	//m_Position.x -= 0.1f;
	//}

	//if (Input::GetKeyPress('D')) {
	//	//m_Position.x += 0.1f;
	//}

	if (Input::GetKeyPress('Q')) {
		m_Rotation.x += 0.1f;
	}

	if (Input::GetKeyTrigger(VK_SPACE)) {
		Bullet* b = Manager::GetScene()->AddGameObject<Bullet>();
		b->SetPosition(m_Position);
	}

	m_Frame++;
	

	Filed* f = Manager::GetScene()->GetGameObject<Filed>();
	m_Position.y = f->GetHeight(m_Position);



	if (Input::GetKeyPress('S')) {
		
		m_Model->Update("Idle" ,"Run", m_Blend,m_Frame);
		//m_Model->Update("Run", m_Frame);
		//m_Rotation.x = 0.0f;
		D3DXVECTOR3 foward = -GetForward();
		m_Position -= foward * 0.06f;
		m_Blend += 0.05f;
	}
	else if (Input::GetKeyPress('W')) {
		
		m_Model->Update("Idle", "Run", m_Blend, m_Frame);
		//m_Rotation.x = 3.14f;
		D3DXVECTOR3 foward = -GetForward();
		m_Position += foward * 0.06f;
		m_Blend += 0.05f;
	}
	else {

		m_Model->Update("Idle", "Run", m_Blend, m_Frame);
		m_Blend -= 0.05f;
		//m_Model->Update("Idle", m_Frame);
	}

	if (Input::GetKeyPress('A')) {
		
		//m_Model->Update("Idle", "Run", m_Blend, m_Frame);
		m_Rotation.x -= 0.03f;
		//m_Position.x -= 0.06f;
		//m_Blend += 0.05f;
	}
	if (Input::GetKeyPress('D')) {
		
		//m_Model->Update("Idle", "Run", m_Blend, m_Frame);
		m_Rotation.x += 0.03f;
		//m_Position.x += 0.06f;
		//m_Blend += 0.05f;
	}
	
	
	
	if (m_Blend > 1.0f)m_Blend = 1.0f;
	if (m_Blend < 0.0f)m_Blend = 0.0f;
}

void Player::Draw()
{
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

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

	m_Model->Draw();
	//m_Model2->Draw();
}
