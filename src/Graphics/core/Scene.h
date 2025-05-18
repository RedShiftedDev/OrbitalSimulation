#pragma once

#include <vector>
#include <memory>
#include "SceneObject.h"

class Scene {
public:
    void addObject(std::shared_ptr<SceneObject> object) {
        objects.push_back(std::move(object));
    }

    void removeObject(const SceneObject* object) {
        objects.erase(
            std::remove_if(objects.begin(), objects.end(),
                [object](const auto& ptr) { return ptr.get() == object; }),
            objects.end());
    }

    const std::vector<std::shared_ptr<SceneObject>>& getObjects() const {
        return objects;
    }

private:
    std::vector<std::shared_ptr<SceneObject>> objects;
};
