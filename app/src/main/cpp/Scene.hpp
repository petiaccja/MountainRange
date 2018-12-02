#pragma once


#include <set>


namespace mrange {


class TileEntity;


class Scene {
public:
	void AddEntity(const TileEntity* entity);
	void RemoveEntity(const TileEntity* entity);
	void Reset(const TileEntity* entity);

	auto begin() const { return m_entites.begin(); }
	auto end() const { return m_entites.end(); }
	auto cbegin() const { return m_entites.cbegin(); }
	auto cend() const { return m_entites.cend(); }
private:
	std::set<const TileEntity*> m_entites;
};


} // namespace mrange