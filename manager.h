#pragma once


class Manager
{
private:
	static class Scene* m_Scene;


public:
	static void Init();
	static void Uninit();
	static void Update();
	static void Draw();

	static Scene* GetScene() { return m_Scene; }
};