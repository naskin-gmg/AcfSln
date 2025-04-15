#ifndef icalib_CNoneCalibration2d
#define icalib_CNoneCalibration2d


// ACF includes
#include <i2d/ICalibration2d.h>


namespace icalib
{


/**
	Identity calibration, no transformation will be done.
*/
class CNoneCalibration2d: virtual public i2d::ICalibration2d
{
public:
	// reimplemented (i2d::ICalibration2d)
	virtual const i2d::CRectangle* GetArgumentArea() const override;
	virtual const i2d::CRectangle* GetResultArea() const override;
	virtual const imath::IUnitInfo* GetArgumentUnitInfo() const override;
	virtual const imath::IUnitInfo* GetResultUnitInfo() const override;
	virtual const ICalibration2d* CreateCombinedCalibration(const ITransformation2d& transformation) const override;

	// reimplemented (i2d::ITransformation2d)
	virtual int GetTransformationFlags() const override;
	virtual bool GetDistance(
				const i2d::CVector2d& origPos1,
				const i2d::CVector2d& origPos2,
				double& result,
				i2d::ITransformation2d::ExactnessMode mode = EM_NONE) const override;
	virtual bool GetLocalTransform(const i2d::CVector2d& logPosition, i2d::CAffine2d& result, ExactnessMode mode = EM_NONE) const override;
	virtual bool GetLocalInvTransform(const i2d::CVector2d& viewPosition, i2d::CAffine2d& result, ExactnessMode mode = EM_NONE) const override;
	virtual bool GetPositionAt(const i2d::CVector2d& viewPosition, i2d::CVector2d& result, ExactnessMode mode = EM_NONE) const override;
	virtual bool GetInvPositionAt(const i2d::CVector2d& logPosition, i2d::CVector2d& result, ExactnessMode mode = EM_NONE) const override;

	// reimplemented (imath::TISurjectFunction)
	virtual bool GetInvValueAt(const i2d::CVector2d& argument, i2d::CVector2d& result) const override;
	virtual i2d::CVector2d GetInvValueAt(const i2d::CVector2d& argument) const override;

	// reimplemented (imath::TIMathFunction)
	virtual bool GetValueAt(const i2d::CVector2d& argument, i2d::CVector2d& result) const override;
	virtual i2d::CVector2d GetValueAt(const i2d::CVector2d& argument) const override;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive) override;

	// static methods
	static const CNoneCalibration2d& GetInstance();

private:
	// static attributes
	static CNoneCalibration2d s_defaultInstance;
};


} // namespace icalib


#endif // !icalib_CNoneCalibration2d


