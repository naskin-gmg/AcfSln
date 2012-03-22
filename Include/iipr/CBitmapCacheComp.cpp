#include "iipr/CBitmapCacheComp.h"


namespace iipr
{


// reimplemented (iipr::IBitmapProvider)

const iimg::IBitmap* CBitmapCacheComp::GetBitmap() const
{
	return m_bitmapPtr.GetPtr();
}


// reimplemented (i2d::ICalibrationProvider)

const i2d::ITransformation2d* CBitmapCacheComp::GetLogicalTransform() const
{
	return m_transformPtr.GetPtr();
}


// reimplemented (istd::IChangeable)

bool CBitmapCacheComp::CopyFrom(const IChangeable& object)
{
	const IBitmapProvider* providerPtr = dynamic_cast<const IBitmapProvider*>(&object);
	if (providerPtr != NULL){
		m_bitmapPtr.Reset();
		const iimg::IBitmap* bitmapPtr = providerPtr->GetBitmap();
		if (bitmapPtr != NULL){
			m_bitmapPtr.SetCastedOrRemove(bitmapPtr->CloneMe());
		}

		m_transformPtr.Reset();

		const i2d::ICalibrationProvider* calibrationProviderPtr = dynamic_cast<const i2d::ICalibrationProvider*>(&object);
		if (calibrationProviderPtr != NULL){
			const i2d::ITransformation2d* transformPtr = calibrationProviderPtr->GetLogicalTransform();
			if (transformPtr != NULL){
				m_transformPtr.SetCastedOrRemove(transformPtr->CloneMe());
			}
		}

		return true;
	}

	return false;
}


} // namespace iipr


