#include "GameProject_precompiled.h"
#pragma optimize("", on)

#include <AzCore/Component/Entity.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/RTTI/BehaviorContext.h>

/// EBUS
#include <AzCore/Component/TransformBus.h>
#include <AzCore/Component/ComponentApplicationBus.h>

#include <AzFramework/Entity/EntityContextBus.h>
#include <AzFramework/Entity/GameEntityContextBus.h>

#include <AzFramework\Components\ConsoleBus.h>

/// User Headers
#include "EngineFeatureManager.h"
#include "EngineFeatureManagerBus.h"

using namespace GameProject;

void EngineFeatureManager::Reflect(AZ::ReflectContext * context)
{
	AZ::SerializeContext* serialize = azrtti_cast<AZ::SerializeContext*>(context);

	if (serialize)
	{
		serialize->Class<EngineFeatureManager, AZ::Component >()
			->Version(1)
			->Field("presetname", &EngineFeatureManager::m_name)
			->Field("EnabledAll", &EngineFeatureManager::m_EnabledAll)
			->Field("e_Terrain", &EngineFeatureManager::m_Terrain)
			->Field("e_Entities", &EngineFeatureManager::m_Entities)
			->Field("e_Decals", &EngineFeatureManager::m_Decals)
			->Field("e_Fog", &EngineFeatureManager::m_Fog)
			->Field("e_SkyBox", &EngineFeatureManager::m_SkyBox)
			->Field("e_Vegetation", &EngineFeatureManager::m_Vegetation)
			->Field("e_Brushes", &EngineFeatureManager::m_Brushes)
			->Field("e_Sun", &EngineFeatureManager::m_Sun)
			->Field("e_Objects", &EngineFeatureManager::m_Objects)
			->Field("e_WaterOcean", &EngineFeatureManager::m_WaterOcean)
			->Field("e_GeomCaches", &EngineFeatureManager::m_GeomCaches)
			->Field("e_Roads", &EngineFeatureManager::m_Roads)
			->Field("e_WaterVolumes", &EngineFeatureManager::m_WaterVolumes)
			->Field("e_Particles", &EngineFeatureManager::m_Particles)
			->Field("ApplyOnActivation", &EngineFeatureManager::m_ApplyOnActivation)
			



			;

		AZ::EditContext* edit = serialize->GetEditContext();
		if (edit)
		{
			edit->Class<EngineFeatureManager>("EngineFeatureManager", "")
				->ClassElement(AZ::Edit::ClassElements::EditorData, "")
				->Attribute(AZ::Edit::Attributes::Category, CATEGORY)
				->Attribute(AZ::Edit::Attributes::Icon, ":Editor/Icons/Components/ComponentPlaceholder.png")
				->Attribute(AZ::Edit::Attributes::PrimaryAssetType, azrtti_typeid<Component>())
				->Attribute(AZ::Edit::Attributes::ViewportIcon, ":Editor/Icons/Components/ComponentPlaceholder.png")
				->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC("Game", 0x232b318c))
				->Attribute(AZ::Edit::Attributes::AutoExpand, true)
				->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC("Game", 0x232b318c))
				->DataElement(AZ::Edit::UIHandlers::Default, &EngineFeatureManager::m_name, "Preset name", "")
				->DataElement(AZ::Edit::UIHandlers::Default, &EngineFeatureManager::m_ApplyOnActivation, "Apply on activation", "")

				

				->DataElement(AZ::Edit::UIHandlers::Default, &EngineFeatureManager::m_EnabledAll, "EnabledAll", "")
				->Attribute(AZ::Edit::Attributes::ChangeNotify, &EngineFeatureManager::OnParamChanged)
				->ClassElement(AZ::Edit::ClassElements::Group, "Details")
				->DataElement(AZ::Edit::UIHandlers::Default, &EngineFeatureManager::m_Terrain, "e_Terrain", "")
				->Attribute(AZ::Edit::Attributes::ChangeNotify, &EngineFeatureManager::OnParamChanged)
				->DataElement(AZ::Edit::UIHandlers::Default, &EngineFeatureManager::m_Entities, "e_Entities", "")
				->Attribute(AZ::Edit::Attributes::ChangeNotify, &EngineFeatureManager::OnParamChanged)
				->DataElement(AZ::Edit::UIHandlers::Default, &EngineFeatureManager::m_Decals, "e_Decals", "")
				->Attribute(AZ::Edit::Attributes::ChangeNotify, &EngineFeatureManager::OnParamChanged)
				->DataElement(AZ::Edit::UIHandlers::Default, &EngineFeatureManager::m_Fog, "e_Fog", "")
				->Attribute(AZ::Edit::Attributes::ChangeNotify, &EngineFeatureManager::OnParamChanged)
				->DataElement(AZ::Edit::UIHandlers::Default, &EngineFeatureManager::m_SkyBox, "e_SkyBox", "")
				->Attribute(AZ::Edit::Attributes::ChangeNotify, &EngineFeatureManager::OnParamChanged)
				->DataElement(AZ::Edit::UIHandlers::Default, &EngineFeatureManager::m_Vegetation, "e_Vegetation", "")
				->Attribute(AZ::Edit::Attributes::ChangeNotify, &EngineFeatureManager::OnParamChanged)
				->DataElement(AZ::Edit::UIHandlers::Default, &EngineFeatureManager::m_Brushes, "e_Brushes", "")
				->Attribute(AZ::Edit::Attributes::ChangeNotify, &EngineFeatureManager::OnParamChanged)
				->DataElement(AZ::Edit::UIHandlers::Default, &EngineFeatureManager::m_Sun, "e_Sun", "")
				->Attribute(AZ::Edit::Attributes::ChangeNotify, &EngineFeatureManager::OnParamChanged)
				->DataElement(AZ::Edit::UIHandlers::Default, &EngineFeatureManager::m_Objects, "e_Objects", "")
				->Attribute(AZ::Edit::Attributes::ChangeNotify, &EngineFeatureManager::OnParamChanged)
				->DataElement(AZ::Edit::UIHandlers::Default, &EngineFeatureManager::m_WaterOcean, "e_WaterOcean", "")
				->Attribute(AZ::Edit::Attributes::ChangeNotify, &EngineFeatureManager::OnParamChanged)
				->DataElement(AZ::Edit::UIHandlers::Default, &EngineFeatureManager::m_GeomCaches, "e_GeomCaches", "")
				->Attribute(AZ::Edit::Attributes::ChangeNotify, &EngineFeatureManager::OnParamChanged)
				->DataElement(AZ::Edit::UIHandlers::Default, &EngineFeatureManager::m_Roads, "e_Roads", "")
				->Attribute(AZ::Edit::Attributes::ChangeNotify, &EngineFeatureManager::OnParamChanged)
				->DataElement(AZ::Edit::UIHandlers::Default, &EngineFeatureManager::m_WaterVolumes, "e_WaterVolumes", "")
				->Attribute(AZ::Edit::Attributes::ChangeNotify, &EngineFeatureManager::OnParamChanged)
				->DataElement(AZ::Edit::UIHandlers::Default, &EngineFeatureManager::m_Particles, "e_Particles", "")
				->Attribute(AZ::Edit::Attributes::ChangeNotify, &EngineFeatureManager::OnParamChanged)

				;
		}
	}

	if (AZ::BehaviorContext* behaviorContext = azrtti_cast<AZ::BehaviorContext*>(context))
	{
		AZ::BehaviorParameterOverrides someEventParam1 = { "preset name", "Param Tooltip" };
		behaviorContext->EBus<EngineFeatureManagerBus>("EngineFeatureManagerBus")
			->Event("ApplyPreset", &EngineFeatureManagerBus::Events::ApplyPreset, { { someEventParam1} })
			->Event("ApplyPresetBroadcast", &EngineFeatureManagerBus::Events::ApplyPresetBroadcast, { { someEventParam1} })

			;

		behaviorContext->Class<EngineFeatureManager>()
			->RequestBus("EngineFeatureManagerBus")
			;
	}
}

void EngineFeatureManager::Init()
{
	//AZ_Printf("EngineFeatureManager", "Init");
}

void EngineFeatureManager::Activate()
{
	//AZ_Printf("EngineFeatureManager", "Activate");
	//AZ::EntityBus::Handler::BusConnect(GetEntityId());

	AzFramework::GameEntityContextEventBus::Handler::BusConnect();
	EngineFeatureManagerBus::Handler::BusConnect(GetEntityId());

	if (m_ApplyOnActivation)
		UpdateAll(m_EnabledAll);
}

void EngineFeatureManager::Deactivate()
{
	//AZ::TickBus::Handler::BusDisconnect();
	//AZ::EntityBus::Handler::BusDisconnect(GetEntityId());
	AzFramework::GameEntityContextEventBus::Handler::BusDisconnect();
	EngineFeatureManagerBus::Handler::BusDisconnect();
}

void EngineFeatureManager::OnTick(float deltaTime, AZ::ScriptTimePoint time)
{

}

void EngineFeatureManager::OnGameEntitiesStarted()
{
	//AZ_Printf("EngineFeatureManager", "OnGameEntitiesStarted");
	AZ::TickBus::Handler::BusConnect();
	UpdateAll(m_EnabledAll);
}

void EngineFeatureManager::ApplyPresetBroadcast(AZStd::string_view presetName)
{
	EngineFeatureManagerBus::Broadcast(&EngineFeatureManagerBus::Events::ApplyPreset, presetName);
}

void EngineFeatureManager::ApplyPreset(AZStd::string_view pressetName)
{
	// if broadcast request names is equals then update to apply CVars
	if (pressetName == m_name)
		UpdateAll(m_EnabledAll);
}

void EngineFeatureManager::UpdateAll(bool show)
{
	m_EnabledAll = show;

	if (m_EnabledAll)
	{
		AzFramework::ConsoleRequestBus::Broadcast(&AzFramework::ConsoleRequestBus::Events::ExecuteConsoleCommand, "e_Terrain 1");
		AzFramework::ConsoleRequestBus::Broadcast(&AzFramework::ConsoleRequestBus::Events::ExecuteConsoleCommand, "e_Entities 1");
		AzFramework::ConsoleRequestBus::Broadcast(&AzFramework::ConsoleRequestBus::Events::ExecuteConsoleCommand, "e_Decals 1");
		AzFramework::ConsoleRequestBus::Broadcast(&AzFramework::ConsoleRequestBus::Events::ExecuteConsoleCommand, "e_Fog 1");
		AzFramework::ConsoleRequestBus::Broadcast(&AzFramework::ConsoleRequestBus::Events::ExecuteConsoleCommand, "e_SkyBox 1");
		AzFramework::ConsoleRequestBus::Broadcast(&AzFramework::ConsoleRequestBus::Events::ExecuteConsoleCommand, "e_Vegetation 1");
		AzFramework::ConsoleRequestBus::Broadcast(&AzFramework::ConsoleRequestBus::Events::ExecuteConsoleCommand, "e_Brushes 1");
		AzFramework::ConsoleRequestBus::Broadcast(&AzFramework::ConsoleRequestBus::Events::ExecuteConsoleCommand, "e_Sun 1");
		AzFramework::ConsoleRequestBus::Broadcast(&AzFramework::ConsoleRequestBus::Events::ExecuteConsoleCommand, "e_Objects 1");
		AzFramework::ConsoleRequestBus::Broadcast(&AzFramework::ConsoleRequestBus::Events::ExecuteConsoleCommand, "e_WaterOcean 1");
		AzFramework::ConsoleRequestBus::Broadcast(&AzFramework::ConsoleRequestBus::Events::ExecuteConsoleCommand, "e_GeomCaches 1");
		AzFramework::ConsoleRequestBus::Broadcast(&AzFramework::ConsoleRequestBus::Events::ExecuteConsoleCommand, "e_Roads 1");
		AzFramework::ConsoleRequestBus::Broadcast(&AzFramework::ConsoleRequestBus::Events::ExecuteConsoleCommand, "e_WaterVolumes 1");
		AzFramework::ConsoleRequestBus::Broadcast(&AzFramework::ConsoleRequestBus::Events::ExecuteConsoleCommand, "e_Particles 1");

	}
	else 
	{
		AZStd::string s_Terrain = AZStd::string::format("e_Terrain %d", m_Terrain);
		AzFramework::ConsoleRequestBus::Broadcast(&AzFramework::ConsoleRequestBus::Events::ExecuteConsoleCommand, s_Terrain.c_str());

		AZStd::string s_Entities = AZStd::string::format("e_Entities %d", m_Entities);
		AzFramework::ConsoleRequestBus::Broadcast(&AzFramework::ConsoleRequestBus::Events::ExecuteConsoleCommand, s_Entities.c_str());

		AZStd::string s_Decals = AZStd::string::format("e_Decals %d", m_Decals);
		AzFramework::ConsoleRequestBus::Broadcast(&AzFramework::ConsoleRequestBus::Events::ExecuteConsoleCommand, s_Decals.c_str());

		AZStd::string s_Fog = AZStd::string::format("e_Fog %d", m_Fog);
		AzFramework::ConsoleRequestBus::Broadcast(&AzFramework::ConsoleRequestBus::Events::ExecuteConsoleCommand, s_Fog.c_str());

		AZStd::string s_SkyBox = AZStd::string::format("e_SkyBox %d", m_SkyBox);
		AzFramework::ConsoleRequestBus::Broadcast(&AzFramework::ConsoleRequestBus::Events::ExecuteConsoleCommand, s_SkyBox.c_str());

		AZStd::string s_Vegetation = AZStd::string::format("e_Vegetation %d", m_Vegetation);
		AzFramework::ConsoleRequestBus::Broadcast(&AzFramework::ConsoleRequestBus::Events::ExecuteConsoleCommand, s_Vegetation.c_str());

		AZStd::string s_Brushes = AZStd::string::format("e_Brushes %d", m_Brushes);
		AzFramework::ConsoleRequestBus::Broadcast(&AzFramework::ConsoleRequestBus::Events::ExecuteConsoleCommand, s_Brushes.c_str());

		AZStd::string s_Sun = AZStd::string::format("e_Sun %d", m_Sun);
		AzFramework::ConsoleRequestBus::Broadcast(&AzFramework::ConsoleRequestBus::Events::ExecuteConsoleCommand, s_Sun.c_str());

		AZStd::string s_Objects = AZStd::string::format("e_Objects %d", m_Objects);
		AzFramework::ConsoleRequestBus::Broadcast(&AzFramework::ConsoleRequestBus::Events::ExecuteConsoleCommand, s_Objects.c_str());

		AZStd::string s_WaterOcean = AZStd::string::format("e_WaterOcean %d", m_WaterOcean);
		AzFramework::ConsoleRequestBus::Broadcast(&AzFramework::ConsoleRequestBus::Events::ExecuteConsoleCommand, s_WaterOcean.c_str());

		AZStd::string s_GeomCaches = AZStd::string::format("e_GeomCaches %d", m_GeomCaches);
		AzFramework::ConsoleRequestBus::Broadcast(&AzFramework::ConsoleRequestBus::Events::ExecuteConsoleCommand, s_GeomCaches.c_str());

		AZStd::string s_Roads = AZStd::string::format("e_Roads %d", m_Roads);
		AzFramework::ConsoleRequestBus::Broadcast(&AzFramework::ConsoleRequestBus::Events::ExecuteConsoleCommand, s_Roads.c_str());

		AZStd::string s_WaterVolumes = AZStd::string::format("e_WaterVolumes %d", m_WaterVolumes);
		AzFramework::ConsoleRequestBus::Broadcast(&AzFramework::ConsoleRequestBus::Events::ExecuteConsoleCommand, s_WaterVolumes.c_str());

		AZStd::string s_Particles = AZStd::string::format("e_Particles %d", m_Particles);
		AzFramework::ConsoleRequestBus::Broadcast(&AzFramework::ConsoleRequestBus::Events::ExecuteConsoleCommand, s_Particles.c_str());

	}
}

void EngineFeatureManager::OnParamChanged()
{
	UpdateAll(m_EnabledAll);



}
