#include "iipr/CMultiBitmapCacheComp.h"


// ACF includes
#include "istd/TDelPtr.h"


namespace iipr
{


CMultiBitmapCacheComp::CMultiBitmapCacheComp()
{
}


// reimplemented (iipr::IMultiBitmapProvider)

const iprm::ISelectionConstraints* CMultiBitmapCacheComp::GetBitmapSelectionContraints() const
{
	return NULL;
}


int CMultiBitmapCacheComp::GetBitmapsCount() const
{
	return m_bitmapsPtr.GetCount();
}


const iimg::IBitmap* CMultiBitmapCacheComp::GetBitmap(int bitmapIndex) const
{
	I_ASSERT(bitmapIndex >= 0);
	I_ASSERT(bitmapIndex < GetBitmapsCount());

	return m_bitmapsPtr.GetAt(bitmapIndex);
}


// reimplemented (i2d::IMultiCalibrationProvider)

const iprm::ISelectionConstraints* CMultiBitmapCacheComp::GetCalibrationSelectionContraints() const
{
	return NULL;
}


int CMultiBitmapCacheComp::GetCalibrationsCount() const
{
	return m_transformsPtr.GetCount();
}


const i2d::ICalibration2d* CMultiBitmapCacheComp::GetCalibration(int calibrationIndex) const
{
	I_ASSERT(calibrationIndex >= 0);
	I_ASSERT(calibrationIndex < GetBitmapsCount());
	I_ASSERT(m_bitmapsPtr.GetCount() == m_transformsPtr.GetCount());

	return m_transformsPtr.GetAt(calibrationIndex);
}


// reimplemented (istd::IChangeable)

bool CMultiBitmapCacheComp::CopyFrom(const IChangeable& object)
{
	bool retVal = false;

	const IMultiBitmapProvider* providerPtr = CompCastPtr<const IMultiBitmapProvider>(&object);
	if (providerPtr != NULL){
		int bitmapsCount = providerPtr->GetBitmapsCount();

		m_bitmapsPtr.SetCount(bitmapsCount);

		for (int i = 0; i < bitmapsCount; ++i){
			istd::TDelPtr<iimg::IBitmap> clonedBitmapPtr;
			const iimg::IBitmap* bitmapPtr = providerPtr->GetBitmap(i);
			if (bitmapPtr != NULL){
				clonedBitmapPtr.SetCastedOrRemove(bitmapPtr->CloneMe());
			}
			m_bitmapsPtr.SetElementAt(i, clonedBitmapPtr.PopPtr());
		}

		retVal = true;
	}

	const i2d::IMultiCalibrationProvider* calibrationProviderPtr = CompCastPtr<const i2d::IMultiCalibrationProvider>(&object);
	if (calibrationProviderPtr != NULL){
		int calibrationsCount = calibrationProviderPtr->GetCalibrationsCount();

		m_transformsPtr.SetCount(calibrationsCount);

		for (int i = 0; i < calibrationsCount; ++i){
			istd::TDelPtr<i2d::ICalibration2d> clonedTransformPtr;
			const i2d::ICalibration2d* calibrationPtr = calibrationProviderPtr->GetCalibration(i);
			if (calibrationPtr != NULL){
				clonedTransformPtr.SetCastedOrRemove(calibrationPtr->CloneMe());
			}
			m_transformsPtr.SetElementAt(i, clonedTransformPtr.PopPtr());
		}

		retVal = true;
	}

	return retVal;
}


} // namespace iipr


