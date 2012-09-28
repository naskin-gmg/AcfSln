#include "iqtex/CXslSerializerComp.h"


// Qt includes
#include <QtCore/QObject>

// ACF includes
#include "istd/TChangeNotifier.h"

// ACF-Solutions includes
#include "iqtex/CXslTransformationReadArchive.h"
#include "iqtex/CXslTransformationWriteArchive.h"


namespace iqtex
{


int CXslSerializerComp::LoadFromFile(istd::IChangeable& data, const QString& filePath) const
{
	if (IsOperationSupported(&data, &filePath, QF_LOAD | QF_FILE, false)){
		CXslTransformationReadArchive archive(filePath, m_xslReadFilePath->GetPath());

		I_ASSERT(!archive.IsStoring());

		iser::ISerializable* serializablePtr = CompCastPtr<iser::ISerializable>(&data);
		I_ASSERT(serializablePtr != NULL);

		istd::CChangeNotifier changePtr(NULL, istd::IChangeable::CF_MODEL);

		if (serializablePtr->Serialize(archive)){
			changePtr.SetPtr(&data);

			return StateOk;
		}
		else{
			OnReadError(archive, data, filePath);
		}
	}
	return StateFailed;
}


int CXslSerializerComp::SaveToFile(const istd::IChangeable& data, const QString& filePath) const
{
	if (IsOperationSupported(&data, &filePath, QF_SAVE | QF_FILE, false)){
		CXslTransformationWriteArchive archive(filePath, m_xslWriteFilePath->GetPath(), GetVersionInfo(), this);
		I_ASSERT(archive.IsStoring());

		const iser::ISerializable* serializablePtr = CompCastPtr<iser::ISerializable>(&data);
		I_ASSERT(serializablePtr != NULL);

		if (!CheckMinimalVersion(*serializablePtr, archive.GetVersionInfo())){
			SendWarningMessage(MI_UNSUPPORTED_VERSION, QObject::tr("Archive version is not supported, possible lost of data"));
		}

		if ((const_cast<iser::ISerializable*>(serializablePtr))->Serialize(archive)){
			return StateOk;
		}
		else{
			SendInfoMessage(MI_CANNOT_SAVE, QObject::tr("Cannot serialize object to file"));
		}
	}

	return StateFailed;
}


// protected methods


void CXslSerializerComp::OnReadError(const iser::IArchive& /*archive*/, const istd::IChangeable& /*data*/, const QString& filePath) const
{
	SendWarningMessage(MI_CANNOT_LOAD, QObject::tr("Cannot load object from file ") + filePath);
}


} // namespace iqtex


