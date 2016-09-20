#include <iipr/CBitmapProviderCacheComp.h>


namespace iipr
{


// reimplemented (iimg::IBitmapProvider)

const iimg::IBitmap* CBitmapProviderCacheComp::GetBitmap() const
{
	return m_bitmapPtr.GetPtr();
}


// reimplemented (i2d::ICalibrationProvider)

const i2d::ICalibration2d* CBitmapProviderCacheComp::GetCalibration() const
{
	return m_transformPtr.GetPtr();
}


// reimplemented (istd::IChangeable)

bool CBitmapProviderCacheComp::CopyFrom(const IChangeable& object, CompatibilityMode mode)
{
	const iimg::IBitmapProvider* providerPtr = dynamic_cast<const iimg::IBitmapProvider*>(&object);
	if (providerPtr != NULL){
		m_bitmapPtr.Reset();
		const iimg::IBitmap* bitmapPtr = providerPtr->GetBitmap();
		if (bitmapPtr != NULL){
			m_bitmapPtr.SetCastedOrRemove(bitmapPtr->CloneMe(mode));
		}

		m_transformPtr.Reset();

		const i2d::ICalibrationProvider* calibrationProviderPtr = dynamic_cast<const i2d::ICalibrationProvider*>(&object);
		if (calibrationProviderPtr != NULL){
			const i2d::ICalibration2d* transformPtr = calibrationProviderPtr->GetCalibration();
			if (transformPtr != NULL){
				m_transformPtr.SetCastedOrRemove(transformPtr->CloneMe(mode));
			}
		}

		return true;
	}

	return false;
}


} // namespace iipr


