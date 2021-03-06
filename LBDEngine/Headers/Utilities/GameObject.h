#pragma once
#include "Utilities.h"
#include "../Behaviours/Behaviour.h"

class Behaviour;

//An empty object in the world that contains a position, scale, and rotation
class GameObject
{
public:
	//Calls each behaviour's start method
	void Start();

	//Calls each behaviour's update method
	//Called each frame.
	void Update();

	/// <summary>
	/// Fixed time step.
	/// Unlike Update, which is called once per frame, this is called at a consistent rate regardless of framerate.
	/// Mainly used for physics updates.
	/// </summary>
	void FixedUpdate();

	template<typename T>
	T* GetBehaviour()
	{
		for (auto& behaviour : _behaviours)
		{
			auto casted{ dynamic_cast<T*>(behaviour.get()) };
			if (casted)
			{
				return casted;
			}
		}

		return nullptr;
	}

	/*
	* Add a behaviour of type T.
	* This is the only correct way to create a behaviour, as a behaviour
	* should not exist without being attached to a GameObject.
	* This is because behaviours have a pointer back to their own GameObject.
	* To prevent bugs, users should not be able to access a null GameObject pointer.
	*/
	template<typename T>
	Behaviour* AddBehaviour()
	{
		auto behaviour{ std::make_unique<T>() };
		behaviour->SetGameObject(this);
		_behaviours.push_back(std::move(behaviour));

		return _behaviours.back().get();
	}

	XMFLOAT4X4& GetScale()
	{
		return _scale;
	}

	void SetScale(XMMATRIX scale)
	{
		XMStoreFloat4x4(&_scale, scale);
	}
	
	XMFLOAT4X4& GetRotation()
	{
		return _rotation;
	}
	
	void SetRotation(XMMATRIX rotation)
	{
		XMStoreFloat4x4(&_rotation, rotation);
	}

	XMFLOAT4X4& GetTranslation()
	{
		return _translation;
	}

	void SetTranslation(XMMATRIX translation)
	{
		XMStoreFloat4x4(&_translation, translation);
	}

	XMMATRIX GetWorldTransform()
	{
		return XMMatrixMultiply(XMMatrixMultiply(XMLoadFloat4x4(&_scale), XMLoadFloat4x4(&_rotation)), XMLoadFloat4x4(&_translation));
	}

private:
	XMFLOAT4X4 _scale{ MathHelper::CreateIdentity4x4() };
	XMFLOAT4X4 _rotation{ MathHelper::CreateIdentity4x4() };
	XMFLOAT4X4 _translation{ MathHelper::CreateIdentity4x4() };

	//A list of single purpose objects that provide functionality to a game object.
	std::vector<std::unique_ptr<Behaviour>> _behaviours;

};
