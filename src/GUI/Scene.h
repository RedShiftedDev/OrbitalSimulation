#pragma once

#include "GUI/gui.h"
#include <vector>
#include <memory>
#include <string>
#include <algorithm>

namespace gui {

class Scene {
public:
    void addObject(const std::shared_ptr<Object3D>& obj, const std::string& name);
    void removeObject(size_t index);
    void removeObject(const std::string& name);

    const std::vector<SceneObject>& getObjects() const { return objects; }
    std::vector<SceneObject>& getObjects() { return objects; }

private:
    std::vector<SceneObject> objects;
};

} // namespace gui
