// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#ifndef icalib_CCalibration2dProxyComp_included
#define icalib_CCalibration2dProxyComp_included


// ACF includes
#include <icomp/CComponentBase.h>
#include <imod/CModelUpdateBridge.h>
#include <i2d/ICalibration2d.h>
#include <i2d/ICalibrationProvider.h>


namespace icalib
{


/**
	Implementation of a proxy of ICalibration2d interface realized over ICalibrationProvider access.
	Behind this proxy a ICalibrationProvider component reference is used for interface implementation
*/
class CCalibration2dProxyComp:
			public icomp::CComponentBase,
			public i2d::ICalibration2d
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CCalibration2dProxyComp);
		I_REGISTER_INTERFACE(ICalibration2d);
		I_REGISTER_INTERFACE(ITransformation2d);
		I_REGISTER_INTERFACE(istd::IChangeable);
		I_ASSIGN(m_calibrationProviderCompPtr, "CalibrationProvider", "Provider of a 2D calibration", true, "CalibrationProvider");
		I_ASSIGN_TO(m_calibrationProviderModelCompPtr, m_calibrationProviderCompPtr, true);
		I_ASSIGN(m_defaultCalibrationProviderCompPtr, "DefaultCalibrationProvider", "Provides default (fallback) calibration if the input calibration provider was failed", false, "DefaultCalibrationProvider");
	I_END_COMPONENT;

	CCalibration2dProxyComp();

	// reimplemented (i2d::ICalibration2d)
	virtual const i2d::CRectangle* GetArgumentArea() const override;
	virtual const i2d::CRectangle* GetResultArea() const override;
	virtual const imath::IUnitInfo* GetArgumentUnitInfo() const override;
	virtual const imath::IUnitInfo* GetResultUnitInfo() const override;
	virtual istd::TUniqueInterfacePtr<i2d::ICalibration2d> CreateCombinedCalibration(const ITransformation2d& transformation) const override;

	// reimplemented (i2d::ITransformation2d)
	virtual int GetTransformationFlags() const override;
	virtual bool GetDistance(
				const i2d::CVector2d& origPos1,
				const i2d::CVector2d& origPos2,
				double& result,
				ExactnessMode mode = EM_NONE) const override;
	virtual bool GetPositionAt(
				const i2d::CVector2d& origPosition,
				i2d::CVector2d& result,
				ExactnessMode mode = EM_NONE) const override;
	virtual bool GetInvPositionAt(
				const i2d::CVector2d& transfPosition,
				i2d::CVector2d& result,
				ExactnessMode mode = EM_NONE) const override;
	virtual bool GetLocalTransform(
				const i2d::CVector2d& origPosition,
				i2d::CAffine2d& result,
				ExactnessMode mode = EM_NONE) const override;
	virtual bool GetLocalInvTransform(
				const i2d::CVector2d& transfPosition,
				i2d::CAffine2d& result,
				ExactnessMode mode = EM_NONE) const override;

	// reimplemented (imath::TISurjectFunction<i2d::CVector2d, i2d::CVector2d>)
	virtual bool GetInvValueAt(const i2d::CVector2d& argument, i2d::CVector2d& result) const override;
	virtual i2d::CVector2d GetInvValueAt(const i2d::CVector2d& argument) const override;

	// reimplemented (imath::TIMathFunction<i2d::CVector2d, i2d::CVector2d>)
	virtual bool GetValueAt(const i2d::CVector2d& argument, i2d::CVector2d& result) const override;
	virtual i2d::CVector2d GetValueAt(const i2d::CVector2d& argument) const override;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive) override;

	// reimplemented (istd::IChangeable)
	virtual istd::IChangeableUniquePtr CloneMe(CompatibilityMode mode = CM_WITHOUT_REFS) const;

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated() override;
	virtual void OnComponentDestroyed() override;

private:
	const i2d::ICalibration2d* GetCalibration() const;

private:
	I_REF(i2d::ICalibrationProvider, m_calibrationProviderCompPtr);
	I_REF(imod::IModel, m_calibrationProviderModelCompPtr);
	I_REF(i2d::ICalibrationProvider, m_defaultCalibrationProviderCompPtr);

	imod::CModelUpdateBridge m_updateBridge;
};


} // namespace icalib


#endif // !icalib_CCalibration2dProxyComp_included


