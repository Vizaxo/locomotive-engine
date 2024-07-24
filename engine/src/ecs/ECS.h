#pragma once
#include <typeindex>

#include "types/Types.h"
#include "types/Array.h"
#include "types/ArrayView.h"
#include "types/Map.h"

namespace ECS {

typedef u32 ID;
typedef ID Entity;
constexpr ID NULL_ID = UINT32_MAX;

inline Entity nextId = 0;
inline Array<Entity> freeList;

inline Entity generateEntity() {
	if (!freeList.isEmpty())
		return freeList.pop();
	else
		return nextId++;
}

inline void freeEntity(Entity e) {
	// Delete components
	freeList.add(e);
}

template <typename I, typename T>
struct Iterator {
	virtual T& operator*() = 0;
	virtual bool atEnd() = 0;
	virtual void next() = 0;
	virtual I getIndex() = 0;
	virtual ~Iterator() {}
protected:
	Iterator() {}
};

struct TestComponent {
	u64 data;
};

struct AnotherComponent {
	float f;
};

struct ComponentManagerBase {
};

template <typename C>
struct Stored {
	C comp;
	Entity id;
};

template <typename C>
struct ComponentManager : ComponentManagerBase {
	virtual RefPtr<Iterator<Entity, C>> getIterator() = 0;
	virtual ArrayView<Stored<C>> getRawBuffer() = 0;
	virtual ~ComponentManager() {}
	virtual C& addComponent(Entity e, C&& comp) = 0;
	virtual C& getComponent(Entity e) = 0;
};

template <typename C>
struct SparseSetComponentManager : ComponentManager<C> {
	struct SparseSetComponentIterator : Iterator<Entity, C> {
		SparseSetComponentIterator(SparseSetComponentManager<C>* in) : components(in) {}
		SparseSetComponentManager<C>* components;
		u32 index = 0;
		C& operator*() override { return components->dense[index].comp; }
		bool atEnd() override { return index >= components->dense.num(); }
		void next() override { index++; }
		Entity getIndex() override { return components->dense[index].id; }
		void reset() { index = 0; };
	};
	SparseSetComponentIterator it{this};

	Array<Entity> sparse;
	Array<Stored<C>> dense;

	ArrayView<Stored<C>> getRawBuffer() override { return { dense.data, dense.num() }; }

	C& addComponent(Entity e, C&& comp) override {
		ID index = dense.add({std::move(comp), e});
		sparse.insertAt(e, index, NULL_ID);
		return dense[index].comp;
	}

	C& getComponent(const Entity e) override {
		return dense[sparse[e]].comp;
	}

	void removeComponent(const Entity e) {
		ID index = sparse[e];

		// Move last element to deleted position to retain contiguous data
		dense[index] = dense.pop(); // move assignment

		// Update sparse pointer to moved component
		sparse[dense[index].id] = index;
	}

	RefPtr<Iterator<Entity, C>> getIterator() override {
		it.reset();
		return &it;
	}
};

struct ECSManager;
struct System {
	ECSManager* manager;
	void onUpdate();

	void init();};

struct ECSManager {
	HashMap<u32, ComponentManagerBase*> componentManagers;

	ECSManager() {
		addComponentManager(new SparseSetComponentManager<TestComponent>);
		addComponentManager(new SparseSetComponentManager<AnotherComponent>);
		s.init();
	}

	template <typename C>
	void addComponentManager(ComponentManager<C>* cm) {
		ASSERT(componentManagers.get(TypeID<C>::get()) == false, "Component manager for components of type '%s' already added", typeid(C).name());
		componentManagers.insert(TypeID<C>::get(), cm);
	}

	template <typename C>
	void addComponent(Entity e, C&& comp) {
		RefPtr<typename HashMap<u32, ECS::ComponentManagerBase*>::Entry, true> cm = componentManagers.get(TypeID<C>::get());
		ASSERT(cm, "Failed to get component manager for component type '%s'", typeid(C).name());
		ComponentManager<C>* manager = static_cast<ComponentManager<C>*>(cm->value);
		manager->addComponent(e, std::move(comp));
	}

	System s = System{this};

	//template <typename... Cs> Iterator<Entity, Product<Cs...>> view();
	template <typename C> RefPtr<Iterator<Entity, C>> view() {
		RefPtr<typename HashMap<u32, ECS::ComponentManagerBase*>::Entry, true> cm = componentManagers.get(TypeID<C>::get());
		ASSERT(cm, "Failed to get component manager for component type '%s'", typeid(C).name());
		ComponentManager<C>* manager = static_cast<ComponentManager<C>*>(cm->value);
		return manager->getIterator();
	}

	template <typename C> RefPtr<ComponentManager<C>> getComponentManager() {
		RefPtr<typename HashMap<u32, ECS::ComponentManagerBase*>::Entry, true> cm = componentManagers.get(TypeID<C>::get());
		ComponentManager<C>* m = static_cast<ComponentManager<C>*>(cm->value);
		return m;
	}

	template <typename C> C& getComponent(Entity e) {
		return getComponentManager<C>()->getComponent(e);
	}

	void tick() {
		// Not every system needs to be ticked every frame. this is just an example
		s.onUpdate();
	}
};

inline void System::init() {
	auto compManager = static_cast<ComponentManager<TestComponent>*>(manager->componentManagers.get(TypeID<TestComponent>::get())->value);
}

inline void System::onUpdate() {
	for (auto it = manager->view<TestComponent>(); !it->atEnd(); it->next()) {
		TestComponent& tc = **it;
		tc.data++;
	}
}


inline ECSManager ecsManager;

}
