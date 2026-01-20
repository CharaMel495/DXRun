#include "pch.h"
#include "RenderQueue.h"

using namespace CaramelEngine;

const Cvector<RenderCommand> RenderQueue::s_empty{};

void RenderQueue::push(const RenderCommand& cmd) noexcept
{
    _commands[cmd.sortKey].emplace_back(cmd);
}

void RenderQueue::sort() noexcept
{
    for (auto& [key, vec] : _commands)
    {
        vec.orderBy([](const RenderCommand& a, const RenderCommand& b) noexcept
            {
                return a.depth < b.depth;
                //if (a.depth != b.depth) 
                //    return a.depth < b.depth;
                //// tie-break: stable-ish
                //return a.renderer < b.renderer;
            });
    }
}

void RenderQueue::clear() noexcept
{
    for (auto& [key, vec] : _commands)
        vec.clear();
}

const Cvector<RenderCommand>& RenderQueue::getCommands(uint32_t key) const noexcept
{
    auto it = _commands.find(key);
    if (it == _commands.end())
        return s_empty;
    return it->second;
}

std::vector<uint32_t> RenderQueue::keys() const
{
    std::vector<uint32_t> out;
    out.reserve(_commands.size());
    for (auto& kv : _commands)
        out.push_back(kv.first);
    return out;
}
