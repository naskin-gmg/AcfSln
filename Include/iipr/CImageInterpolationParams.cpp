#include "iipr/CImageInterpolationParams.h"


// ACF includes
#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


#include "istd/TChangeNotifier.h"


namespace iipr
{


// public methods

CImageInterpolationParams::CImageInterpolationParams()
	:m_interpolationMode(IM_NO_INTERPOLATION)
{
}


// reimplemented (iipr::IImageInterpolationParams)

int CImageInterpolationParams::GetInterpolationMode() const
{
	return m_interpolationMode;
}


void CImageInterpolationParams::SetInterpolationMode(int interpolationMode)
{
	if (m_interpolationMode != interpolationMode){
		istd::CChangeNotifier changePtr(this);

		m_interpolationMode = interpolationMode;
	}
}
	

// reimplemented (iser::ISerializable)

bool CImageInterpolationParams::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	static iser::CArchiveTag interpolationModeTag("InterpolationMode", "Interpolation mode");
	retVal = retVal && archive.BeginTag(interpolationModeTag);
	retVal = retVal && archive.Process(m_interpolationMode);
	retVal = retVal && archive.EndTag(interpolationModeTag);

	return retVal;
}


} // namespace iipr


