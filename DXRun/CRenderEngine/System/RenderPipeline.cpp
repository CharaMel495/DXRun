#include "pch.h"
#include "renderPipeline.h"
#include "graphicsDevice.h"
#include "renderTargets.h"
#include "renderQueue.h"
#include "Definition.h" // RenderCommand
#include "IShader.h"
#include "IRenderer.h"

using namespace CaramelEngine;

static void setTargetAndClear(GraphicsDevice& gfx, IRenderTarget& target)
{
    auto& ctx = gfx.context();

    // ここはCDeviceContextに thin wrapper を増やすのが本命だけど、
    // まずは native を取れる前提でもOK（あなたの設計に合わせて調整）
    ID3D11DeviceContext* native = ctx.getNative(); // これも追加しておくと便利

    ID3D11RenderTargetView* rtvs[] = { target.rtv() };
    native->OMSetRenderTargets(1, rtvs, target.dsv());

    const auto& vp = target.viewport();
    native->RSSetViewports(1, &vp);

    // clear
    float clearColor[4] = { 0,0,0,1 };
    native->ClearRenderTargetView(target.rtv(), clearColor);
    native->ClearDepthStencilView(target.dsv(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void DefaultGamePipeline::begin(GraphicsDevice& gfx, IRenderTarget& target)
{
    setTargetAndClear(gfx, target);
}

void DefaultGamePipeline::buildQueue(RenderQueue& queue)
{
    // ここは本来、Scene/WorldからIRendererを列挙して queue.push() する。
    // 今は“繋がる形”なので空でOK。
}

void DefaultGamePipeline::execute(GraphicsDevice& gfx, IRenderTarget& target, RenderQueue& queue)
{
    auto& ctx = gfx.context();

    // keyの回し順は「固定順」を推奨（例：world→ui→overlay）。
    // 今は queue 側の key 設計に合わせて明示順にするのが最強。
    // 例：for (uint32_t key : queue.keys()) ...

    for (uint32_t key : queue.keys())
    {
        const auto& cmds = queue.getCommands(key);
        for (const auto& cmd : cmds)
        {
            // shader
            cmd.shader->bind(ctx);
            cmd.shader->applyCBuffer(ctx, cmd);

            // renderer側で Draw 系を持たせる設計ならここで呼ぶ
            // cmd.renderer->draw(ctx); など
            // 今は最小接続のため省略
        }
    }
}

void DefaultGamePipeline::end(GraphicsDevice& gfx, IRenderTarget& target)
{
    // post-process hook point
    // 例：offscreenTargetの場合 srv() を使って full-screen quad を描く
}

void DefaultEditorPipeline::begin(GraphicsDevice& gfx, IRenderTarget& target)
{
    setTargetAndClear(gfx, target);
}

void DefaultEditorPipeline::buildQueue(RenderQueue& queue)
{
    // editor: gizmo, grid, selection outline etc
}

void DefaultEditorPipeline::execute(GraphicsDevice& gfx, IRenderTarget& target, RenderQueue& queue)
{
    // 基本は game と同じでOK。後でeditor専用パスを追加していく。
    DefaultGamePipeline tmp;
    tmp.execute(gfx, target, queue);
}

void DefaultEditorPipeline::end(GraphicsDevice& gfx, IRenderTarget& target)
{
}
