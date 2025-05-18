#pragma once

class Sphere;
class CubeSphere;
// Add future object types here

class ObjectVisitor {
public:
    virtual ~ObjectVisitor() = default;
    virtual void visit(Sphere& sphere) = 0;
    virtual void visit(CubeSphere& cubeSphere) = 0;
    // Add visit methods for future object types
};
