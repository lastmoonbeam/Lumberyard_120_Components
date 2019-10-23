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


#include <..\..\..\Gems\PhysX\Code\Include\PhysX\NativeTypeIdentifiers.h>
#include <..\..\..\Gems\PhysX\Code\Include\PhysX\MathConversion.h>


/// ActionGame Headers
#include "ExplosionComponent.h"
#include "ExplosionBus.h"

using namespace GameProject;

bool CallbackFilter(const Physics::WorldBody * body, const Physics::Shape * shape)
{
	// we use this callback filter to collect only dynamic bodies, nor static
	if (body->GetNativeType() == PhysX::NativeTypeIdentifiers::RigidBody)
		return true;
	else
		return false;
}

class BehaviorExplosionComponentNotificationBusHandler : public ExplosionNotificationBus::Handler, public AZ::BehaviorEBusHandler
{
public:
	AZ_EBUS_BEHAVIOR_BINDER(BehaviorExplosionComponentNotificationBusHandler, "{BE41CA31-50A7-4B0B-ADD0-C72441D1A0BD}", AZ::SystemAllocator,
		OnExplosion, OnEventTwo);
	void OnExplosion() override
	{
		Call(FN_OnExplosion);
	}

	void OnEventTwo(AZ::u32 value) override
	{
		Call(FN_OnEventTwo, value);
	}
};


void ExplosionComponent::Reflect(AZ::ReflectContext * context)
{
	AZ::SerializeContext* serialize = azrtti_cast<AZ::SerializeContext*>(context);

	if (serialize)
	{
		serialize->Class<ExplosionComponent, AZ::Component >()
			->Version(1)
			->Field("applyExplosionOnActivation", &ExplosionComponent::m_ApplyExplosionOnActivation)
			->Field("radius", &ExplosionComponent::m_radius)
			->Field("explosion_power", &ExplosionComponent::m_explosion_power)
			
			;

		AZ::EditContext* edit = serialize->GetEditContext();
		if (edit)
		{
			edit->Class<ExplosionComponent>("ExplosionComponent", "")
				->ClassElement(AZ::Edit::ClassElements::EditorData, "")
				->Attribute(AZ::Edit::Attributes::Category, CATEGORY)
				->Attribute("Icon", "Editor/Icons/Components/ComponentPlaceholder.png")
				->Attribute("ViewportIcon", "Editor/Icons/Components/Viewport/ComponentPlaceholder.png")
				->Attribute("AutoExpand", true)
				->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC("Game", 0x232b318c))
				->DataElement(AZ::Edit::UIHandlers::Default, &ExplosionComponent::m_ApplyExplosionOnActivation, "ApplyExplosionOnActivation", "")
				->DataElement(AZ::Edit::UIHandlers::Default, &ExplosionComponent::m_radius, "Radius", "")
				->DataElement(AZ::Edit::UIHandlers::Default, &ExplosionComponent::m_explosion_power, "Explosion power", "")
				;
		}
	}

	if (AZ::BehaviorContext* behaviorContext = azrtti_cast<AZ::BehaviorContext*>(context))
	{
		AZ::BehaviorParameterOverrides someEventParam1 = { "Param", "Param Tooltip" };
		behaviorContext->EBus<ExplosionBus>("ExplosionBus")
			->Event("ApplyExplosion", &ExplosionBus::Events::ApplyExplosion, { { someEventParam1} })
			;

		behaviorContext->Class<ExplosionComponent>()
			->RequestBus("ExplosionBus")
			;

		behaviorContext->EBus<ExplosionNotificationBus>("ExplosionNotification", "ExplosionNotificationBus", "Notifications for the Explosion Component")
			->Attribute(AZ::Script::Attributes::Category, "Custom")
			->Handler<BehaviorExplosionComponentNotificationBusHandler>();

	}
}

void ExplosionComponent::Init()
{
	//AZ_Printf("ExplosionComponent", "Init");
}

void ExplosionComponent::Activate()
{
	//AZ_Printf("ExplosionComponent", "Activate");
	//AZ::EntityBus::Handler::BusConnect(GetEntityId());
	AzFramework::GameEntityContextEventBus::Handler::BusConnect();
	ExplosionBus::Handler::BusConnect(GetEntityId());

	if (m_ApplyExplosionOnActivation)
		ApplyExplosion();

}

void ExplosionComponent::Deactivate()
{
	//AZ::TickBus::Handler::BusDisconnect();
	//AZ::EntityBus::Handler::BusDisconnect(GetEntityId());
	AzFramework::GameEntityContextEventBus::Handler::BusDisconnect();
	ExplosionBus::Handler::BusDisconnect();
}

void ExplosionComponent::OnTick(float deltaTime, AZ::ScriptTimePoint time)
{

}

void ExplosionComponent::OnGameEntitiesStarted()
{
	//AZ_Printf("ExplosionComponent", "OnGameEntitiesStarted");
	//AZ::TickBus::Handler::BusConnect();
}

void ExplosionComponent::ApplyExplosion()
{
	//void* w = nullptr;
	//Physics::WorldRequestBus::BroadcastResult(w, &Physics::WorldRequestBus::Events::GetNativePointer);
	//PhysX::World* world = static_cast<PhysX::World*>(w);

	//if (world)
	//{
	//}

	AZ::TransformInterface* tm = AZ::TransformBus::FindFirstHandler(m_entity->GetId());

	if (tm)
	{
		AZ::Vector3 explosion_center = tm->GetWorldTranslation();
		AZ::Transform pos = AZ::Transform::CreateTranslation(explosion_center);
		AZStd::vector<Physics::OverlapHit> hits;

		// grab all worldbodies around entity by some radius
		Physics::WorldRequestBus::BroadcastResult(hits, &Physics::WorldRequestBus::Events::OverlapSphere, m_radius, pos, CallbackFilter);

		// for each worldbody we trying to get rigidbody ptr and apply impulse
		for (auto hit : hits)
		{
			physx::PxRigidDynamic* m_pxRigidActor = (physx::PxRigidDynamic*)hit.m_body->GetNativePointer();

			if (m_pxRigidActor)
			{
				// we should't add impulses to kinematic rigidbody
				auto rigidBodyFlags = m_pxRigidActor->getRigidBodyFlags();
				if (!rigidBodyFlags.isSet(physx::PxRigidBodyFlag::eKINEMATIC))
				{
					// calculate direction of impulse
					AZ::Vector3 direction = (hit.m_body->GetPosition() - explosion_center).GetNormalized();
					// add impulse
					m_pxRigidActor->addForce(PxMathConvert(direction * m_explosion_power), physx::PxForceMode::eIMPULSE);
				}
			}	
		}	
	}

	ExplosionNotificationBus::Broadcast(&ExplosionNotificationBus::Events::OnExplosion);
}




