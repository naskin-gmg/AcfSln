// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#ifndef icam_CExposureParamsComp_included
#define icam_CExposureParamsComp_included


#include <icomp/CComponentBase.h>

#include <icam/IExposureParams.h>
#include <icam/IExposureConstraints.h>
#include <iprm/ISelectionParam.h>
#include <imod/CSingleModelObserverBase.h>
#include <imod/IModel.h>


namespace icam
{


class CExposureParamsComp: public icomp::CComponentBase, public imod::CSingleModelObserverBase, virtual public IExposureParams
{
public:
	typedef icomp::CComponentBase BaseClass;
	typedef imod::CSingleModelObserverBase BaseClass2;

	I_BEGIN_COMPONENT(CExposureParamsComp);
		I_REGISTER_INTERFACE(IExposureParams);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_ASSIGN(m_exposureConstraintsCompPtr, "Constraints", "Information about valid exposure parameters", false, "Constraints");
		I_ASSIGN(m_cameraSelectionCompPtr, "CameraSelection", "Camera selection", false, "CameraSelection");
		I_ASSIGN_TO(m_cameraSelectionModelCompPtr, m_cameraSelectionCompPtr, false);
		I_ASSIGN(m_defaultShutterTimeAttrPtr, "DefaultShutterTime", "Default value of shutter time in seconds", true, 0.001);
		I_ASSIGN(m_defaultGainAttrPtr, "DefaultGain", "Default value of gain", true, 0);
	I_END_COMPONENT;

	CExposureParamsComp();

	// reimplemented (icam::IExposureParams)
	virtual const IExposureConstraints* GetExposureConstraints() const override;
	virtual double GetShutterTime() const override;
	virtual bool SetShutterTime(double time) override;
	virtual double GetDelayTime() const override;
	virtual bool SetDelayTime(double time) override;
	virtual double GetEenDelay() const override;
	virtual bool SetEenDelay(double time) override;
	virtual int GetGainValue() const override;
	virtual bool SetGainValue(int gain) override;
	virtual int GetCurrentCameraIndex() const override;


	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive) override;

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated() override;
	virtual void OnComponentDestroyed() override;

	// reimplement (imod::CSingleModelObserverBase)
	virtual void OnUpdate(const istd::IChangeable::ChangeSet& changeSet) override;

private:
	I_REF(IExposureConstraints, m_exposureConstraintsCompPtr);
	I_REF(iprm::ISelectionParam, m_cameraSelectionCompPtr);
	I_REF(imod::IModel, m_cameraSelectionModelCompPtr);
	I_ATTR(double, m_defaultShutterTimeAttrPtr);
	I_ATTR(int, m_defaultGainAttrPtr);

	double m_shutterTime;
	double m_delayTime;
	double m_eenDelayTime;
	int m_gain;
};


} // namespace icam


#endif // !icam_CExposureParamsComp_included


