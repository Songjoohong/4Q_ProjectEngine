#pragma once
#include "ECS.h"

using namespace ECS;

class ScriptSystem : public EntitySystem,
	public EventSubscriber<Events::OnEntityCreated>,
	

{
};

