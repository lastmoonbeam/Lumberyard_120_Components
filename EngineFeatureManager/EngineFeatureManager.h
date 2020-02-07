#pragma once

#ifndef EngineFeatureManager_H
#define EngineFeatureManager_H

#include <AzCore/Component/Component.h>
#include <AzCore/Component/EntityBus.h>
#include <AzCore/Component/TickBus.h>

// EBUS 
#include <AzFramework/Entity/GameEntityContextBus.h>
#include <AzFramework\Components\CameraBus.h>
#include "EngineFeatureManagerBus.h"

namespace GameProject
{
	class EngineFeatureManager:
		public AZ::Component,
		public AZ::EntityBus::MultiHandler,
		public AZ::TickBus::Handler,
		public AzFramework::GameEntityContextEventBus::Handler,
		public EngineFeatureManagerBus::Handler
	{
	public:
		AZ_COMPONENT(EngineFeatureManager, "{D30E7844-D755-4335-BF79-AE8C6DBF8722}", AZ::Component);

		EngineFeatureManager() {};
		virtual ~EngineFeatureManager() {};

		static void Reflect(AZ::ReflectContext* context);

		void Init() override;
		void Activate() override;
		void Deactivate() override;

		// AZ::TickBus
		void OnTick(float deltaTime, AZ::ScriptTimePoint time);

		//GameEntityContextEventBus
		void OnGameEntitiesStarted() override;
		
		// DefaultBus

		void ApplyPresetBroadcast(AZStd::string_view presetName) override;
		void ApplyPreset(AZStd::string_view pressetName) override;
		void UpdateAll(bool show) override;
	
	private:
		void OnParamChanged();
		AZStd::string m_name = "default";
		bool m_EnabledAll = true;
		bool m_ApplyOnActivation = false;

		bool m_Terrain = true;
		bool m_Entities = true;
		bool m_Decals = true;
		bool m_Fog = true;
		bool m_SkyBox = true;
		bool m_Vegetation = true;
		bool m_Brushes = true;
		bool m_Sun = true;
		bool m_Objects = true;
		bool m_WaterOcean = true;
		bool m_GeomCaches = true;
		bool m_Roads = true;
		bool m_WaterVolumes = true;
		bool m_Particles = true;

		//	e_MergedMeshes
		//	e_DefaultMaterial
		//	e_Shadows
	};
}
#endif