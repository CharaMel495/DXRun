#include "CaramelEngine.h"
#include "EnemyDataTypeMaster.h"
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <filesystem>

bool EnemyTypeMaster::loadFromJson()
{
    char exePath[MAX_PATH];
    GetModuleFileNameA(NULL, exePath, MAX_PATH);  // exe‚Ìƒtƒ‹ƒpƒX
    std::filesystem::path exeDir = std::filesystem::path(exePath).parent_path();

    std::filesystem::path streamingAssetsPath = exeDir / "StreamingAssets/JsonFiles/EnemyMasterTable.json";
    CString path = streamingAssetsPath.string();

    JsonReader reader;
    if (!reader.parseFromFile(path)) 
    {
        std::cerr << "Failed to parse enemy master: " << path << std::endl;
        return false;
    }

    const auto& root = reader.root();
    if (!root.hasKey("EnemyMasterTable")) 
    {
        std::cerr << "Invalid enemy master format: 'EnemyMasterTable' not found" << std::endl;
        return false;
    }

    const auto& array = root["EnemyMasterTable"];
    for (size_t i = 0; i < array.size(); ++i)
    {
        const auto& item = array[i];
        EnemyMasterDataReciever data;

        data.id = item["iD"].asInt();
        data.name = item["name"].asString();
        data.spriteId = item["spriteID"].asInt();
        data.hasDamage = item["hasDamage"].asInt();
        data.moveType = item["moveType"].asInt();
        data.moveSpeed = item["moveSpeed"].asNumber();
        data.colliderType = item["colliderType"].asInt();
        data.colliderSize = item["colliderSize"].asNumber();
        data.fireEnabled = item["fireEnabled"].asString() == "True";
        data.fireType = item["fireType"].asInt();
        data.bulletTableId = item["bulletTableID"].asInt();
        data.score = item["score"].asInt();
        data.deadEffectType = item["deadEffectType"].asInt();
        data.dropScoreItemValue = item["dropScoreItemValue"].asInt();
        data.dropPowerItemValue = item["dropPowerItemValue"].asInt();

        _table[data.id] = data;
    }

    return true;
}

const EnemyParam EnemyTypeMaster::getEnemyDataById(const uint32_t& iD, CVector3 pos, CVector3 startDir, std::shared_ptr<ITargetProvider> target) const
{
    auto it = _table.find(iD);
    if (it == _table.end()) 
        throw std::runtime_error("EnemyTypeMaster: Unknown enemy ID: " + iD);

    const EnemyMasterDataReciever recieveData = it->second;

    EnemyParam param;

    param.enemyTransform = std::make_shared<Transform>();
    param.enemyTransform->setPosition(pos);
    param.target = target;

    param.name = recieveData.name;
    param.spriteID = recieveData.spriteId;
    param.colliderSize = recieveData.colliderSize;
    param.hasDamage = recieveData.hasDamage;
    param.startDir = startDir;
    param.moveSpeed = recieveData.moveSpeed;
    param.fireEnabled = recieveData.fireEnabled;
    
    if (param.fireEnabled)
    {
        //param.shootPattern = getShootPattern(recieveData.fireType);
        param.useBulletID = recieveData.bulletTableId;
    }

    param.hasScore = recieveData.score;
    param.deadEffectID = recieveData.deadEffectType;
    param.dropScoreItemValue = recieveData.dropScoreItemValue;
    param.dropPowerItemValue = recieveData.dropPowerItemValue;

    return param;
}