#include "iipr/CBitmapJoinerSupplierComp.h"


#include <iipr/CBitmapOperations.h>


namespace iipr
{


// reimplemented (iipr::IBitmapProvider)

const iimg::IBitmap* CBitmapJoinerSupplierComp::GetBitmap() const
{
	const ProductType* productPtr = GetWorkProduct();
	if (productPtr != NULL){
		return productPtr->second.GetPtr();
	}

	return NULL;
}


// reimplemented (i2d::ICalibrationProvider)

const i2d::ITransformation2d* CBitmapJoinerSupplierComp::GetLogTransform() const
{
	const ProductType* productPtr = GetWorkProduct();
	if (productPtr != NULL){
		return productPtr->first;
	}
	else{
		return NULL;
	}
}

// reimplemented (iproc::TSupplierCompWrap)

int CBitmapJoinerSupplierComp::ProduceObject(ProductType& result) const
{
	if (!m_inputBitmapsProviderCompPtr.IsValid()){
		return WS_CRITICAL;
	}

	if (!EnsureBitmapCreated(result)){
		return WS_CRITICAL;
	}
	
	I_ASSERT(result.second.IsValid());

	result.first = NULL;

	int inputBitmapsCount = m_inputBitmapsProviderCompPtr->GetBitmapsCount();

	if (inputBitmapsCount <= 0){
		return WS_ERROR;
	}

	QVector<const iimg::IBitmap*> inputBitmaps;

	for (int inputBitmapIndex = 0; inputBitmapIndex < inputBitmapsCount; inputBitmapIndex++){
	
		const iimg::IBitmap* bitmapPtr = m_inputBitmapsProviderCompPtr->GetBitmap(inputBitmapIndex);
		if (bitmapPtr == NULL){
			SendErrorMessage(0, "Input image could not be provided");

			return WS_ERROR;
		}
	
		inputBitmaps.push_back(bitmapPtr);
	}

	CBitmapOperations::JoinMode joinMode = *m_useHorizontalJoinAttrPtr ? CBitmapOperations::JM_HORIZONTAL : CBitmapOperations::JM_VERTICAL;

	return CBitmapOperations::JoinBitmaps(
				inputBitmaps,
				joinMode,
				*result.second.GetPtr(),
				const_cast<CBitmapJoinerSupplierComp*>(this)) ? WS_OK : WS_ERROR;
}


bool CBitmapJoinerSupplierComp::EnsureBitmapCreated(ProductType& result) const
{
	if (!m_bitmapCompFact.IsValid()){
		return false;
	}

	if (!result.second.IsValid()){
		result.second.SetPtr(m_bitmapCompFact.CreateInstance());
	}

	return result.second.IsValid();
}


// reimplemented (icomp::CComponentBase)

void CBitmapJoinerSupplierComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_inputBitmapsProviderModelCompPtr.IsValid()){
		RegisterSupplierInput(m_inputBitmapsProviderModelCompPtr.GetPtr());
	}

	if (m_calibrationModelCompPtr.IsValid()){
		RegisterSupplierInput(m_calibrationModelCompPtr.GetPtr());
	}
}


} // namespace iipr


