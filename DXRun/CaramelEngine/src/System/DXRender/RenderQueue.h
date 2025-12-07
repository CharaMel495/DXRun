#pragma once
#include <wrl/client.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <array>
#include "Mesh.h"
#include "Material.h"

namespace CaramelEngine
{
	struct SpriteVertex
	{
		DirectX::XMFLOAT4 position;
		DirectX::XMFLOAT2 uv;
		DirectX::XMFLOAT4 color;
	};

	struct SpriteVSConstant
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX viewProj;
		DirectX::XMFLOAT2 uvOffset;
		DirectX::XMFLOAT2 uvScale;
		DirectX::XMFLOAT4 color;
	};

	enum class RenderPassType : uint8_t 
	{
		DepthPrepass,
		Opaque,
		Transparent,
		Sprite,
		UI,
		Count
	};

	/// <summary>
	/// ゲーム内のあらゆるオブジェクトがこの構造体を
	/// </summary>
	struct RenderCommand
	{
		RenderPassType pass = RenderPassType::Opaque;

		// --- 3D用 ---
		MeshRenderer* meshPtr = nullptr;

		// --- 共通 (3D/2D両対応) ---
		Material* materialPtr = nullptr;   // Sprite も必ず Material を通す前提
		float depth = 0.0f;
		uint64_t sortKey = 0;

		// --- 2D Sprite専用 ---
		CVector3 position{ 0, 0, 0 };      // ワールド上の描画位置
		CVector3 scale{ 1, 1, 1 };         // 拡縮
		float rotation = 0.0f;             // Z軸回転（ラジアン）
		Color color = Color::White();      // 乗算カラー

		// UV情報（仮想アトラス対応）
		CVector2 uvOffset{ 0, 0 };         // UV左上座標
		CVector2 uvScale{ 1, 1 };          // UV幅・高さ

		// テクスチャ
		ID3D11ShaderResourceView* srv = nullptr;
	};

	/// <summary>
	/// 各描画パスに対応する描画コマンドを集積し管理するクラス
	/// </summary>
	class RenderQueue 
	{
	public:

		static RenderQueue& getInstance() {
			static RenderQueue instance;
			return instance;
		}

		// 描画対象を追加するメソッド
		void add(const RenderCommand& cmd)
		{
			_commands[static_cast<size_t>(cmd.pass)].push_back(cmd);
		}

		// 全ての描画リソースを破棄するメソッド
		void clear() 
		{
			for (auto& bin : _commands)
				bin.clear();
		}

		const Cvector<RenderCommand>& get(RenderPassType pass) const 
		{
			return _commands[static_cast<size_t>(pass)];
		}

		Cvector<RenderCommand>& getMutable(RenderPassType pass) 
		{
			return _commands[static_cast<size_t>(pass)];
		}

		void sort(RenderPassType pass, bool frontToBack = true) 
		{
			auto& cmds = getMutable(pass);
			cmds.orderBy([=](const RenderCommand& a, const RenderCommand& b) {
				return frontToBack ? (a.depth < b.depth) : (a.depth > b.depth);
				});
		}

		void initialize(ID3D11Device* device);
		void flush(RenderPassType pass);
		void flushAll();

	private:

		std::array<Cvector<RenderCommand>, static_cast<size_t>(RenderPassType::Count)> _commands;
		Microsoft::WRL::ComPtr<ID3D11Buffer> _vertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> _constBuffer;

		Microsoft::WRL::ComPtr<ID3D11VertexShader> _vs;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> _ps;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> _inputLayout;
		Microsoft::WRL::ComPtr<ID3D11SamplerState> _sampler;
		Microsoft::WRL::ComPtr<ID3D11BlendState> _blend;
	};
}