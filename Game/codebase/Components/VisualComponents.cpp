#include "stdafx.h"
#include "VisualComponents.h"
#include "ComponentMessenger.h"

void MeshRenderComponent::Init(const Ogre::String& filename, Ogre::SceneManager* scene_manager){
	m_scene_manager = scene_manager;
	m_entity = m_scene_manager->createEntity(filename);
	m_node = m_scene_manager->getRootSceneNode()->createChildSceneNode();
	m_node->attachObject(m_entity);
}

void MeshRenderComponent::Notify(int type, void* msg){
	switch (type){
	case MSG_NODE_GET_NODE:
			*static_cast<Ogre::SceneNode**>(msg) = m_node;
		break;
	case MSG_INCREASE_SCALE_BY_VALUE:
		{
			Ogre::Vector3 scale = m_node->getScale();
			scale += *static_cast<Ogre::Vector3*>(msg);
			m_node->setScale(scale);
		}
			break;
	case MSG_SET_OBJECT_POSITION:
		m_node->setPosition(*static_cast<Ogre::Vector3*>(msg));
		break;
	default:
		break;
	}
}

void MeshRenderComponent::SetMessenger(ComponentMessenger* messenger){
	m_messenger = messenger;
	m_messenger->Register(MSG_NODE_GET_NODE, this);
	m_messenger->Register(MSG_INCREASE_SCALE_BY_VALUE, this);
	m_messenger->Register(MSG_SET_OBJECT_POSITION, this);
}

void MeshRenderComponent::Shut(){
	if (m_messenger){
		m_messenger->Unregister(MSG_NODE_GET_NODE, this);
		m_messenger->Unregister(MSG_INCREASE_SCALE_BY_VALUE, this);
		m_messenger->Unregister(MSG_SET_OBJECT_POSITION, this);
	}
	if (m_node != NULL){
		m_scene_manager->destroySceneNode(m_node);
		m_node = NULL;
	}
	if (m_entity != NULL){
		m_scene_manager->destroyEntity(m_entity);
		m_entity = NULL;
	}
}

void AnimationComponent::SetMessenger(ComponentMessenger* messenger){
	MeshRenderComponent::SetMessenger(messenger);
	m_messenger->Register(MSG_ANIMATION_PLAY, this);
	m_messenger->Register(MSG_ANIMATION_PAUSE, this);
}

void AnimationComponent::Init(const Ogre::String& filename, Ogre::SceneManager* scene_manager){
	MeshRenderComponent::Init(filename, scene_manager);
}

void AnimationComponent::AddAnimationStates(unsigned int value){
	for (unsigned int i = 0; i < value; i++){
		Ogre::AnimationState* a = NULL;
		m_animation_states.push_back(a);
	}
}

void AnimationComponent::Update(float dt){
	for (unsigned int i = 0; i < m_animation_states.size(); i++){
		if (m_animation_states[i] != NULL){
			if (m_animation_states[i]->getEnabled()){
				m_animation_states[i]->addTime(dt);
			}
		}
	}
}

void AnimationComponent::Notify(int type, void* msg){
	MeshRenderComponent::Notify(type, msg);
	switch (type){
	case MSG_ANIMATION_PLAY:
		{
			int index = static_cast<AnimationMsg*>(msg)->index;
			m_animation_states[index] = m_entity->getAnimationState(static_cast<AnimationMsg*>(msg)->id);
			if (m_animation_states[index] != NULL){
				m_animation_states[index]->setEnabled(true);
				m_animation_states[index]->setLoop(true);
			}
		}
		break;
	case MSG_ANIMATION_PAUSE:
		{
			int index = static_cast<AnimationMsg*>(msg)->index;
			if (m_animation_states[index] != NULL){
				m_animation_states[index]->setEnabled(false);
				m_animation_states[index]->setLoop(false);
			}
		}
		break;
	default:
		break;
	}
}

void AnimationComponent::Shut(){
	if (!m_animation_states.empty()){
		for (unsigned int i = 0; i < m_animation_states.size(); i++){
			if (m_animation_states[i] != NULL){
				m_animation_states[i]->setEnabled(false);
			}
		}
	}
	m_animation_states.clear();
	m_messenger->Unregister(MSG_ANIMATION_PLAY, this);
	m_messenger->Unregister(MSG_ANIMATION_PAUSE, this);
	MeshRenderComponent::Shut();
}

void ChildSceneNodeComponent::Notify(int type, void* msg){
	switch (type)
	{
	case MSG_CHILD_NODE_GET_NODE:
		*static_cast<Ogre::SceneNode**>(msg) = m_node;
		break;
	default:
		break;
	}
}

void ChildSceneNodeComponent::Init(const Ogre::Vector3& position, const Ogre::String& id, Ogre::SceneNode* parent){
	m_id = id;
	m_node = parent->createChildSceneNode();
	m_node->setPosition(position);
}

void ChildSceneNodeComponent::Shut(){
	if (m_messenger){
		m_messenger->Unregister(MSG_CHILD_NODE_GET_NODE, this);
	}
}

void ChildSceneNodeComponent::SetMessenger(ComponentMessenger* messenger){
	m_messenger = messenger;
	m_messenger->Register(MSG_CHILD_NODE_GET_NODE, this);
}