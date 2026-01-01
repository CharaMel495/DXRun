#pragma once
#include "CaramelEngine.h"
#include "ITargetProvider.h"
#include "EnemyBase.h"
#include "EnemyDataTypeMaster.h"

namespace
{
	template<typename TEnemy>
	concept EnemyCreatable = std::is_base_of<EnemyBase, TEnemy>::value&& std::constructible_from<TEnemy, EnemyParam>;

	/*template<typename TShootingEnemy>
	concept ShootingEnemyCreatable = std::constructible_from<TShootingEnemy, EnemyParam, std::unique_ptr<IBulletShooter>>;*/
}

struct EnemyCreateData
{
	uint32_t enemyID;
	CVector3 pos;
	CVector3 dir;
};

/// <summary>
/// 敵管理クラス
/// </summary>
class EnemyManager final
{
public:

	inline EnemyManager() noexcept :
		_createID(0)
	{
	}

	std::shared_ptr<EnemyBase> createEnemy(EnemyCreateData data)
	{
		// TODO : 環境を整えたので、次は敵の生成を試してみる
		auto param = _masterData->getEnemyDataById(data.enemyID, data.pos, data.dir);

		param.name = "enemy_" + std::to_string(_createID);
		param.createID = _createID;
		param.enemyTransform->setScale(CVector3(70.0f, 70.0f, 0.0f));
		//// もし弾を撃つ機構を備えているなら
		//if (param.shootPattern.has_value())
		//	param.target = _playerManager->getPlayerTarget();

		// 選択された型を生成
		auto createdEnemy = std::make_shared<EnemyBase>(param);
		// ヒットイベントを作成
		createdEnemy->createEvents();
		// 初期化
		createdEnemy->initialize();
		// 現在のシーンに登録
		Engine::getInstance().getSceneManager().getCurrentScene()->addActor(createdEnemy);
		// アクティブな敵として登録
		_activeEnemys[_createID] = createdEnemy;
		// 重複を避けるためにインクリメント
		++_createID;

		// 一定を超えたらIDをリセット
		if (_createID > 1000000)
			_createID = 0;

		// 生成した敵を返却
		return createdEnemy;
	}

	void initialize();

	// 全ての敵の移動を行う関数
	void moveEnemy(float deltaTime) noexcept;

	void removeFromArrayByID(int id);

	// 破棄フラグが立っている敵を処理する関数
	void flushDeadEnemys();

	//void setPlayerManager(std::shared_ptr<PlayerManager> pm) noexcept { _playerManager = pm; }

	//MotionType getEnemyMotion(uint32_t motionID) const noexcept;

private:

	int _createID;

	/// <summary>
	/// 現在アクティブな弾
	/// </summary>
	std::unordered_map<int, std::shared_ptr<EnemyBase>> _activeEnemys;

	/// <summary>
	/// 敵の移動を管理するクラス
	/// </summary>
	/*std::unique_ptr<EnemyMover> _mover;

	std::shared_ptr<PlayerManager> _playerManager;*/

	/// <summary>
	/// 敵のマスターデータ
	/// </summary>
	std::unique_ptr<EnemyTypeMaster> _masterData;
};