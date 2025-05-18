#include "GUI/Scene.h"

namespace gui {

void Scene::addObject(const std::shared_ptr<Object3D>& obj, const std::string& name) {
    // Generate unique name if necessary
    std::string uniqueName = name;
    int suffix = 1;
    while (std::find_if(objects.begin(), objects.end(),
                       [&uniqueName](const SceneObject& obj) { return obj.name == uniqueName; }) != objects.end()) {
        uniqueName = name + " " + std::to_string(suffix++);
    }

    objects.push_back({uniqueName, obj, false});
}

void Scene::removeObject(size_t index) {
    if (index < objects.size()) {
        objects.erase(objects.begin() + index);
    }
}

void Scene::removeObject(const std::string& name) {
    objects.erase(
        std::remove_if(objects.begin(), objects.end(),
                      [&name](const SceneObject& obj) { return obj.name == name; }),
        objects.end());
}

} // namespace gui
