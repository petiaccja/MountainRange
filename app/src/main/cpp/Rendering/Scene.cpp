#include "Scene.hpp"



namespace mrange {


void Scene::AddEntity(const TileEntity* entity) {
	auto [it, newlyAdded] = m_entites.insert(entity);

	[[unlikely]]
	if (!newlyAdded) {
		throw std::invalid_argument("Entity is already part of the scene.");
	}
}


void Scene::RemoveEntity(const TileEntity* entity) {
	auto it = m_entites.find(entity);

	[[unlikely]]
	if (it == m_entites.end()) {
		throw std::invalid_argument("Entity is not part of the scene.");
	}

	m_entites.erase(it);
}


void Scene::Reset(const TileEntity* entity) {
	m_entites.clear();
}



} // namespace mrange