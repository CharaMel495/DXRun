#include "pch.h"

namespace CaramelEngine
{
	void Scene::addActor(std::shared_ptr<Actor> newActor) noexcept
	{
		//ìoò^ë“ÇøÇÃÉLÉÖÅ[Ç…í«â¡
		_pendingAdditions.emplace_back(newActor);
	}

	void Scene::removeActor(const std::shared_ptr<Actor>& actor) noexcept
	{
		_pendingRemovals.emplace_back(actor);
	}

	void Scene::flushPendingActors() noexcept
	{
		if (_pendingAdditions.empty()) 
			return;

		_actors.insert(_actors.end(), _pendingAdditions.begin(), _pendingAdditions.end());
		_pendingAdditions.clear();
	}

	void Scene::flushPendingRemovals() noexcept
	{
		for (const auto& actor : _pendingRemovals)
		{
			auto it = std::remove(_actors.begin(), _actors.end(), actor);
			if (it != _actors.end())
			{
				_actors.erase(it, _actors.end());
			}
		}
		_pendingRemovals.clear();
	}
}