#pragma once
#ifndef EXPLOSIONBUS_H
#define EXPLOSIONBUS_H

#include <AzCore/Component/ComponentBus.h>
#include <AzCore/Math/Vector3.h>

namespace GameProject
{
	class ExplosionInterface : public AZ::ComponentBus
	{
	public:
		// One handler per address is supported.
		static const AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;
		// The EBus has multiple addresses. Addresses are not ordered.
		static const AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::ById;
		// Messages are addressed by EntityId.
		using BusIdType = AZ::EntityId;
		//-For multithreaded cases where an event handler sends a new event on the same bus
		// or connects / disconnects while handling an event on the same bus, use AZStd::recursive_mutex.
		//using MutexType = AZStd::recursive_mutex;

		virtual	void ApplyExplosion() = 0;

	};
	using ExplosionBus = AZ::EBus<ExplosionInterface>;


	class ExplosionNotificationInterface : public AZ::ComponentBus
	{
	public:
		// One handler per address is supported.
		static const AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;
		// The EBus has multiple addresses. Addresses are not ordered.
		static const AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::ById;
		// Messages are addressed by EntityId.
		using BusIdType = AZ::EntityId;
		//-For multithreaded cases where an event handler sends a new event on the same bus
		// or connects / disconnects while handling an event on the same bus, use AZStd::recursive_mutex.
		//using MutexType = AZStd::recursive_mutex;

		virtual	void OnExplosion() = 0;
		virtual	void OnEventTwo(AZ::u32 value) = 0;

	};
	using ExplosionNotificationBus = AZ::EBus<ExplosionNotificationInterface>;
}
#endif