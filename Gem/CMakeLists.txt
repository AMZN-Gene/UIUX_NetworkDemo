
set(gem_path ${CMAKE_CURRENT_LIST_DIR})
set(gem_json ${gem_path}/gem.json)
o3de_restricted_path(${gem_json} gem_restricted_path gem_parent_relative_path)

# Currently we are in the UIUX_NetworkDemo/Code folder: ${CMAKE_CURRENT_LIST_DIR}
# Get the platform specific folder ${pal_dir} for the current folder: ${CMAKE_CURRENT_LIST_DIR}/Platform/${PAL_PLATFORM_NAME}
# Note: o3de_pal_dir will take care of the details for us, as this may be a restricted platform
#       in which case it will see if that platform is present here or in the restricted folder.
#       i.e. It could here : UIUX_NetworkDemo/Code/Platform/<platform_name>  or
#            <restricted_folder>/<platform_name>/UIUX_NetworkDemo/Code
o3de_pal_dir(pal_dir ${CMAKE_CURRENT_LIST_DIR}/Platform/${PAL_PLATFORM_NAME} ${gem_restricted_path} ${gem_path} ${gem_parent_relative_path})

# Now that we have the platform abstraction layer (PAL) folder for this folder, thats where we will find the
# traits for this platform. Traits for a platform are defines for things like whether or not something in this project
# is supported by this platform.
include(${pal_dir}/PAL_${PAL_PLATFORM_NAME_LOWERCASE}.cmake)

# Now that we have loaded our project traits for this platform, see if this project is even supported on this platform.
# If its not supported we just return after including the unsupported.
if(NOT PAL_TRAIT_UIUX_NETWORKDEMO_SUPPORTED)
    return()
endif()

# We are on a supported platform, so add the UIUX_NetworkDemo target
# Note: We include the common files and the platform specific files which are set in uiux_networkdemo_files.cmake and
# in ${pal_dir}/uiux_networkdemo_${PAL_PLATFORM_NAME_LOWERCASE}_files.cmake
ly_add_target(
    NAME UIUX_NetworkDemo.Static STATIC
    NAMESPACE Gem
    FILES_CMAKE
        uiux_networkdemo_files.cmake
        ${pal_dir}/uiux_networkdemo_${PAL_PLATFORM_NAME_LOWERCASE}_files.cmake
        uiux_networkdemo_autogen_files.cmake
    INCLUDE_DIRECTORIES
        PUBLIC
            Include
            .
    BUILD_DEPENDENCIES
        PRIVATE
            AZ::AzGameFramework
            Gem::Atom_AtomBridge.Static
            Gem::Multiplayer.Static
            Gem::PhysX.Static
        PUBLIC
            AZ::AzFramework
            AZ::AzNetworking
            Gem::Multiplayer
            Gem::PhysX
    AUTOGEN_RULES
        *.AutoComponent.xml,AutoComponent_Header.jinja,$path/$fileprefix.AutoComponent.h
        *.AutoComponent.xml,AutoComponent_Source.jinja,$path/$fileprefix.AutoComponent.cpp
        *.AutoComponent.xml,AutoComponentTypes_Header.jinja,$path/AutoComponentTypes.h
        *.AutoComponent.xml,AutoComponentTypes_Source.jinja,$path/AutoComponentTypes.cpp
)

ly_add_target(
    NAME UIUX_NetworkDemo ${PAL_TRAIT_MONOLITHIC_DRIVEN_MODULE_TYPE}
    NAMESPACE Gem
    FILES_CMAKE
        uiux_networkdemo_shared_files.cmake
        ${pal_dir}/uiux_networkdemo_shared_${PAL_PLATFORM_NAME_LOWERCASE}_files.cmake
    INCLUDE_DIRECTORIES
        PUBLIC
            Include
    BUILD_DEPENDENCIES
        PRIVATE
            Gem::UIUX_NetworkDemo.Static
            AZ::AzCore
)

# if enabled, UIUX_NetworkDemo is used by all kinds of applications
ly_create_alias(NAME UIUX_NetworkDemo.Builders NAMESPACE Gem TARGETS Gem::UIUX_NetworkDemo)
ly_create_alias(NAME UIUX_NetworkDemo.Tools    NAMESPACE Gem TARGETS Gem::UIUX_NetworkDemo)
ly_create_alias(NAME UIUX_NetworkDemo.Clients  NAMESPACE Gem TARGETS Gem::UIUX_NetworkDemo)
ly_create_alias(NAME UIUX_NetworkDemo.Servers  NAMESPACE Gem TARGETS Gem::UIUX_NetworkDemo)

################################################################################
# Gem dependencies
################################################################################

# Enable the specified list of gems from GEM_FILE or GEMS list for this specific project:
ly_enable_gems(PROJECT_NAME UIUX_NetworkDemo GEM_FILE enabled_gems.cmake)

if(PAL_TRAIT_BUILD_SERVER_SUPPORTED)
    # this property causes it to actually make a ServerLauncher.
    # if you don't want a Server application, you can remove this and the
    # following ly_enable_gems lines.
    set_property(GLOBAL APPEND PROPERTY LY_LAUNCHER_SERVER_PROJECTS UIUX_NetworkDemo)
endif()
