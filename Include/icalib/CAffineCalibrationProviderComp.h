#pragma once


// ACF includes
#include <icomp/CComponentBase.h>
#include <imod/CMultiModelDispatcherBase.h>
#include <i2d/ICalibration2d.h>
#include <i2d/ICalibrationProvider.h>
#include <icalib/CAffineCalibration2d.h>
#include <imeas/INumericValue.h>


namespace icalib
{


/**
	Provides affine calibration based on rotation, scale and translation parameters.
*/
class CAffineCalibrationProviderComp:
			public icomp::CComponentBase,
			protected imod::CMultiModelDispatcherBase,
			public i2d::ICalibrationProvider
{
public:
	typedef icomp::CComponentBase BaseClass;
	typedef imod::CMultiModelDispatcherBase BaseClass2;

	I_BEGIN_COMPONENT(CAffineCalibrationProviderComp);
		I_REGISTER_INTERFACE(i2d::ICalibrationProvider);
		I_ASSIGN(m_rotationAngleCompPtr, "RotationAngle", "Rotation angle in degree", false, "RotationAngle");
		I_ASSIGN_TO(m_rotationAngleModelCompPtr, m_rotationAngleCompPtr, false);
		I_ASSIGN(m_scaleCompPtr, "ScaleXY", "Scale values (x,y)", false, "ScaleXY");
		I_ASSIGN(m_translationCompPtr, "TranslationXY", "Translation (x,y)", false, "TranslationXY");
		I_ASSIGN_TO(m_translationModelCompPtr, m_translationCompPtr, false);
	I_END_COMPONENT;

	// reimplemented (i2d::ICalibrationProvider)
	virtual const i2d::ICalibration2d* GetCalibration() const;

protected:
	enum ModelId
	{
		MI_ROTATION,
		MI_SCALE,
		MI_TRANSLATION
	};

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

	// reimplemented (imod::CMultiModelDispatcherBase)
	virtual void OnModelChanged(int modelId, const istd::IChangeable::ChangeSet& changeSet);

private:
	void UpdateMatrix();
	void UpdateTranslation();

private:
	I_REF(imeas::INumericValue, m_rotationAngleCompPtr);
	I_REF(imod::IModel, m_rotationAngleModelCompPtr);
	I_REF(imeas::INumericValue, m_scaleCompPtr);
	I_REF(imod::IModel, m_scaleModelCompPtr);
	I_REF(imeas::INumericValue, m_translationCompPtr);
	I_REF(imod::IModel, m_translationModelCompPtr);

	icalib::CAffineCalibration2d m_affineCalibration;
};


} // namespace icalib


