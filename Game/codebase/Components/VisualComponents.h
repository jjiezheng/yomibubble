#ifndef _N_VISUAL_COMPONENTS_H_
#define _N_VISUAL_COMPONENTS_H_

#include "ComponentsPrereq.h"

class MeshRenderComponent : public Component, public IComponentObserver {
public:
	MeshRenderComponent(void) : m_node(NULL), m_entity(NULL), m_scene_manager(NULL){ m_type = COMPONENT_RENDERER; }
	virtual ~MeshRenderComponent(void){}
	virtual void Notify(int type, void* message);
	virtual void Init(const Ogre::String& filename, Ogre::SceneManager* scene_manager);
	Ogre::SceneNode* GetSceneNode() const { return m_node; }
	Ogre::Entity* GetEntity() const { return m_entity; }
	virtual void Shut();
	virtual void SetMessenger(ComponentMessenger* messenger);

protected:
	Ogre::SceneNode*	m_node;
	Ogre::Entity*		m_entity;
	Ogre::SceneManager* m_scene_manager;
};

class ChildSceneNodeComponent : public Component, public IComponentObserver{
public:
	ChildSceneNodeComponent(void){}
	virtual ~ChildSceneNodeComponent(void){}
	virtual void Notify(int type, void* msg);
	virtual void Init(const Ogre::Vector3& position, const Ogre::String& id, Ogre::SceneNode* parent);
	virtual void Shut();
	virtual void SetMessenger(ComponentMessenger* messenger);
	Ogre::SceneNode* GetNode() const { return m_node; }
	const Ogre::String& GetId() const { return m_id; }

protected:
	Ogre::SceneNode* m_node;
	Ogre::String m_id;
};

class AnimationComponent : public MeshRenderComponent, public IComponentUpdateable{
public:
	AnimationComponent(void){ m_type = COMPONENT_ANIMATION; }
	virtual ~AnimationComponent(void){}
	virtual void Update(float dt);
	virtual void Notify(int type, void* message);
	virtual void Init(const Ogre::String& filename, Ogre::SceneManager* scene_manager);
	virtual void AddAnimationStates(unsigned int value = 1);
	virtual void Shut();
	virtual void SetMessenger(ComponentMessenger* messenger);

protected:
	std::vector<Ogre::AnimationState*> m_animation_states;
};

#endif // _N_VISUAL_COMPONENTS_H_