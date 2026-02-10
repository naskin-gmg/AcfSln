// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#ifndef imm_IMediaController_included
#define imm_IMediaController_included


// ACF includes
#include <istd/IChangeable.h>
#include <QtCore/QString>
#include <ifile/IFileTypeInfo.h>

#include <imm/imm.h>


namespace imm
{


class IMediaController:
			virtual public istd::IChangeable,
			virtual public ifile::IFileTypeInfo
{
public:
	/**
		Data model change notification flags.
	*/
	enum ChangeFlags
	{
		/**
			Status (start/stop) changed.
		*/
		CF_STATUS = 0xc72a266,
		/**
			Current position changed.
		*/
		CF_MEDIA_POSITION
	};

	enum SupportedFeatures
	{
		SF_OPEN_MEDIA = 0x1,
		SF_PLAY = 0x2,
		SF_SEEK = 0x4,
		SF_AUTO_PLAY = 0x8
	};

	/**
		Get URL of the medium object.
	*/
	virtual QString GetOpenedMediumUrl() const = 0;

	/**
		Open some media file.
	*/
	virtual bool OpenMediumUrl(const QString& url, bool autoPlay = true) = 0;

	/**
		Close this media player.
		It stops playing and closes media file.
	*/
	virtual void CloseMedium() = 0;

	/**
		Return true, if media is currntly playing.
	*/
	virtual bool IsPlaying() const = 0;

	/**
		Play or pause current media.
	*/
	virtual bool SetPlaying(bool state = true) = 0;

	/**
		Return length of opened media.
	*/
	virtual double GetMediumLength() const = 0;

	/**
		Get current position.
	*/
	virtual double GetCurrentPosition() const = 0;

	/**
		Skip to some position of media stream.
	*/
	virtual bool SetCurrentPosition(double position) = 0;

	/**
		Get info about features, that are supported by this media controller.
		\sa SupportedFeatures
	*/
	virtual int GetSupportedFeatures() const = 0;
};


} // namespace imm


#endif // !imm_IMediaController_included


