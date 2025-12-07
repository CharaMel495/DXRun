#include "pch.h"
#include "Mesh.h"
// SdkmeshのFormat
//    { "POSITION",     0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // 0
//  //{ "BLENDWEIGHT",  0, DXGI_FORMAT_R8G8B8A8_UNORM,  0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // 1
//  //{ "BLENDINDICES", 0, DXGI_FORMAT_R8G8B8A8_UINT,   0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // 2
//    { "NORMAL",       0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // 3
//    { "TEXCOORD",     0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // 5
//  //{ "TANGENT",      0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // 6
//  //{ "BINORMAL",     0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // 7
//  //{ "COLOR",        0, DXGI_FORMAT_B8G8R8A8_UNORM,  0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // 10


using namespace DirectX;

namespace CaramelEngine
{
    
    //--------------------------------------------------------------------------------------
    // Constructor
    MeshRenderer::MeshRenderer() :
        _mesh(nullptr)
    {
    }

    //--------------------------------------------------------------------------------------
    // メッシュの作成
    HRESULT MeshRenderer::CreateMesh(LPCWSTR strFilename)
    {
        HRESULT hr = S_OK;
        auto device = DXUTGetD3D11Device();

        // メッシュの作成
        _mesh = std::make_unique<CDXUTSDKMesh>();
        if (!_mesh)
            return E_OUTOFMEMORY;
        // メッシュの読み込み
        V_RETURN(_mesh->Create(device, strFilename));

        // メッシュの頂点バッファを取得
        LoadVertexBuffer(strFilename);

        return hr;
    }

    //--------------------------------------------------------------------------------------
    // メッシュ描画
    void MeshRenderer::Render(CXMMATRIX world,
        _In_opt_ std::function<void(CXMMATRIX, const SDKMESH_MATERIAL*)> setCustomFunction, const UINT textureSlot)
    {
        if (_mesh) 
        {
            auto context = DXUTGetD3D11DeviceContext();

            UINT strides[1] = {};
            UINT offsets[1] = {};
            ID3D11Buffer* vb[1] = {};
            strides[0] = static_cast<UINT>(_mesh->GetVertexStride(0, 0));
            offsets[0] = 0;
            vb[0] = _mesh->GetVB11(0, 0);
            context->IASetVertexBuffers(0, 1, vb, strides, offsets);
            context->IASetIndexBuffer(_mesh->GetIB11(0), _mesh->GetIBFormat11(0), 0);

            for (UINT subsetId = 0; subsetId < _mesh->GetNumSubsets(0); ++subsetId) 
            {
                auto subset = _mesh->GetSubset(0, subsetId);
                auto material = _mesh->GetMaterial(subset->MaterialID);

                if (setCustomFunction)
                    setCustomFunction(world, material);

                auto primType = _mesh->GetPrimitiveType11(static_cast<SDKMESH_PRIMITIVE_TYPE>(subset->PrimitiveType));
                context->IASetPrimitiveTopology(primType);

                auto diffuseRV = material->pDiffuseRV11;
                if (diffuseRV != nullptr)
                    context->PSSetShaderResources(textureSlot, 1, &diffuseRV);
                else
                {
                    ID3D11ShaderResourceView* nullRV[] = { nullptr };
                    context->PSSetShaderResources(textureSlot, 1, nullRV);
                }

                context->DrawIndexed(
                    static_cast<UINT>(subset->IndexCount),
                    static_cast<UINT>(subset->IndexStart),
                    static_cast<UINT>(subset->VertexStart)
                );
            }
        }
    }

    //--------------------------------------------------------------------------------------
    // 交差判定(レイと三角形ポリゴン)
    bool MeshRenderer::Intersect(const XMVECTOR* pRayPos, const XMVECTOR* pRayDir,
        DWORD* pFaceIndex, float* pU, float* pV, float* pW, float* pDist)
    {
        bool hit = false;
        XMVECTOR a, b, c;
        XMVECTOR ab, ac, p, qp;
        XMVECTOR n, d;
        XMVECTOR ap, t;
        XMVECTOR e, v, w;
        float ood, tt, vv, ww, uu;

        for (auto i = 0; i < _vertexBuffer.size(); i++) 
        {
            a = XMLoadFloat3(reinterpret_cast<XMFLOAT3*>(_vertexBuffer[i++].get()));
            b = XMLoadFloat3(reinterpret_cast<XMFLOAT3*>(_vertexBuffer[i++].get()));
            c = XMLoadFloat3(reinterpret_cast<XMFLOAT3*>(_vertexBuffer[i].get()));

            ab = b - a;
            ac = c - a;
            p = *pRayPos;
            qp = -XMVector3Normalize(*pRayDir);

            n = XMVector3Cross(ab, ac);
            d = XMVector3Dot(qp, n);
            // 三角形の裏側に当たる方向で飛んでいる
            if (d.m128_f32[0] <= 0.0f) goto BACKSIDE;

            ap = p - a;
            t = XMVector3Dot(ap, n);
            // 三角形の裏側に光線の始点がある
            if (t.m128_f32[0] < 0.0f) goto BACKSIDE;

            // 光線が表から当たる時，始点は表にある
            if (d.m128_f32[0] > 0.0f && t.m128_f32[0] <= 0.0f) goto BACKSIDE;
            // 光線が裏から当たる時，始点は裏にある
            if (d.m128_f32[0] <= 0.0f && t.m128_f32[0] > 0.0f) goto BACKSIDE;

            e = XMVector3Cross(qp, ap);
            v = XMVector3Dot(ac, e);
            if (v.m128_f32[0] < 0.0f || v.m128_f32[0] > d.m128_f32[0]) goto BACKSIDE;
            w = -XMVector3Dot(ab, e);
            if (w.m128_f32[0] < 0.0f || v.m128_f32[0] + w.m128_f32[0] > d.m128_f32[0]) goto BACKSIDE;

            ood = 1.0f / d.m128_f32[0];
            tt = t.m128_f32[0] * ood;
            vv = v.m128_f32[0] * ood;
            ww = w.m128_f32[0] * ood;
            uu = 1.0f - vv - ww;

            if (tt < 0.0f) goto BACKSIDE;

            if (!hit) 
            {
                hit = true;
                *pDist = tt;
                *pU = uu;
                *pV = vv;
                *pW = ww;
                *pFaceIndex = i / 3;
            }
            else if (*pDist > tt) 
            {
                *pDist = tt;
                *pU = uu;
                *pV = vv;
                *pW = ww;
                *pFaceIndex = i / 3;
            }

        BACKSIDE:
            auto temp = b;
            b = c;
            c = temp;

            ab = b - a;
            ac = c - a;
            p = *pRayPos;
            qp = -XMVector3Normalize(*pRayDir);

            n = XMVector3Cross(ab, ac);
            d = XMVector3Dot(qp, n);
            // 三角形の裏側に当たる方向で飛んでいる
            if (d.m128_f32[0] <= 0.0f) 
                continue;

            ap = p - a;
            t = XMVector3Dot(ap, n);
            // 三角形の裏側に光線の始点がある
            if (t.m128_f32[0] < 0.0f) 
                continue;

            // 光線が表から当たる時，始点は表にある
            if (d.m128_f32[0] > 0.0f && t.m128_f32[0] <= 0.0f) 
                continue;
            // 光線が裏から当たる時，始点は裏にある
            if (d.m128_f32[0] <= 0.0f && t.m128_f32[0] > 0.0f) 
                continue;

            e = XMVector3Cross(qp, ap);
            v = XMVector3Dot(ac, e);
            if (v.m128_f32[0] < 0.0f || v.m128_f32[0] > d.m128_f32[0]) 
                continue;
            w = -XMVector3Dot(ab, e);
            if (w.m128_f32[0] < 0.0f || v.m128_f32[0] + w.m128_f32[0] > d.m128_f32[0]) continue;

            ood = 1.0f / d.m128_f32[0];
            tt = t.m128_f32[0] * ood;
            vv = v.m128_f32[0] * ood;
            ww = w.m128_f32[0] * ood;
            uu = 1.0f - vv - ww;

            if (tt < 0.0f) 
                continue;

            if (!hit) 
            {
                hit = true;
                *pDist = tt;
                *pU = uu;
                *pV = vv;
                *pW = ww;
                *pFaceIndex = i / 3;
            }
            else if (*pDist > tt) 
            {
                *pDist = tt;
                *pU = uu;
                *pV = vv;
                *pW = ww;
                *pFaceIndex = i / 3;
            }
        }

        return hit;
    }

    //--------------------------------------------------------------------------------------
    // 境界ボックスを計算
    void MeshRenderer::ComputeBoundingBox(XMVECTOR* min, XMVECTOR* max)
    {
        *min = g_XMFltMax;
        min->m128_f32[3] = 1.0f;
        *max = g_XMFltMin;
        max->m128_f32[3] = 1.0f;
        for (auto no = 0; no < _vertexBuffer.size(); ++no) 
        {
            auto pos = XMLoadFloat3(reinterpret_cast<XMFLOAT3*>(_vertexBuffer[no].get()));

            //*min = XMVectorMin(*min, pos);
            if (min->m128_f32[0] > pos.m128_f32[0])
                min->m128_f32[0] = pos.m128_f32[0];
            if (min->m128_f32[1] > pos.m128_f32[1])
                min->m128_f32[1] = pos.m128_f32[1];
            if (min->m128_f32[2] > pos.m128_f32[2])
                min->m128_f32[2] = pos.m128_f32[2];

            //*max = XMVectorMax(*max, pos);
            if (max->m128_f32[0] < pos.m128_f32[0])
                max->m128_f32[0] = pos.m128_f32[0];
            if (max->m128_f32[1] < pos.m128_f32[1])
                max->m128_f32[1] = pos.m128_f32[1];
            if (max->m128_f32[2] < pos.m128_f32[2])
                max->m128_f32[2] = pos.m128_f32[2];
        }
    }

    //--------------------------------------------------------------------------------------
    // 最少または最少に近い境界球を計算
    void MeshRenderer::ComputeBoundingSphere(XMVECTOR* center, float* radius)
    {
        // MostSeparatedPointsOnAABB
        int minx = 0, maxx = 0;
        int miny = 0, maxy = 0;
        int minz = 0, maxz = 0;
        for (auto no = 0; no < _vertexBuffer.size(); ++no) 
        {
            auto vertex = reinterpret_cast<XMFLOAT3*>(_vertexBuffer[no].get());

            if (vertex->x < reinterpret_cast<XMFLOAT3*>(_vertexBuffer[minx].get())->x) minx = no;
            if (vertex->x > reinterpret_cast<XMFLOAT3*>(_vertexBuffer[maxx].get())->x) maxx = no;
            if (vertex->y < reinterpret_cast<XMFLOAT3*>(_vertexBuffer[miny].get())->y) miny = no;
            if (vertex->y > reinterpret_cast<XMFLOAT3*>(_vertexBuffer[maxy].get())->y) maxy = no;
            if (vertex->z < reinterpret_cast<XMFLOAT3*>(_vertexBuffer[minz].get())->z) minz = no;
            if (vertex->z > reinterpret_cast<XMFLOAT3*>(_vertexBuffer[maxz].get())->z) maxz = no;
        }
        auto dist2x = XMLoadFloat3(reinterpret_cast<XMFLOAT3*>(_vertexBuffer[maxx].get())) - XMLoadFloat3(reinterpret_cast<XMFLOAT3*>(_vertexBuffer[minx].get()));
        dist2x = XMVector3Dot(dist2x, dist2x);
        auto dist2y = XMLoadFloat3(reinterpret_cast<XMFLOAT3*>(_vertexBuffer[maxy].get())) - XMLoadFloat3(reinterpret_cast<XMFLOAT3*>(_vertexBuffer[miny].get()));
        dist2y = XMVector3Dot(dist2y, dist2y);
        auto dist2z = XMLoadFloat3(reinterpret_cast<XMFLOAT3*>(_vertexBuffer[maxz].get())) - XMLoadFloat3(reinterpret_cast<XMFLOAT3*>(_vertexBuffer[minz].get()));;
        dist2z = XMVector3Dot(dist2z, dist2z);
        auto min = minx;
        auto max = maxx;
        if (dist2y.m128_f32[0] > dist2x.m128_f32[0] && dist2y.m128_f32[0] > dist2z.m128_f32[0]) 
        {
            min = miny;
            max = maxy;
        }
        if (dist2z.m128_f32[0] > dist2x.m128_f32[0] && dist2z.m128_f32[0] > dist2y.m128_f32[0]) 
        {
            min = minz;
            max = maxz;
        }

        auto c = (XMLoadFloat3(reinterpret_cast<XMFLOAT3*>(_vertexBuffer[min].get())) + XMLoadFloat3(reinterpret_cast<XMFLOAT3*>(_vertexBuffer[max].get()))) * 0.5f;
        auto r = XMVector3Length(XMLoadFloat3(reinterpret_cast<XMFLOAT3*>(_vertexBuffer[max].get())) - c).m128_f32[0];

        // SphereOfSphereAndPt
        for (auto no = 0; no < _vertexBuffer.size(); ++no) 
        {
            auto d = XMLoadFloat3(reinterpret_cast<XMFLOAT3*>(_vertexBuffer[no].get())) - c;
            auto dist = XMVector3Dot(d, d).m128_f32[0];
            if (dist > r * r) 
            {
                dist = sqrt(dist);
                auto rNew = (r + dist) * 0.5f;
                auto k = (rNew - r) / dist;
                r = rNew;
                c += d * k;
            }
        }
        *center = c;
        *radius = r;
    }

    //--------------------------------------------------------------------------------------
    // 頂点バッファの読み込み
    HRESULT MeshRenderer::LoadVertexBuffer(LPCWSTR strFilename)
    {
        HRESULT hr = S_OK;

        // Find the path for the file
        WCHAR strPathW[MAX_PATH];
        V_RETURN(DXUTFindDXSDKMediaFileCch(strPathW, sizeof(strPathW) / sizeof(WCHAR), strFilename));

        // Open the file
        HANDLE hFile;
        hFile = CreateFile(strPathW, FILE_READ_DATA, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, nullptr);
        if (INVALID_HANDLE_VALUE == hFile)
            return DXUTERR_MEDIANOTFOUND;

        // Get the file size
        LARGE_INTEGER FileSize;
        GetFileSizeEx(hFile, &FileSize);
        UINT cBytes = FileSize.LowPart;

        // Allocate memory
        BYTE* pStaticMeshData;
        pStaticMeshData = new (std::nothrow) BYTE[cBytes];
        if (!pStaticMeshData) 
        {
            CloseHandle(hFile);
            return E_OUTOFMEMORY;
        }
        // Read in the file
        DWORD dwBytesRead;
        if (!ReadFile(hFile, pStaticMeshData, cBytes, &dwBytesRead, nullptr))
            hr = E_FAIL;

        CloseHandle(hFile);

        // Pointer fixup
        SDKMESH_HEADER* pMeshHeader;
        SDKMESH_VERTEX_BUFFER_HEADER* pVertexBufferArray;
        SDKMESH_INDEX_BUFFER_HEADER* pIndexBufferArray;
        SDKMESH_MESH* pMeshArray;
        SDKMESH_SUBSET* pSubsetArray;
        pMeshHeader = reinterpret_cast<SDKMESH_HEADER*>(pStaticMeshData);
        pVertexBufferArray = (SDKMESH_VERTEX_BUFFER_HEADER*)(pStaticMeshData +
            pMeshHeader->VertexStreamHeadersOffset);
        pIndexBufferArray = (SDKMESH_INDEX_BUFFER_HEADER*)(pStaticMeshData +
            pMeshHeader->IndexStreamHeadersOffset);
        pMeshArray = (SDKMESH_MESH*)(pStaticMeshData + pMeshHeader->MeshDataOffset);
        pSubsetArray = (SDKMESH_SUBSET*)(pStaticMeshData + pMeshHeader->SubsetDataOffset);

        // Setup subsets
        for (UINT i = 0; i < pMeshHeader->NumMeshes; i++) 
        {
            pMeshArray[i].pSubsets = (UINT*)(pStaticMeshData + pMeshArray[i].SubsetOffset);
            pMeshArray[i].pFrameInfluences = (UINT*)(pStaticMeshData + pMeshArray[i].FrameInfluenceOffset);
        }

        // Setup buffer data pointer
        BYTE* pBufferData = pStaticMeshData + pMeshHeader->HeaderSize + pMeshHeader->NonBufferDataSize;

        // Get the start of the buffer data
        UINT64 BufferDataStart = pMeshHeader->HeaderSize + pMeshHeader->NonBufferDataSize;

        // Create VBs
        BYTE** ppVertices;
        ppVertices = new (std::nothrow) BYTE * [pMeshHeader->NumVertexBuffers];
        if (!ppVertices) 
        {
            return E_OUTOFMEMORY;
        }
        for (UINT i = 0; i < pMeshHeader->NumVertexBuffers; i++) 
        {
            BYTE* pVertices = nullptr;
            pVertices = (BYTE*)(pBufferData + (pVertexBufferArray[i].DataOffset - BufferDataStart));

            ppVertices[i] = pVertices;
        }

        // Create IBs
        BYTE** ppIndices;
        ppIndices = new (std::nothrow) BYTE * [pMeshHeader->NumIndexBuffers];
        if (!ppIndices) 
            return E_OUTOFMEMORY;
        for (UINT i = 0; i < pMeshHeader->NumIndexBuffers; i++) 
        {
            BYTE* pIndices = nullptr;
            pIndices = (BYTE*)(pBufferData + (pIndexBufferArray[i].DataOffset - BufferDataStart));

            ppIndices[i] = pIndices;
        }

        SDKMESH_SUBSET* pSubset = nullptr;

        SDKMESH_MESH* currentMesh = &pMeshArray[0];
        for (UINT meshi = 0; meshi < pMeshHeader->NumMeshes; ++meshi) 
        {
            currentMesh = &pMeshArray[meshi];
            INT indsize;
            if (pIndexBufferArray[currentMesh->IndexBuffer].IndexType == IT_16BIT) 
                indsize = 2;
            else
                indsize = 4;

            for (UINT subset = 0; subset < currentMesh->NumSubsets; subset++) 
            {
                pSubset = &pSubsetArray[currentMesh->pSubsets[subset]];

                UINT IndexCount = (UINT)pSubset->IndexCount;
                UINT IndexStart = (UINT)pSubset->IndexStart;

                UINT* ind = (UINT*)ppIndices[currentMesh->IndexBuffer];
                float* verts = (float*)ppVertices[currentMesh->VertexBuffers[0]];
                UINT stride = (UINT)pVertexBufferArray[currentMesh->VertexBuffers[0]].StrideBytes;
                stride /= 4;
                for (UINT vertind = IndexStart; vertind < IndexStart + IndexCount; ++vertind) {
                    UINT current_ind = 0;
                    if (indsize == 2) 
                    {
                        UINT ind_div2 = vertind / 2;
                        current_ind = ind[ind_div2];
                        if (vertind % 2 == 0) 
                        {
                            current_ind = current_ind << 16;
                            current_ind = current_ind >> 16;
                        }
                        else 
                        {
                            current_ind = current_ind >> 16;
                        }
                    }
                    else
                        current_ind = ind[vertind];

                    XMFLOAT3* pt = (XMFLOAT3*)&(verts[stride * current_ind]);
                    auto p = std::make_unique<XMFLOAT3>(*pt);

                    _vertexBuffer.emplace_back(std::move(p));
                }
            }
        }

        SAFE_DELETE_ARRAY(ppVertices);
        SAFE_DELETE_ARRAY(ppIndices);
        SAFE_DELETE_ARRAY(pStaticMeshData);

        return S_OK;
    }
}