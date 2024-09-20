#ifndef COMMON_API_CONTROLLABLE_PROCESS_PLUGIN_HPP
#define COMMON_API_CONTROLLABLE_PROCESS_PLUGIN_HPP

#include "plugin/defines.hpp"
#include "controllableProcess.hpp"

#define COMMONAPICONTROLLABLEPROCESS_PLUGIN_CREATOR_NAME  commonapi_controllable_process_create_plugin

#define COMMONAPICONTROLLABLEPROCESS_PLUGIN_CREATOR_NAME_STR  COMMONAPI_XSTR(COMMONAPICONTROLLABLEPROCESS_PLUGIN_CREATOR_NAME)


COMMONAPI_DECLARE_PLUGIN_CREATOR(::reactorFramework::controllableprocess::ControllableProcess, COMMONAPICONTROLLABLEPROCESS_PLUGIN_CREATOR_NAME, _services);


#define COMMONAPI_DEFINE_CONTROLLABLEPROCESS_PLUGIN_CREATOR(_services)  COMMONAPI_DECLARE_PLUGIN_CREATOR(::reactorFramework::controllableprocess::ControllableProcess, COMMONAPICONTROLLABLEPROCESS_PLUGIN_CREATOR_NAME, _services)


#endif