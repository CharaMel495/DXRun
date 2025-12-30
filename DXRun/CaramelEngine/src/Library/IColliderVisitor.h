#pragma once

namespace CaramelEngine
{
    class CRectangle;
    class CCircle;
    class CCube;
    class CSphere;
    class CRay;

    class IColliderVisitor 
    {
    public:
        virtual void visit(CRectangle& rect) noexcept = 0;
        virtual void visit(CCircle& circle) noexcept = 0;
        virtual void visit(CCube& cube) noexcept = 0;
        virtual void visit(CSphere& sphere) noexcept = 0;
        virtual void visit(CRay& ray) noexcept = 0;
    };
}