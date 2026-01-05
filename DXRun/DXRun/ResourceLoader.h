#pragma once
#include <Windows.h>
#include <	filesystem>
#include <fstream>
#include "resource.h"

HRSRC LoadEmbeddedResource(
    int resourceId,
    const void*& outData,
    size_t& outSize)
{
    HRSRC hRes = FindResource(nullptr, MAKEINTRESOURCE(resourceId), RT_RCDATA);
    if (!hRes) 
        return hRes;

    HGLOBAL hMem = LoadResource(nullptr, hRes);
    if (!hMem) 
        return hRes;

    outSize = SizeofResource(nullptr, hRes);
    outData = LockResource(hMem);

    return hRes;
}

void EnsureDirectory(const std::filesystem::path& dir)
{
    std::filesystem::create_directories(dir);
}

HRSRC WriteFileFromResource(
    int resourceId,
    const std::filesystem::path& outPath)
{
    const void* data;
    size_t size;

    HRSRC hRes = LoadEmbeddedResource(resourceId, data, size);

    if (!hRes)
        return hRes;

    EnsureDirectory(outPath.parent_path());

    std::ofstream ofs(outPath, std::ios::binary);
    ofs.write(reinterpret_cast<const char*>(data), size);

    return hRes;
}

HRSRC ExtractAssetsToTemp()
{
    HRSRC hRes = WriteFileFromResource(
        IDR_MODEL_CUBE,
        "temp//Models//Test//cube.mv1"
    );

    if (!hRes)
        return hRes;

    hRes = WriteFileFromResource(
        IDR_MODEL_SPHERE,
        "temp//Models//Test//sphere.mv1"
    );

    if (!hRes)
        return hRes;

    hRes = WriteFileFromResource(
        IDR_MODEL_STAGE,
        "temp//Models//Test//testStageA.mv1"
    );

    if (!hRes)
        return hRes;

    hRes = WriteFileFromResource(
        IDR_DATA_ENEMY,
        "temp//Params//JsonFiles//EnemyMasterTable.json"
    );

    if (!hRes)
        return hRes;

    return hRes;
}