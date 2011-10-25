#include "CboxPck.h"


#include "icomp/export.h"


namespace CboxPck
{


I_EXPORT_DEFAULT_SERVICES;

I_EXPORT_PACKAGE(
			"Cbox",
			"Package based on Crypto Box library CBIOS",
			"Crypto Box Dongle" IM_PROJECT("ACF-Solutions") IM_COMPANY("ImagingTools") IM_AUTHOR("\"Witold Gantzke\" \"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			MemoryBankSerializer,
			"Load and save objects from/to Crypto Box memory",
			"Serializer Archive Load Save Encryption Decryption Crypto Box CBIOS Dongle RAM Partition Memory" IM_TAG("Loader"));


} // namespace CboxPck


