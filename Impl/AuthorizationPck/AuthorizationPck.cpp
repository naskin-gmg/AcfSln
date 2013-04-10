#include "AuthorizationPck.h"


#include "icomp/export.h"


namespace AuthorizationPck
{


I_EXPORT_PACKAGE(
			"AcfSln/AuthorizationPck",
			"Package for security and authorization components",
			IM_PROJECT("ACF-Solutions") IM_COMPANY("ImagingTools") IM_TAG("Authorization") IM_AUTHOR("\"Witold Gantzke\" \"Kirill Lepskiy\""));


I_EXPORT_COMPONENT(
			StaticRightsProvider,
			"Static rights provider",
			"User Rights" IM_TAG("\"User Management\""));

I_EXPORT_COMPONENT(
			UserLogin,
			"User login and authotization mechanism using user manager",
			"Authentication User Rights User Manager" IM_TAG("\"User Management\""));

I_EXPORT_COMPONENT(
			SimpleLogin,
			"Simple static login logic without any user manager",
			"Basic Simple Static Authentication User Rights" IM_TAG("\"User Management\""));

I_EXPORT_COMPONENT(
			UsersManager,
			"Allows manage of list of users",
			"Authentication Manager User Rights" IM_TAG("\"User Management\""));

I_EXPORT_COMPONENT(
			RightsBasedEnabler,
			"Enabling state based on a rights provider",
			"Authentication Rights Enable Allow" IM_TAG("\"User Management\""));


} // namespace AuthorizationPck


