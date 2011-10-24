#ifndef iipr_CImageInterpolationParams_included
#define iipr_CImageInterpolationParams_included


// IACF includes
#include "iipr/IImageInterpolationParams.h"


namespace iipr
{


/**
	Common interace for interpolation parameters
*/
class CImageInterpolationParams: virtual public iipr::IImageInterpolationParams
{
public:
	CImageInterpolationParams();

	// reimplemented (iipr::IImageInterpolationParams)
	virtual int GetInterpolationMode() const;
	virtual void SetInterpolationMode(int interpolationMode);
	
	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	int m_interpolationMode;
};


} // namespace iipr


#endif // !iipr_CImageInterpolationParams_included


