#include "main.h"
//#include "base_scene.h"
#include "manager.h"
//#include "SceneMa.h"
#include "renderer.h"
#include "input.h"
#include "Scene.h"


Scene* Manager::m_Scene;

void Manager::Init()
{
	Renderer::Init();
	Input::Init();
	m_Scene = new Scene();
	m_Scene->Init();
	//SceneManager::SecneRegist();
	//SceneManager::Init();	
}


void Manager::Uninit()
{
	//SceneManager::Uninit();
	//SceneManager::SecneRelease();

	m_Scene->Uninit();
	delete m_Scene;

	Input::Uninit();
	Renderer::Uninit();
}

void Manager::Update()
{
	Input::Update();
	m_Scene->Update();
	//SceneManager::Update();
}

void Manager::Draw()
{
	Renderer::Begin();

	m_Scene->Draw();
	//SceneManager::Draw();

	Renderer::End();
}
