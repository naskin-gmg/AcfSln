// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <icalib/CCalibration2dProxyComp.h>


// ACF includes
#include <imod/IModel.h>
#include <i2d/CAffine2d.h>
#include <i2d/CAffineTransformation2d.h>


namespace icalib
{


CCalibration2dProxyComp::CCalibration2dProxyComp()
:	m_updateBridge(this)
{
}


// reimplemented (i2d::ICalibration2d)

const i2d::CRectangle* CCalibration2dProxyComp::GetArgumentArea() const
{
	const i2d::ICalibration2d* calibrationPtr = GetCalibration();
	if (calibrationPtr != NULL){
		return calibrationPtr->GetArgumentArea();
	}

	return NULL;
}


const i2d::CRectangle* CCalibration2dProxyComp::GetResultArea() const
{
	const i2d::ICalibration2d* calibrationPtr = GetCalibration();
	if (calibrationPtr != NULL){
		return calibrationPtr->GetResultArea();
	}

	return NULL;
}


const imath::IUnitInfo* CCalibration2dProxyComp::GetArgumentUnitInfo() const
{
	const i2d::ICalibration2d* calibrationPtr = GetCalibration();
	if (calibrationPtr != NULL){
		return calibrationPtr->GetArgumentUnitInfo();
	}

	return NULL;
}


const imath::IUnitInfo* CCalibration2dProxyComp::GetResultUnitInfo() const
{
	const i2d::ICalibration2d* calibrationPtr = GetCalibration();
	if (calibrationPtr != NULL){
		return calibrationPtr->GetResultUnitInfo();
	}

	return NULL;
}


istd::TUniqueInterfacePtr<i2d::ICalibration2d> CCalibration2dProxyComp::CreateCombinedCalibration(const i2d::ITransformation2d& transformation) const
{
	const i2d::ICalibration2d* calibrationPtr = GetCalibration();
	if (calibrationPtr != NULL){
		return calibrationPtr->CreateCombinedCalibration(transformation);
	}

	istd::TUniqueInterfacePtr<ICalibration2d> retVal;
	retVal.MoveCastedPtr(transformation.CloneMe());

	return retVal;
}


// reimplemented (i2d::ITransformation2d)

int CCalibration2dProxyComp::GetTransformationFlags() const
{
	const i2d::ICalibration2d* calibrationPtr = GetCalibration();
	if (calibrationPtr != NULL){
		return calibrationPtr->GetTransformationFlags();
	}

	return EM_NONE;
}


bool CCalibration2dProxyComp::GetDistance(
			const i2d::CVector2d& origPos1,
			const i2d::CVector2d& origPos2,
			double& result,
			ExactnessMode mode) const
{
	const i2d::ICalibration2d* calibrationPtr = GetCalibration();
	if (calibrationPtr != NULL){
		return calibrationPtr->GetDistance(origPos1, origPos2, result, mode);
	}

	result = origPos1.GetDistance(origPos2);

	return true;
}


bool CCalibration2dProxyComp::GetPositionAt(
			const i2d::CVector2d& origPosition,
			i2d::CVector2d& result,
			ExactnessMode mode) const
{
	const i2d::ICalibration2d* calibrationPtr = GetCalibration();
	if (calibrationPtr != NULL){
		return calibrationPtr->GetPositionAt(origPosition, result, mode);
	}

	result = origPosition;

	return true;
}


bool CCalibration2dProxyComp::GetInvPositionAt(
			const i2d::CVector2d& transfPosition,
			i2d::CVector2d& result,
			ExactnessMode mode) const
{
	const i2d::ICalibration2d* calibrationPtr = GetCalibration();
	if (calibrationPtr != NULL){
		return calibrationPtr->GetInvPositionAt(transfPosition, result, mode);
	}

	result = transfPosition;

	return true;
}


bool CCalibration2dProxyComp::GetLocalTransform(
			const i2d::CVector2d& origPosition,
			i2d::CAffine2d& result,
			ExactnessMode mode) const
{
	const i2d::ICalibration2d* calibrationPtr = GetCalibration();
	if (calibrationPtr != NULL){
		return calibrationPtr->GetLocalTransform(origPosition, result, mode);
	}

	result = i2d::CAffine2d::GetIdentity();

	return true;
}


bool CCalibration2dProxyComp::GetLocalInvTransform(
				const i2d::CVector2d& transfPosition,
				i2d::CAffine2d& result,
				ExactnessMode mode) const
{
	const i2d::ICalibration2d* calibrationPtr = GetCalibration();
	if (calibrationPtr != NULL){
		return calibrationPtr->GetLocalInvTransform(transfPosition, result, mode);
	}

	result = i2d::CAffine2d::GetIdentity();

	return true;
}

// reimplemented (imath::TISurjectFunction)

bool CCalibration2dProxyComp::GetInvValueAt(const i2d::CVector2d& argument, i2d::CVector2d& result) const
{
	const i2d::ICalibration2d* calibrationPtr = GetCalibration();
	if (calibrationPtr != NULL){
		return calibrationPtr->GetInvValueAt(argument, result);
	}

	result = argument;

	return true;
}


i2d::CVector2d CCalibration2dProxyComp::GetInvValueAt(const i2d::CVector2d& argument) const
{
	const i2d::ICalibration2d* calibrationPtr = GetCalibration();
	if (calibrationPtr != NULL){
		return calibrationPtr->GetInvValueAt(argument);
	}

	return argument;
}


// reimplemented (imath::TIMathFunction)

bool CCalibration2dProxyComp::GetValueAt(const i2d::CVector2d& argument, i2d::CVector2d& result) const
{
	const i2d::ICalibration2d* calibrationPtr = GetCalibration();
	if (calibrationPtr != NULL){
		return calibrationPtr->GetValueAt(argument, result);
	}

	result = argument;

	return true;
}


i2d::CVector2d CCalibration2dProxyComp::GetValueAt(const i2d::CVector2d& argument) const
{
	const i2d::ICalibration2d* calibrationPtr = GetCalibration();
	if (calibrationPtr != NULL){
		return calibrationPtr->GetValueAt(argument);
	}

	return argument;
}


// reimplemented (iser::ISerializable)

bool CCalibration2dProxyComp::Serialize(iser::IArchive& /*archive*/)
{
	return false;
}


// reimplemented (istd::IChangeable)

istd::IChangeableUniquePtr CCalibration2dProxyComp::CloneMe(CompatibilityMode mode) const
{
	const i2d::ICalibration2d* calibrationPtr = GetCalibration();
	if (calibrationPtr != NULL){
		return calibrationPtr->CloneMe(mode);
	}

	return new i2d::CAffineTransformation2d(i2d::CAffine2d::GetIdentity());
}


// protected methods

// reimplemented (icomp::CComponentBase)

void CCalibration2dProxyComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_calibrationProviderCompPtr.IsValid() && m_calibrationProviderModelCompPtr.IsValid()){
		m_calibrationProviderModelCompPtr->AttachObserver(&m_updateBridge);
	}

	m_defaultCalibrationProviderCompPtr.EnsureInitialized();
}


void CCalibration2dProxyComp::OnComponentDestroyed()
{
	m_updateBridge.EnsureModelsDetached();

	BaseClass::OnComponentDestroyed();
}


// private methods

const i2d::ICalibration2d* CCalibration2dProxyComp::GetCalibration() const
{
	const i2d::ICalibration2d* resultPtr = NULL;

	if (m_calibrationProviderCompPtr.IsValid()){
		resultPtr = m_calibrationProviderCompPtr->GetCalibration();
	}

	if (resultPtr == NULL && m_defaultCalibrationProviderCompPtr.IsValid()){
		resultPtr = m_defaultCalibrationProviderCompPtr->GetCalibration();
	}

	return resultPtr;
}


} // namespace icalib


