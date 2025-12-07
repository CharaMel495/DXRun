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