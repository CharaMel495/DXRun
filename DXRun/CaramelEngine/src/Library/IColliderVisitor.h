#pragma once

namespace CaramelEngine
{
    class CRectangle;
    class CCircle;

    class IColliderVisitor 
    {
    public:
        virtual void visit(CRectangle& rect) noexcept = 0;
        virtual void visit(CCircle& circle) noexcept = 0;
    };
}