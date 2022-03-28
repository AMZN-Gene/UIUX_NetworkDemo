
#include <AzCore/Memory/SystemAllocator.h>
#include <AzCore/Module/Module.h>

#include "UIUX_NetworkDemoSystemComponent.h"
#include <Source/AutoGen/AutoComponentTypes.h>

namespace UIUX_NetworkDemo
{
    class UIUX_NetworkDemoModule
        : public AZ::Module
    {
    public:
        AZ_RTTI(UIUX_NetworkDemoModule, "{7adb7ba4-0576-4bf6-b824-44c2e4950cde}", AZ::Module);
        AZ_CLASS_ALLOCATOR(UIUX_NetworkDemoModule, AZ::SystemAllocator, 0);

        UIUX_NetworkDemoModule()
            : AZ::Module()
        {
            // Push results of [MyComponent]::CreateDescriptor() into m_descriptors here.
            m_descriptors.insert(m_descriptors.end(), {
                UIUX_NetworkDemoSystemComponent::CreateDescriptor(),
            });

            CreateComponentDescriptors(m_descriptors); //< Add this line to register your project's multiplayer components
        }

        /**
         * Add required SystemComponents to the SystemEntity.
         */
        AZ::ComponentTypeList GetRequiredSystemComponents() const override
        {
            return AZ::ComponentTypeList{
                azrtti_typeid<UIUX_NetworkDemoSystemComponent>(),
            };
        }
    };
}// namespace UIUX_NetworkDemo

AZ_DECLARE_MODULE_CLASS(Gem_UIUX_NetworkDemo, UIUX_NetworkDemo::UIUX_NetworkDemoModule)
