#include "pch.h"
#include "CModel.h"
#include "GraphicsDevice.h"
#include "Definition.h"

#include <filesystem>

using namespace CaramelEngine;
using Microsoft::WRL::ComPtr;

bool CModel::loadFromFile(GraphicsDevice& gfx, const std::string& path)
{
    Assimp::Importer importer;

    const unsigned int flags =
        aiProcess_Triangulate |
        aiProcess_GenNormals |
        aiProcess_CalcTangentSpace |
        aiProcess_JoinIdenticalVertices |
        aiProcess_ImproveCacheLocality |
        aiProcess_FlipUVs;

    const aiScene* scene = importer.ReadFile(path, flags);

    if (!scene || !scene->mRootNode)
    {
#ifdef _DEBUG
        OutputDebugStringA(importer.GetErrorString());
#endif
        return false;
    }

    _meshes.clear();

    return processAssimpScene(gfx, scene);
}

bool CModel::loadFromDirectory(
    GraphicsDevice& gfx,
    const std::string& directory,
    const std::string& filename)
{
    std::filesystem::path fullPath = directory;
    fullPath /= filename;

    return loadFromFile(gfx, fullPath.string());
}

bool CModel::processAssimpScene(
    GraphicsDevice& gfx,
    const aiScene* scene)
{
    for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
    {
        processAssimpMesh(gfx, scene->mMeshes[i]);
    }

    return !_meshes.empty();
}

void CModel::processAssimpMesh(GraphicsDevice& gfx, const aiMesh* mesh)
{
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    vertices.reserve(mesh->mNumVertices);

    for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
    {
        Vertex v{};

        // position
        v.position = {
            mesh->mVertices[i].x,
            mesh->mVertices[i].y,
            mesh->mVertices[i].z
        };

        // normal
        if (mesh->HasNormals())
        {
            v.normal = {
                mesh->mNormals[i].x,
                mesh->mNormals[i].y,
                mesh->mNormals[i].z
            };
        }
        else
        {
            v.normal = { 0.0f, 1.0f, 0.0f };
        }

        // uviAssimp‚Í•¡”UVŽ‚Ä‚é‚ªA‚Ü‚¸‚Í0”Ôj
        if (mesh->HasTextureCoords(0))
        {
            v.uv = {
                mesh->mTextureCoords[0][i].x,
                mesh->mTextureCoords[0][i].y
            };
        }
        else
        {
            v.uv = { 0.0f, 0.0f };
        }

        vertices.emplace_back(v);
    }

    // indices
    for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
    {
        const aiFace& face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; ++j)
        {
            indices.emplace_back(face.mIndices[j]);
        }
    }

    // --- GPU Buffer ì¬ ---
    CMesh outMesh{};

    // Vertex Buffer
    {
        D3D11_BUFFER_DESC desc{};
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.ByteWidth = UINT(sizeof(Vertex) * vertices.size());
        desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

        D3D11_SUBRESOURCE_DATA init{};
        init.pSysMem = vertices.data();

        gfx.device().createBuffer(
            &desc,
            &init,
            outMesh.vertexBuffer.ReleaseAndGetAddressOf()
        );
    }

    // Index Buffer
    {
        D3D11_BUFFER_DESC desc{};
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.ByteWidth = UINT(sizeof(uint32_t) * indices.size());
        desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

        D3D11_SUBRESOURCE_DATA init{};
        init.pSysMem = indices.data();

        gfx.device().createBuffer(
            &desc,
            &init,
            outMesh.indexBuffer.ReleaseAndGetAddressOf()
        );
    }

    outMesh.indexCount = static_cast<uint32_t>(indices.size());

    _meshes.emplace_back(std::move(outMesh));
}
