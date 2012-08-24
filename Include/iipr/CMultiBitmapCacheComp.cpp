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


const i2d::ITransformation2d* CMultiBitmapCacheComp::GetLogTransform(int bitmapIndex) const
{
	I_ASSERT(bitmapIndex >= 0);
	I_ASSERT(bitmapIndex < GetBitmapsCount());
	I_ASSERT(m_bitmapsPtr.GetCount() == m_transformsPtr.GetCount());

	return m_transformsPtr.GetAt(bitmapIndex);
}


// reimplemented (istd::IChangeable)

bool CMultiBitmapCacheComp::CopyFrom(const IChangeable& object)
{
	const IMultiBitmapProvider* providerPtr = dynamic_cast<const IMultiBitmapProvider*>(&object);
	if (providerPtr != NULL){
		int bitmapsCount = providerPtr->GetBitmapsCount();

		m_bitmapsPtr.SetCount(bitmapsCount);
		m_transformsPtr.SetCount(bitmapsCount);

		for (int i = 0; i < bitmapsCount; ++i){
			istd::TDelPtr<iimg::IBitmap> clonedBitmapPtr;
			const iimg::IBitmap* bitmapPtr = providerPtr->GetBitmap(i);
			if (bitmapPtr != NULL){
				clonedBitmapPtr.SetCastedOrRemove(bitmapPtr->CloneMe());
			}
			m_bitmapsPtr.SetElementAt(i, clonedBitmapPtr.PopPtr());

			istd::TDelPtr<i2d::ITransformation2d> clonedTransformPtr;
			const i2d::ITransformation2d* transformPtr = providerPtr->GetLogTransform(i);
			if (transformPtr != NULL){
				clonedTransformPtr.SetCastedOrRemove(transformPtr->CloneMe());
			}
			m_transformsPtr.SetElementAt(i, clonedTransformPtr.PopPtr());
		}

		return true;
	}

	return false;
}


} // namespace iipr


