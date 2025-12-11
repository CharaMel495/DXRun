#pragma once

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

// --- ユーティリティ系 ---
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
//#include "Mesh.h"
//#include "Material.h"
//#include "IRenderableObject.h"

// --- コライダー関係(基底クラス) ---
//#include "ICollider.h"
//#include "IColliderVisitor.h"

// --- コンポーネント系 ---
#include "Component.h"
//#include "ComponentHolder.h"
#include "CRectangle.h"
#include "CCircle.h"
//#include "SpriteRenderer.h"

// --- コライダー関係(実装クラス) ---
//#include "CollisionInfo.h"
#include "ICollisionObject.h"
//#include "CollisionTester.h"
#include "ColliderManagerBase.h"

// --- アクター系 ---
#include "Actor.h"
#include "Pawn.h"

// --- その他 ---
#include "Scene.h"
//#include "SceneManager.h"
//#include "StageManager.h"
#include "Engine.h"

using namespace CaramelEngine;