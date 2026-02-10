// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#ifndef icam_ICameraInfo_included
#define icam_ICameraInfo_included


// ACF includes
#include <istd/IPolymorphic.h>


namespace icam
{


/**
	Provides common information about the camera.
*/
class ICameraInfo: virtual public istd::IPolymorphic
{
public:
	enum CameraAttribute
	{
		CA_VENDOR,
		CA_MODEL,
		CA_DEVICE_VERSION,
		CA_FIRMWARE_VERSION,
		CA_DEVICE_ID,
		CA_DEVICE_NAME,
		CA_DEVICE_CLASS,
		CA_SERIAL_NUMBER,
		CA_IP_ADDRESS,
		CA_MAC_ADDRESS,
		CA_MAX_WIDTH,
		CA_MAX_HEIGHT,
		CA_PORT,
		CA_USER = 100
	};

	/**
		Get value of camera attribute.
		\param	cameraIndex	index of the camera in the enumerated list
		\param	attributeId	ID of camera attribute \sa CameraAttribute.
	*/
	virtual QString GetCameraAttribute(int cameraIndex, int attributeId) const = 0;
};


} // namespace icam


#endif // !icam_ICameraInfo_included
