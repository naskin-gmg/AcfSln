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
	explicit CSimpleLensCorrection(const i2d::CVector2d& opticalCenter, double distortionFactor = 0, double scaleFactor = 1, bool movableOpticalCenter = false);

	/**
		Reset this calibration, set to be identity transform.
	*/
	void Reset(const i2d::CVector2d& opticalCenter = i2d::CVector2d::GetZero(), double distortionFactor = 0, double scaleFactor = 1, bool movableOpticalCenter = false);

	const i2d::CVector2d& GetOpticalCenter() const;
	virtual void SetOpticalCenter(const i2d::CVector2d& center);

	double GetDistortionFactor() const;
	virtual void SetDistortionFactor(double factor);

	double GetScaleFactor() const;
	virtual void SetScaleFactor(double factor);

	bool GetDoDistortionOnly() const;
	virtual void SetDoDistortionOnly(bool value);

	bool operator==(const CSimpleLensCorrection& calib) const;
	bool operator!=(const CSimpleLensCorrection& calib) const;

	// reimplemented (i2d::ICalibration2d)
	virtual const i2d::CRectangle* GetArgumentArea() const;
	virtual const i2d::CRectangle* GetResultArea() const;
	virtual const imath::IUnitInfo* GetArgumentUnitInfo() const;
	virtual const imath::IUnitInfo* GetResultUnitInfo() const;
	virtual const ICalibration2d* CreateCombinedCalibration(const ITransformation2d& transformation) const;

	// reimplemented (i2d::ITransformation2d)
	virtual int GetTransformationFlags() const;
	virtual bool GetDistance(
				const i2d::CVector2d& origPos1,
				const i2d::CVector2d& origPos2,
				double& result,
				ExactnessMode mode = EM_NONE) const;
	virtual bool GetPositionAt(
				const i2d::CVector2d& origPosition,
				i2d::CVector2d& result,
				ExactnessMode mode = EM_NONE) const;
	virtual bool GetInvPositionAt(
				const i2d::CVector2d& transfPosition,
				i2d::CVector2d& result,
				ExactnessMode mode = EM_NONE) const;
	virtual bool GetLocalTransform(
				const i2d::CVector2d& origPosition,
				i2d::CAffine2d& result,
				ExactnessMode mode = EM_NONE) const;
	virtual bool GetLocalInvTransform(
				const i2d::CVector2d& transfPosition,
				i2d::CAffine2d& result,
				ExactnessMode mode = EM_NONE) const;

	// reimplemented (imath::TISurjectFunction)
	virtual bool GetInvValueAt(const i2d::CVector2d& argument, i2d::CVector2d& result) const;
	virtual i2d::CVector2d GetInvValueAt(const i2d::CVector2d& argument) const;

	// reimplemented (imath::TIMathFunction)
	virtual bool GetValueAt(const i2d::CVector2d& argument, i2d::CVector2d& result) const;
	virtual i2d::CVector2d GetValueAt(const i2d::CVector2d& argument) const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

	// reimplemented (istd::IChangeable)
	virtual int GetSupportedOperations() const;
	virtual bool CopyFrom(const istd::IChangeable& object, CompatibilityMode mode = CM_WITHOUT_REFS);
	virtual bool IsEqual(const istd::IChangeable& object) const;
	virtual istd::IChangeable* CloneMe(CompatibilityMode mode = CM_WITHOUT_REFS) const;
	virtual bool ResetData(CompatibilityMode mode = CM_WITHOUT_REFS);

private:
	i2d::CVector2d m_opticalCenter;

	double m_distortionFactor;
	double m_scaleFactor;
	
	// if this is true the optical center moved in order to its value
	// if false then the optical center has (0, 0) coord and result vector translates to m_opticalCenter.
	bool m_doDistortionOnly;  
};


// inline methods

inline bool CSimpleLensCorrection::operator!=(const CSimpleLensCorrection& calib) const
{
	return !operator==(calib);
}


} // namespace icalib


#endif // !icalib_CSimpleLensCorrection_included


