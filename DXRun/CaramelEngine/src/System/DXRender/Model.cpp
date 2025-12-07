//--------------------------------------------------------------------------------------
// File: Model.cpp
//
// E3D1 E23D1046 –{ŠÔ—F–ç
//--------------------------------------------------------------------------------------
#include "pch.h"
#include "Model.h"

using namespace DirectX;

namespace CaramelEngine
{
    //--------------------------------------------------------------------------------------
    Model::Model(std::shared_ptr<Transform> transform) :
        //_world(XMMatrixIdentity()),
        _transform(transform)
    {
    }

    //--------------------------------------------------------------------------------------
    HRESULT Model::CreateModel(LPCWSTR strFilename)
    {
        HRESULT hr = S_OK;

        // Load the mesh
        mesh = std::make_unique<MeshRenderer>();
        V_RETURN(mesh->CreateMesh(strFilename));

        return S_OK;
    }

    //--------------------------------------------------------------------------------------
    void Model::MoveModel(_In_opt_ std::function<XMMATRIX()> customFunction)
    {
        /*if (customFunction) 
        {
            _world = customFunction();
        }*/
    }

    //--------------------------------------------------------------------------------------
    void Model::RenderModel(
        _In_opt_ std::function<void(CXMMATRIX, const SDKMESH_MATERIAL*)> customFunction,
        const UINT textureSlot)
    {
        mesh->Render(_transform->getWorldMatrix(), customFunction, textureSlot);
    }
}