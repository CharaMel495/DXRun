#include "pch.h"

using namespace CaramelEngine;



bool CollisionTester::testRectangleVsRectangle(const CRectangle& a, const CRectangle& b) noexcept
{
    //判定に必要な情報を取得
    auto dataA = a.getHitJudgeData();
    auto dataB = b.getHitJudgeData();

    //頂点情報をキャッシュ
    auto vertsA = dataA.Verts;
    auto vertsB = dataB.Verts;

    //判定に用いる分離軸の候補を用意
    CVector3 sideDirs[4] =
    {
        (vertsA[0] - vertsA[1]).normalized(), (vertsA[1] - vertsA[2]).normalized(),
        (vertsB[0] - vertsB[1]).normalized(), (vertsB[1] - vertsB[2]).normalized()
    };

    //全ての分離軸の可能性を見ていく
    for (auto& u : sideDirs)
    {
        //頂点の内、軸に射影した物の最大と最小を求める
        float minA = vertsA[0].dot(u);
        float maxA = minA;
        float minB = vertsB[0].dot(u);
        float maxB = minB;

        for (int i = 1; i < 4; i++)
        {
            float projA = vertsA[i].dot(u);
            minA = std::min(minA, projA);
            maxA = std::max(maxA, projA);

            float projB = vertsB[i].dot(u);
            minB = std::min(minB, projB);
            maxB = std::max(maxB, projB);
        }

        //もし分離軸が存在するなら当たってない
        if (maxB < minA || maxA < minB)
            return false;
    }
    //もし、分離軸が存在しないなら当たってる
    return true;
}

bool CollisionTester::testRectangleVsCircle(const CRectangle& a, const CCircle& b) noexcept
{
    //頂点のみ使うのでここで取得
    auto rectData = a.getHitJudgeData();
    auto circleData = b.getHitJudgeData();

    auto verts = rectData.Verts;

    //分離軸の候補となる４直線を求める
    //※矩形１つにつき２つの辺方向をもつので４つ候補が挙がる
    //判定に用いる分離軸の候補を用意
    CVector3 sideDirs[2] =
    {
        (verts[0] - verts[1]).normalized(), (verts[1] - verts[2]).normalized()
    };

    //分離軸の候補全てから分離軸になりうるかを判定していく
    for (auto& u : sideDirs)
    {
        //矩形の頂点の内１点を直線に射影
        float min = verts[0].dot(u);
        float max = min;

        //内積を用いて残りの頂点も直線に射影していき
        //各矩形を射影した際の射影区間を表す最大値と最小値を求める
        for (int i = 1; i < 4; i++)
        {
            float proj = verts[i].dot(u);
            min = std::min(min, proj);
            max = std::max(max, proj);
        }

        float projCirclePos = circleData.Transform->getPosition().dot(u);

        //射影した円が矩形の射影区間に入ってなければ当たっていない
        if (projCirclePos + circleData.Radius < min || max < projCirclePos - circleData.Radius)
            return false;
    }

    //射影した円が矩形の射影区間に入っていたので当たり
    return true;
}

bool CollisionTester::testCircleVsCircle(const CCircle& a, const CCircle& b) noexcept
{
    //判定に必要なデータを受け取る
    auto circleDataA = a.getHitJudgeData();
    auto circleDataB = b.getHitJudgeData();

    //判定の計測は平方処理を避けるため、2乗した者同士で判定する
    float distSq = (circleDataA.Transform->getPosition() - circleDataB.Transform->getPosition()).sqrtmagnitude();
    float judgeLen = std::pow((circleDataA.Radius + circleDataB.Radius), 2.0f);

    return distSq < judgeLen;
}

bool CollisionTester::testOBBVsOBB(const CubeHitJudgeData& a, const CubeHitJudgeData& b) noexcept
{
    // 中心間ベクトル
    CVector3 t = b.Center - a.Center;

    // 判定に使う軸（最大15本）
    CVector3 axes[15];
    int axisCount = 0;

    // Aの軸
    for (int i = 0; i < 3; ++i)
        axes[axisCount++] = a.Axis[i];

    // Bの軸
    for (int i = 0; i < 3; ++i)
        axes[axisCount++] = b.Axis[i];

    // 外積軸
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            CVector3 axis = a.Axis[i].cross(b.Axis[j]);
            if (axis.sqrtmagnitude() > 1e-6f) // 平行でなければ
                axes[axisCount++] = axis.normalized();
        }
    }

    // SAT本体
    for (int i = 0; i < axisCount; ++i)
    {
        const CVector3& L = axes[i];

        float rA =
            std::abs(a.Axis[0].dot(L)) * a.HalfExtent.getX() +
            std::abs(a.Axis[1].dot(L)) * a.HalfExtent.getY() +
            std::abs(a.Axis[2].dot(L)) * a.HalfExtent.getZ();

        float rB =
            std::abs(b.Axis[0].dot(L)) * b.HalfExtent.getX() +
            std::abs(b.Axis[1].dot(L)) * b.HalfExtent.getY() +
            std::abs(b.Axis[2].dot(L)) * b.HalfExtent.getZ();

        float d = std::abs(t.dot(L));

        if (d > rA + rB)
            return false; // 分離軸発見
    }

    return true; // どの軸でも分離しなかった
}

bool CollisionTester::testOBBVsSphere(const CubeHitJudgeData& cube, const SphereHitJudgeData& sphere) noexcept
{
    // 球の中心から直方体中心へのベクトル
    CVector3 dir = sphere.Center - cube.Center;

    // 最近点
    CVector3 closest = cube.Center;

    float extents[3] =
    {
        cube.HalfExtent.getX(),
        cube.HalfExtent.getY(),
        cube.HalfExtent.getZ()
    };

    for (int i = 0; i < 3; ++i)
    {
        float dist = dir.dot(cube.Axis[i]);

        float min = -extents[i];
        float max = extents[i];

        if (dist < min) dist = min;
        else if (dist > max) dist = max;

        closest += cube.Axis[i] * dist;
    }

    // 最近点と球の中心の距離
    CVector3 diff = sphere.Center - closest;
    float distSq = diff.sqrtmagnitude();

    return distSq <= (sphere.Radius * sphere.Radius);
}

bool CollisionTester::testRayVsOBB(const RayHitJudgeData& ray, const CubeHitJudgeData& cube) noexcept
{
    // Ray を Cube ローカル空間へ
    CVector3 p = ray.Origin - cube.Center;

    // ローカルRayの origin / direction
    CVector3 localOrigin(
        p.dot(cube.Axis[0]),
        p.dot(cube.Axis[1]),
        p.dot(cube.Axis[2])
    );

    CVector3 localDir(
        ray.Direction.dot(cube.Axis[0]),
        ray.Direction.dot(cube.Axis[1]),
        ray.Direction.dot(cube.Axis[2])
    );

    float tMin = 0.0f;
    float tMax = ray.Length;

    // 各軸でスラブ判定
    for (int i = 0; i < 3; ++i)
    {
        float origin =
            (i == 0) ? localOrigin.getX() :
            (i == 1) ? localOrigin.getY() :
            localOrigin.getZ();

        float dir =
            (i == 0) ? localDir.getX() :
            (i == 1) ? localDir.getY() :
            localDir.getZ();

        float extent =
            (i == 0) ? cube.HalfExtent.getX() :
            (i == 1) ? cube.HalfExtent.getY() :
            cube.HalfExtent.getZ();

        if (std::abs(dir) < 1e-6f)
        {
            // Ray がこのスラブと平行
            if (origin < -extent || origin > extent)
                return false;
        }
        else
        {
            float inv = 1.0f / dir;
            float t1 = (-extent - origin) * inv;
            float t2 = (extent - origin) * inv;

            if (t1 > t2) std::swap(t1, t2);

            tMin = std::max(tMin, t1);
            tMax = std::min(tMax, t2);

            if (tMin > tMax)
                return false;
        }
    }

    return true;
}

bool CollisionTester::testSphereVsSphere(const SphereHitJudgeData& a, const SphereHitJudgeData& b) noexcept
{
    // 中心間を表すベクトル
    CVector3 d = a.Center - b.Center;

    // 距離の二乗
    float distSq = d.sqrtmagnitude();

    // 半径和の２乗
    float r = a.Radius + b.Radius;
    float rSq = r * r;

    // 平方を避けるため、２乗で計算
    return distSq <= rSq;
}

bool CollisionTester::testRayVsSphere(const RayHitJudgeData& ray, const SphereHitJudgeData& sphere) noexcept
{
    // Rayの始点から球の中心に向かうベクトル
    CVector3 m = sphere.Center - ray.Origin;

    // Ray軸への射影
    float t = m.dot(ray.Direction);

    // Ray軸へ射影して範囲外ならそも当たってない
    if (t < 0.0f || t > ray.Length)
        return false;

    // 最近点を求める
    CVector3 closest = ray.Origin + ray.Direction * t;

    // 最近点と球の距離を求める
    CVector3 d = sphere.Center - closest;
    float distSq = d.sqrtmagnitude();

    // 半径の内にいるかは２乗で計算する
    return distSq <= (sphere.Radius * sphere.Radius);
}