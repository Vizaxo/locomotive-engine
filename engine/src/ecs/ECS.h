#pragma once
#include <typeindex>

#include "types/Types.h"
#include "types/Array.h"
#include "types/Map.h"

namespace ECS {

typedef u32 ID;
typedef ID Entity;
constexpr ID NULL_ID = UINT32_MAX;

inline Entity nextId = 0;
inline Array<Entity> freeList;

Entity generateEntity() {
	if (!freeList.isEmpty())
		return freeList.pop();
	else
		return nextId++;
}

void freeEntity(Entity e) {
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
struct ComponentManager : ComponentManagerBase {
	virtual OwningPtr<Iterator<Entity, C>> getIterator() = 0;
	virtual ~ComponentManager() {}
	virtual C& addComponent(Entity e, C& comp) = 0;
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
	};

	struct Stored {
		C comp;
		Entity id;
	};

	Array<Entity> sparse;
	Array<Stored> dense;

	C& addComponent(Entity e, C& comp) override {
		ID index = dense.add({std::move(comp), e});
		sparse.insertAt(e, index, NULL_ID);
		return dense[index].comp;
	}

	C& getComponent(const Entity e) {
		return dense[sparse[e]];
	}

	void removeComponent(const Entity e) {
		ID index = sparse[e];

		// Move last element to deleted position to retain contiguous data
		dense[index] = dense.pop(); // move assignment

		// Update sparse pointer to moved component
		sparse[dense[index].id] = index;
	}

	OwningPtr<Iterator<Entity, C>> getIterator() override {
		return new SparseSetComponentIterator(this);
	}
};

struct ECSManager;
struct System {
	ECSManager* manager;
	void onUpdate();

	void init();};

struct ECSManager {
	HashMap<std::type_index, ComponentManagerBase*> componentManagers;

	ECSManager() {
		addComponentManager(new SparseSetComponentManager<TestComponent>);
		addComponentManager(new SparseSetComponentManager<AnotherComponent>);
		s.init();
	}

	template <typename C>
	void addComponentManager(ComponentManager<C>* cm) {
		ASSERT(componentManagers.get(std::type_index(typeid(C))) == false, "Component manager for components of type '%s' already added", typeid(C).name());
		componentManagers.insert(std::type_index(typeid(C)), cm);
	}

	System s = System{this};

	//template <typename... Cs> Iterator<Entity, Product<Cs...>> view();
	template <typename C> OwningPtr<Iterator<Entity, C>> view() {
		ComponentManager<C>* manager = static_cast<ComponentManager<C>*>(componentManagers.get(std::type_index(typeid(C)))->value);
		return manager->getIterator();
	}

	void tick() {
		// Not every system needs to be ticked every frame. this is just an example
		s.onUpdate();
	}
};

inline void System::init() {
	auto compManager = static_cast<ComponentManager<TestComponent>*>(manager->componentManagers.get(std::type_index(typeid(TestComponent)))->value);
	TestComponent c1 = {128};
	compManager->addComponent(1, c1);
	compManager->addComponent(0, c1);
	compManager->addComponent(10000, c1);
}

inline void System::onUpdate() {
	for (auto it = manager->view<TestComponent>(); !it->atEnd(); it->next()) {
		TestComponent& tc = **it;
		tc.data++;
	}
}


inline ECSManager ecsManager;

}
