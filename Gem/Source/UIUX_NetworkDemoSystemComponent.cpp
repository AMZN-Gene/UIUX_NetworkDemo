
#include <AzCore/Console/ILogger.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Serialization/EditContextConstants.inl>

#include "UIUX_NetworkDemoSystemComponent.h"
#include <Source/AutoGen/AutoComponentTypes.h>

#include "Spawners/RoundRobinSpawner.h"
#include <Multiplayer/IMultiplayer.h>

namespace UIUX_NetworkDemo
{
    void UIUX_NetworkDemoSystemComponent::Reflect(AZ::ReflectContext* context)
    {
        if (AZ::SerializeContext* serialize = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serialize->Class<UIUX_NetworkDemoSystemComponent, AZ::Component>()
                ->Version(0)
                ;

            if (AZ::EditContext* ec = serialize->GetEditContext())
            {
                ec->Class<UIUX_NetworkDemoSystemComponent>("UIUX_NetworkDemo", "[Description of functionality provided by this System Component]")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                        ->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC("System"))
                        ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                    ;
            }
        }
    }

    void UIUX_NetworkDemoSystemComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        provided.push_back(AZ_CRC("UIUX_NetworkDemoService"));
    }

    void UIUX_NetworkDemoSystemComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        incompatible.push_back(AZ_CRC("UIUX_NetworkDemoService"));
    }

    void UIUX_NetworkDemoSystemComponent::GetRequiredServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& required)
    {
    }

    void UIUX_NetworkDemoSystemComponent::GetDependentServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& dependent)
    {
    }

    UIUX_NetworkDemoSystemComponent::UIUX_NetworkDemoSystemComponent()
    {
        if (UIUX_NetworkDemoInterface::Get() == nullptr)
        {
            UIUX_NetworkDemoInterface::Register(this);
        }
    }

    UIUX_NetworkDemoSystemComponent::~UIUX_NetworkDemoSystemComponent()
    {
        if (UIUX_NetworkDemoInterface::Get() == this)
        {
            UIUX_NetworkDemoInterface::Unregister(this);
        }
    }

    void UIUX_NetworkDemoSystemComponent::Init()
    {
    }

    void UIUX_NetworkDemoSystemComponent::Activate()
    {
        UIUX_NetworkDemoRequestBus::Handler::BusConnect();
        RegisterMultiplayerComponents(); //< Register our gems multiplayer components to assign NetComponentIds
    
        AZ::Interface<Multiplayer::IMultiplayerSpawner>::Register(this);
        m_playerSpawner = AZStd::make_unique<RoundRobinSpawner>();
        AZ::Interface<UIUX_NetworkDemo::IPlayerSpawner>::Register(m_playerSpawner.get());
    }

    void UIUX_NetworkDemoSystemComponent::Deactivate()
    {
        UIUX_NetworkDemoRequestBus::Handler::BusDisconnect();
    }

        Multiplayer::NetworkEntityHandle UIUX_NetworkDemoSystemComponent::OnPlayerJoin(
        [[maybe_unused]] uint64_t userId, [[maybe_unused]] const Multiplayer::MultiplayerAgentDatum& agentDatum)
    {
        AZStd::pair<Multiplayer::PrefabEntityId, AZ::Transform> entityParams = AZ::Interface<IPlayerSpawner>::Get()->GetNextPlayerSpawn();

        Multiplayer::INetworkEntityManager::EntityList entityList =
            AZ::Interface<Multiplayer::IMultiplayer>::Get()->GetNetworkEntityManager()->CreateEntitiesImmediate(
            entityParams.first, Multiplayer::NetEntityRole::Authority, entityParams.second, Multiplayer::AutoActivate::DoNotActivate);

        for (Multiplayer::NetworkEntityHandle subEntity : entityList)
        {
            subEntity.Activate();
        }

        Multiplayer::NetworkEntityHandle controlledEntity;
        if (!entityList.empty())
        {
            controlledEntity = entityList[0];
        }
        else
        {
            AZLOG_WARN("Attempt to spawn prefab %s failed. Check that prefab is network enabled.",
                entityParams.first.m_prefabName.GetCStr());
        }

        return controlledEntity;
    }

    void UIUX_NetworkDemoSystemComponent::OnPlayerLeave(
        Multiplayer::ConstNetworkEntityHandle entityHandle, [[maybe_unused]] const Multiplayer::ReplicationSet& replicationSet, [[maybe_unused]] AzNetworking::DisconnectReason reason)
    {
        AZ::Interface<Multiplayer::IMultiplayer>::Get()->GetNetworkEntityManager()->MarkForRemoval(entityHandle);
    }
}
