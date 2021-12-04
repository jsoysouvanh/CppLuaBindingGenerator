#pragma once

#include "../LuaProperties.h"

#include <type_traits>
#include <Refureku/TypeInfo/Entity/DefaultEntityRegisterer.h>
#include <Refureku/TypeInfo/Archetypes/ArchetypeRegisterer.h>
#include <Refureku/TypeInfo/Namespace/Namespace.h>
#include <Refureku/TypeInfo/Namespace/NamespaceFragment.h>
#include <Refureku/TypeInfo/Namespace/NamespaceFragmentRegisterer.h>
#include <Refureku/TypeInfo/Archetypes/Template/TypeTemplateParameter.h>
#include <Refureku/TypeInfo/Archetypes/Template/NonTypeTemplateParameter.h>
#include <Refureku/TypeInfo/Archetypes/Template/TemplateTemplateParameter.h>

rfk::EEntityKind clb::LuaClass::getTargetEntityKind() const noexcept { return targetEntityKind; }
bool clb::LuaClass::getAllowMultiple() const noexcept { return allowMultiple; }
bool clb::LuaClass::getShouldInherit() const noexcept { return shouldInherit; }
static_assert(std::is_base_of_v<rfk::Property, clb::LuaClass>, "[Refureku] Can't attach rfk::PropertySettings property to clb::LuaClass as it doesn't inherit from rfk::Property.");
rfk::EEntityKind clb::LuaVar::getTargetEntityKind() const noexcept { return targetEntityKind; }
bool clb::LuaVar::getAllowMultiple() const noexcept { return allowMultiple; }
static_assert(std::is_base_of_v<rfk::Property, clb::LuaVar>, "[Refureku] Can't attach rfk::PropertySettings property to clb::LuaVar as it doesn't inherit from rfk::Property.");
rfk::EEntityKind clb::LuaFunc::getTargetEntityKind() const noexcept { return targetEntityKind; }
bool clb::LuaFunc::getAllowMultiple() const noexcept { return allowMultiple; }
static_assert(std::is_base_of_v<rfk::Property, clb::LuaFunc>, "[Refureku] Can't attach rfk::PropertySettings property to clb::LuaFunc as it doesn't inherit from rfk::Property.");
namespace rfk::generated { 
 static rfk::NamespaceFragment const& getNamespaceFragment_16619970450467258497u_16445461527799692811() noexcept {
static rfk::NamespaceFragment fragment("clb", 16619970450467258497u);
static bool initialized = false;
if (!initialized) {
initialized = true;
fragment.setNestedEntitiesCapacity(3u);
fragment.addNestedEntity(*rfk::getArchetype<clb::LuaClass>());
fragment.addNestedEntity(*rfk::getArchetype<clb::LuaVar>());
fragment.addNestedEntity(*rfk::getArchetype<clb::LuaFunc>());
}
return fragment; }
static rfk::NamespaceFragmentRegisterer const namespaceFragmentRegisterer_16619970450467258497u_16445461527799692811(rfk::generated::getNamespaceFragment_16619970450467258497u_16445461527799692811());
 }
rfk::Class const& clb::LuaClass::staticGetArchetype() noexcept {
static bool initialized = false;
static rfk::Class type("LuaClass", 6843603869351142822u, sizeof(LuaClass), 1);
if (!initialized) {
initialized = true;
type.setPropertiesCapacity(1);
static_assert((rfk::PropertySettings::targetEntityKind & rfk::EEntityKind::Class) != rfk::EEntityKind::Undefined, "[Refureku] rfk::PropertySettings can't be applied to a rfk::EEntityKind::Class");static rfk::PropertySettings property_6843603869351142822u_0{rfk::EEntityKind::Struct | rfk::EEntityKind::Class,false,false};type.addProperty(property_6843603869351142822u_0);
type.setDirectParentsCapacity(1);
type.addDirectParent(rfk::getArchetype<rfk::Property>(), static_cast<rfk::EAccessSpecifier>(1));
LuaClass::_rfk_registerChildClass<LuaClass>(type);
static rfk::StaticMethod defaultSharedInstantiator("", 0u, rfk::getType<rfk::SharedPtr<LuaClass>>(),new rfk::NonMemberFunction<rfk::SharedPtr<LuaClass>()>(&rfk::internal::CodeGenerationHelpers::defaultSharedInstantiator<LuaClass>),rfk::EMethodFlags::Default, nullptr);
type.addSharedInstantiator(defaultSharedInstantiator);
}
return type; }

rfk::Class const& clb::LuaClass::getArchetype() const noexcept { return LuaClass::staticGetArchetype(); }

template <> rfk::Archetype const* rfk::getArchetype<clb::LuaClass>() noexcept { return &clb::LuaClass::staticGetArchetype(); }

rfk::Class const& clb::LuaVar::staticGetArchetype() noexcept {
static bool initialized = false;
static rfk::Class type("LuaVar", 9403099614887816099u, sizeof(LuaVar), 1);
if (!initialized) {
initialized = true;
type.setPropertiesCapacity(1);
static_assert((rfk::PropertySettings::targetEntityKind & rfk::EEntityKind::Class) != rfk::EEntityKind::Undefined, "[Refureku] rfk::PropertySettings can't be applied to a rfk::EEntityKind::Class");static rfk::PropertySettings property_9403099614887816099u_0{rfk::EEntityKind::Field,false};type.addProperty(property_9403099614887816099u_0);
type.setDirectParentsCapacity(1);
type.addDirectParent(rfk::getArchetype<rfk::Property>(), static_cast<rfk::EAccessSpecifier>(1));
LuaVar::_rfk_registerChildClass<LuaVar>(type);
static rfk::StaticMethod defaultSharedInstantiator("", 0u, rfk::getType<rfk::SharedPtr<LuaVar>>(),new rfk::NonMemberFunction<rfk::SharedPtr<LuaVar>()>(&rfk::internal::CodeGenerationHelpers::defaultSharedInstantiator<LuaVar>),rfk::EMethodFlags::Default, nullptr);
type.addSharedInstantiator(defaultSharedInstantiator);
}
return type; }

rfk::Class const& clb::LuaVar::getArchetype() const noexcept { return LuaVar::staticGetArchetype(); }

template <> rfk::Archetype const* rfk::getArchetype<clb::LuaVar>() noexcept { return &clb::LuaVar::staticGetArchetype(); }

rfk::Class const& clb::LuaFunc::staticGetArchetype() noexcept {
static bool initialized = false;
static rfk::Class type("LuaFunc", 10655917246322632784u, sizeof(LuaFunc), 1);
if (!initialized) {
initialized = true;
type.setPropertiesCapacity(1);
static_assert((rfk::PropertySettings::targetEntityKind & rfk::EEntityKind::Class) != rfk::EEntityKind::Undefined, "[Refureku] rfk::PropertySettings can't be applied to a rfk::EEntityKind::Class");static rfk::PropertySettings property_10655917246322632784u_0{rfk::EEntityKind::Method,false};type.addProperty(property_10655917246322632784u_0);
type.setDirectParentsCapacity(1);
type.addDirectParent(rfk::getArchetype<rfk::Property>(), static_cast<rfk::EAccessSpecifier>(1));
LuaFunc::_rfk_registerChildClass<LuaFunc>(type);
static rfk::StaticMethod defaultSharedInstantiator("", 0u, rfk::getType<rfk::SharedPtr<LuaFunc>>(),new rfk::NonMemberFunction<rfk::SharedPtr<LuaFunc>()>(&rfk::internal::CodeGenerationHelpers::defaultSharedInstantiator<LuaFunc>),rfk::EMethodFlags::Default, nullptr);
type.addSharedInstantiator(defaultSharedInstantiator);
}
return type; }

rfk::Class const& clb::LuaFunc::getArchetype() const noexcept { return LuaFunc::staticGetArchetype(); }

template <> rfk::Archetype const* rfk::getArchetype<clb::LuaFunc>() noexcept { return &clb::LuaFunc::staticGetArchetype(); }


