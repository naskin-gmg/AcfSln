#ifndef icalib_CSimpleLensCorrection_included
#define icalib_CSimpleLensCorrection_included


// ACF includes
#include <i2d/ICalibration2d.h>
#include <i2d/CVector2d.h>


namespace icalib
{


/**	
	Represents simple correction of image distortion by lenses.
*/
class CSimpleLensCorrection: virtual public i2d::ICalibration2d
{
public:
	CSimpleLensCorrection();
	explicit CSimpleLensCorrection(const i2d::CVector2d& opticalCenter, double distortionFactor = 0, double scaleFactor = 1);

	/**
		Reset this calibration, set to be identity transform.
	*/
	void Reset(const i2d::CVector2d& opticalCenter = i2d::CVector2d::GetZero(), double distortionFactor = 0, double scaleFactor = 1);

	const i2d::CVector2d& GetOpticalCenter() const;
	virtual void SetOpticalCenter(const i2d::CVector2d& center);

	double GetDistortionFactor() const;
	virtual void SetDistortionFactor(double factor);

	double GetScaleFactor() const;
	virtual void SetScaleFactor(double factor);

	bool operator==(const CSimpleLensCorrection& calib) const;
	bool operator!=(const CSimpleLensCorrection& calib) const;

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

	// reimplemented (imath::TISurjectFunction)
	virtual bool GetInvValueAt(const i2d::CVector2d& argument, i2d::CVector2d& result) const override;
	virtual i2d::CVector2d GetInvValueAt(const i2d::CVector2d& argument) const override;

	// reimplemented (imath::TIMathFunction)
	virtual bool GetValueAt(const i2d::CVector2d& argument, i2d::CVector2d& result) const override;
	virtual i2d::CVector2d GetValueAt(const i2d::CVector2d& argument) const override;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive) override;

	// reimplemented (istd::IChangeable)
	virtual int GetSupportedOperations() const override;
	virtual bool CopyFrom(const istd::IChangeable& object, CompatibilityMode mode = CM_WITHOUT_REFS) override;
	virtual bool IsEqual(const istd::IChangeable& object) const override;
	virtual istd::IChangeableUniquePtr CloneMe(CompatibilityMode mode = CM_WITHOUT_REFS) const override;
	virtual bool ResetData(CompatibilityMode mode = CM_WITHOUT_REFS) override;

private:
	i2d::CVector2d m_opticalCenter;

	double m_distortionFactor;
	double m_scaleFactor;
};


// inline methods

inline bool CSimpleLensCorrection::operator!=(const CSimpleLensCorrection& calib) const
{
	return !operator==(calib);
}


} // namespace icalib


#endif // !icalib_CSimpleLensCorrection_included


