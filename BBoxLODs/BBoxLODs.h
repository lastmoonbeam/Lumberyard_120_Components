#pragma once

#include <AzCore/Component/Component.h>
#include <AzCore/Component/TickBus.h>
#include <AzCore/Component/EntityBus.h>
#include <AzFramework/Entity/GameEntityContextBus.h>
#include <AzCore/Component/TransformBus.h>

#include <AzCore/Asset/AssetCommon.h>
#include <AzCore/Script/ScriptProperty.h>
#include <AzCore/std/smart_ptr/unique_ptr.h>

#include <Integration/Assets/MotionAsset.h>
#include <Integration/ActorComponentBus.h>

namespace GameProject
{
	class BBoxLODComponent
		: public AZ::Component
		, private AzFramework::GameEntityContextEventBus::Handler
		, private AZ::TickBus::Handler
		, private EMotionFX::Integration::ActorComponentNotificationBus::Handler
	{
	public:
		AZ_COMPONENT(BBoxLODComponent, "{4D7A9962-4BB1-41F8-91E9-1A6819C1A2E9}", AZ::Component);

		BBoxLODComponent() = default;
		virtual ~BBoxLODComponent() {};

		// AZ::Component
		static void Reflect(AZ::ReflectContext* context);

		void Init() override;
		void Activate() override;
		void Deactivate() override;


		//GameEntityContextEventBus
		void OnGameEntitiesStarted() override;

	private:
		// ActorComponentNotificationBus::Handler
		void OnActorInstanceCreated(EMotionFX::ActorInstance* actorInstance) override;
		void OnActorInstanceDestroyed(EMotionFX::ActorInstance* actorInstance) override;

		// AZ::TickBus::Handler
		void OnTick(float deltaTime, AZ::ScriptTimePoint time) override;
		int  GetTickOrder() override { return  AZ::ComponentTickBus::TICK_GAME + 1; };

		void UpdateLODLevelByBoundingBox(EMotionFX::ActorInstance* actorInstance, const AZStd::vector<float>& distances, AZ::EntityId entityId);
		void DrawRect(AZ::Vector2 min, AZ::Vector2 max);
		void DrawLODLabel(AZ::Vector2 pos, float lod, float ratio);
		void GenerateDefaultRatioLODs(AZ::u32 numLodLevels);
		AZ::u32 GetLODByBoundingBoxSquare(const AZStd::vector<float>& squares, float square);

		EMotionFX::ActorInstance*                   m_actorInstance = nullptr;        // Associated actor instance (retrieved from Actor Component).

		AZStd::vector<float> m_lodSquares;         // Lod squares that decide which lod the actor should choose.
		bool m_debugBBoxBounds = false;
		AZ::Vector3 m_minbb = AZ::Vector3(-0.5, -0.5, 0);
		AZ::Vector3 m_maxbb = AZ::Vector3(0.5, 0.5, 2);
		float m_LODBoundingBoxFactor = 10.0f;
		AZ::u32 m_maxLOD = 0;

	};
}