#pragma once

//
// pch.h
// Header for standard system include files.
//

#include <winsdkver.h>
#ifndef _WIN32_WINNT
//#define _WIN32_WINNT 0x0601
#define _WIN32_WINNT 0x0A00
#endif
#include <sdkddkver.h>

// Use the C++ standard templated min/max
#define NOMINMAX

// Include <mcx.h> if you need this
#define NOMCX

// Include <winsvc.h> if you need this
#define NOSERVICE

// WinHelp is deprecated
#define NOHELP

#define WIN32_LEAN_AND_MEAN

// --- stdヘッダ ---
#include <variant>
#include <vector>
#include <map>
#include <unordered_map>
#include <functional>
#include <string>
#include <any>
#include <memory>
#include <iostream>
#include <fstream>
#include <array>
#include <set>
#include <unordered_set>
#include <algorithm>
#include <numeric>			// accumulate など
#include <utility>			// std::move, std::swap, std::pair
#include <cassert>
#include <cmath>
#include <ctime>
#include <chrono>
#include <iomanip>
#include <optional>
#include <span>
#include <type_traits>

// --- DirectX関係 ---
#include "DXInclude.h"
#include "DXContext.h"
#include <DirectXMath.h>

// --- ユーティリティ系 ---
#include "Cvector.h"
#include "CString.h"
#include "CVector3.h"
#include "CVector2.h"
#include "VectorConversion.h"
#include "CQuaternion.h"
#include "Color.h"
// --- JSON関係 ---
#include "JsonValue.h"
#include "JsonReader.h"
// --- デバッグ機能系 ---
#include "Logger.h"

// --- ゲーム基盤クラス系 ---
#include "Transform.h"
#include "MonoBehaviour.h"
#include "EventDispatcher.h"

// ---描画周り、最新---
#include "Mesh.h"
#include "Material.h"
#include "IRenderableObject.h"

// --- コライダー関係(基底クラス) ---
#include "ICollider.h"
#include "IColliderVisitor.h"

// --- コンポーネント系 ---
#include "Component.h"
#include "ComponentHolder.h"
#include "CRectangle.h"
#include "CCircle.h"
#include "CCube.h"
#include "CSphere.h"
#include "CRay.h"
#include "ShapeTraits.h"
#include "SpriteRenderer.h"

// --- コライダー関係(実装クラス) ---
#include "CollisionInfo.h"
#include "ICollisionObject.h"
#include "CollisionTester.h"
#include "ColliderManagerBase.h"

// --- アクター系 ---
#include "Actor.h"
#include "Pawn.h"

// --- その他 ---
#include "Scene.h"
#include "SceneManager.h"
#include "StageManager.h"
#include "Engine.h"