#include "iedge/CEdgeLineContainer.h"


namespace iedge
{


// reimplemented (i2d::IObject2d)

i2d::CVector2d CEdgeLineContainer::GetCenter() const
{
	return GetBoundingBox().GetCenter();
}


void CEdgeLineContainer::MoveCenterTo(const i2d::CVector2d& position)
{
	i2d::CVector2d diffVector = position - GetCenter();

	int contoursCount = BaseClass::GetItemsCount();
	for (int i = 0; i < contoursCount; ++i){
		CEdgeLine& edgeLine = GetAt(i);

		edgeLine.MoveCenterTo(edgeLine.GetCenter() + diffVector);
	}
}


i2d::CRectangle CEdgeLineContainer::GetBoundingBox() const
{
	i2d::CRectangle retVal = i2d::CRectangle::GetEmpty();

	int contoursCount = BaseClass::GetItemsCount();
	for (int i = 0; i < contoursCount; ++i){
		const CEdgeLine& edgeLine = GetAt(i);

		retVal.Unite(edgeLine.GetBoundingBox());
	}

	return retVal;
}


bool CEdgeLineContainer::Transform(
			const i2d::ITransformation2d& transformation,
			i2d::ITransformation2d::ExactnessMode mode,
			double* errorFactorPtr)
{
	bool retVal = true;

	double errorFactorSum = 0;

	int linesCount = BaseClass::GetItemsCount();
	for (int lineIndex = 0; lineIndex < linesCount; ++lineIndex){
		CEdgeLine& edgeLine = BaseClass::GetAt(lineIndex);

		if (errorFactorPtr != NULL){
			double factor;

			retVal = edgeLine.Transform(transformation, mode, &factor) && retVal;

			errorFactorSum += factor;
		}
		else{
			retVal = edgeLine.Transform(transformation, mode) && retVal;
		}
	}

	if (errorFactorPtr != NULL){
		*errorFactorPtr = errorFactorSum / linesCount;
	}

	return retVal;
}


bool CEdgeLineContainer::InvTransform(
			const i2d::ITransformation2d& transformation,
			i2d::ITransformation2d::ExactnessMode mode,
			double* errorFactorPtr)
{
	bool retVal = true;

	double errorFactorSum = 0;

	int linesCount = BaseClass::GetItemsCount();
	for (int lineIndex = 0; lineIndex < linesCount; ++lineIndex){
		CEdgeLine& edgeLine = BaseClass::GetAt(lineIndex);

		if (errorFactorPtr != NULL){
			double factor;

			retVal = edgeLine.InvTransform(transformation, mode, &factor) && retVal;

			errorFactorSum += factor;
		}
		else{
			retVal = edgeLine.InvTransform(transformation, mode) && retVal;
		}
	}

	if (errorFactorPtr != NULL){
		*errorFactorPtr = errorFactorSum / linesCount;
	}

	return retVal;
}


bool CEdgeLineContainer::GetTransformed(
			const i2d::ITransformation2d& transformation,
			i2d::IObject2d& result,
			i2d::ITransformation2d::ExactnessMode mode,
			double* errorFactorPtr) const
{
	CEdgeLineContainer* resultContainerPtr = dynamic_cast<CEdgeLineContainer*>(&result);
	if (resultContainerPtr != NULL){
		*resultContainerPtr = *this;

		return result.Transform(transformation, mode, errorFactorPtr);
	}
	else{
		return false;
	}
}


bool CEdgeLineContainer::GetInvTransformed(
			const i2d::ITransformation2d& transformation,
			i2d::IObject2d& result,
			i2d::ITransformation2d::ExactnessMode mode,
			double* errorFactorPtr) const
{
	CEdgeLineContainer* resultContainerPtr = dynamic_cast<CEdgeLineContainer*>(&result);
	if (resultContainerPtr != NULL){
		*resultContainerPtr = *this;

		return result.InvTransform(transformation, mode, errorFactorPtr);
	}
	else{
		return false;
	}
}


// reimplemented (istd::IChangeable)

bool CEdgeLineContainer::CopyFrom(const istd::IChangeable& object, CompatibilityMode mode)
{
	return BaseClass::CopyFrom(object, mode) && BaseClass2::CopyFrom(object, mode);
}


// protected methods

// reimplemented (ibase::TSerializableContainer)

bool CEdgeLineContainer::SerializeItem(CEdgeLine& item, iser::IArchive& archive)
{
	return item.Serialize(archive);
}


} // namespace iedge


