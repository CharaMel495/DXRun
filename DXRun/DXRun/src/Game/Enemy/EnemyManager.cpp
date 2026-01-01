#include "EnemyManager.h"

void EnemyManager::initialize()
{
	/*_mover = std::make_unique<EnemyMover>();
	_masterData = std::make_unique<EnemyTypeMaster>();

	_masterData->loadFromJson();*/
}

void EnemyManager::moveEnemy(float deltaTime) noexcept
{
	for (auto& [key, enemy] : _activeEnemys)
	{
		//_mover->move(enemy, enemy->getMotionType(), deltaTime);
	}
}

void EnemyManager::removeFromArrayByID(int id)
{
	// ”jŠüƒtƒ‰ƒO‚ª—§‚Á‚Ä‚¢‚é‚à‚Ì‚ðˆêŠ‡íœ
	std::erase_if(_activeEnemys, [](auto& pair) -> bool { return pair.second->getDestroyFlag(); });
}

void EnemyManager::flushDeadEnemys()
{
	int val = _activeEnemys.size();

	//”jŠüƒtƒ‰ƒO‚ª—§‚Á‚Ä‚¢‚½‚ç‚»‚ê‚ð”jŠü‚·‚é
	std::erase_if(_activeEnemys, [](auto& pair) {
		const auto& enemy = pair.second;

		if (enemy->getDestroyFlag())
		{
			Engine::getInstance().unregisterFromCollisionManager(enemy);
			Engine::getInstance().getSceneManager().getCurrentScene()->removeActor(enemy);

			return true;
		}

		return false;
		});
}