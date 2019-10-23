#pragma once

#ifndef EXPLOSIONCOMPONENT_H
#define EXPLOSIONCOMPONENT_H

#include <AzCore/Component/Component.h>
#include <AzCore/Component/EntityBus.h>
#include <AzCore/Component/TickBus.h>

// EBUS 
#include <AzFramework/Entity/GameEntityContextBus.h>
#include <AzFramework\Components\CameraBus.h>
#include "ExplosionBus.h"

#include <..\..\..\Gems\PhysX\Code\Source\World.h>
#include <AzFramework\Physics\World.h>

// https://docs.nvidia.com/gameworks/content/gameworkslibrary/physx/guide/3.3.4/Manual/SceneQueries.html?highlight=explosion

// https://ru.cppreference.com/w/cpp/utility/functional/function


namespace GameProject
{
	class ExplosionComponent:
		public AZ::Component,
		public AZ::EntityBus::MultiHandler,
		public AZ::TickBus::Handler,
		public AzFramework::GameEntityContextEventBus::Handler,
		public ExplosionBus::Handler
	{
	public:
		AZ_COMPONENT(ExplosionComponent, "{2B5E4717-45E6-4A41-B86D-C49C11B6D57E}", AZ::Component);

		ExplosionComponent() {};
		virtual ~ExplosionComponent() {};

		static void Reflect(AZ::ReflectContext* context);

		void Init() override;
		void Activate() override;
		void Deactivate() override;

		// AZ::TickBus
		void OnTick(float deltaTime, AZ::ScriptTimePoint time);

		//GameEntityContextEventBus
		void OnGameEntitiesStarted() override;
		
		// DefaultBus	
		void ApplyExplosion() override;
		
	private:

		bool m_Enabled = true;
		float m_Elapsed = 0.0f;
		float m_Period = 1.0f;
		float m_radius = 20.0f;
		float m_explosion_power = 1.0f;

		bool m_ApplyExplosionOnActivation = true;


	};
}

bool CallbackFilter(const Physics::WorldBody * body, const Physics::Shape * shape);

#endif