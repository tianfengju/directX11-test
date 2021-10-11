#pragma once

#include<list>
#include <typeinfo>
#include"gameObject.h"
#include"Camera.h"
#include"Field.h"
#include"player.h"
#include"Enemy.h"
#include"Bullet.h"
#include"Polygon2D.h"

class Scene {

protected:
	std::list<GameObject*> m_GameObject;


public:
	Scene(){}
	virtual ~Scene(){}



	virtual void Init() {
		
		AddGameObject<Camera>();
		AddGameObject<Filed>();
		AddGameObject<Player>();
		
		AddGameObject<Enemy>()->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 4.0f));
		AddGameObject<Enemy>()->SetPosition(D3DXVECTOR3(3.0f, 0.0f, 4.0f));
		AddGameObject<Enemy>()->SetPosition(D3DXVECTOR3(-3.0f, 0.0f, 4.0f));

		//AddGameObject<Bullet>();

		//AddGameObject<Polygon2D>();
	}
	virtual void Uninit() {
		for (GameObject* object : m_GameObject) {
			object->Uninit();
			delete object;
		}

		m_GameObject.clear();
	}
	virtual void Update() {
		for (GameObject* object : m_GameObject) {
			object->Update();			
		}

		m_GameObject.remove_if(
			[](GameObject* object)
			{
				return object->Destroy(); 
			}
		);
	}
	virtual void Draw() {
		for (GameObject* object : m_GameObject) {
			object->Draw();
			
		}
	}
	template <typename T>
	T* AddGameObject() {
		T* gameObject = new T();
		gameObject->Init();
		m_GameObject.push_back(gameObject);
		return gameObject;
	}

	template<typename T>
	T* GetGameObject() {
		for (GameObject* object : m_GameObject) {
			if (typeid(*object) == typeid(T)) //å^Çí≤Ç◊ÇÈ(RTTI)ìÆìIå^èÓïÒ
			{
				return (T*)object;
			}
		}
		return NULL;
	}

	template<typename T>
	std::vector<T*> GetGameObjects() {
		std::vector<T*> objects;
		for (GameObject* object : m_GameObject) {
			if (typeid(*object) == typeid(T)) {
				objects.push_back((T*)object);
			}
		}
		return objects;
	}
};
