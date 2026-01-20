#pragma once
#include <map>
#include <vector>
#include "Definition.h"

namespace CaramelEngine
{
    class RenderQueue
    {
    public:
        void push(const RenderCommand& cmd) noexcept;
        void sort() noexcept;
        void clear() noexcept;

        const Cvector<RenderCommand>& getCommands(uint32_t key) const noexcept;

        // pipeline‚ªŒÅ’è‡‚Å‰ñ‚·‚½‚ß‚Ìkeyˆê——
        std::vector<uint32_t> keys() const;

    private:
        std::map<uint32_t, Cvector<RenderCommand>> _commands;
        static const Cvector<RenderCommand> s_empty;
    };
}