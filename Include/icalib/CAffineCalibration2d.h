#ifndef icalib_CAffineCalibration2d_included
#define icalib_CAffineCalibration2d_included


// ACF includes
#include <i2d/ICalibration2d.h>
#include <i2d/CAffineTransformation2d.h>
#include <i2d/CAffine2d.h>


namespace icalib
{


/**
	Definition of an affine transformation for 2D-spaces.
*/
class CAffineCalibration2d:
			public i2d::CAffineTransformation2d,
			virtual public i2d::ICalibration2d
{
public:
	typedef i2d::CAffineTransformation2d BaseClass;

	CAffineCalibration2d();

	explicit CAffineCalibration2d(const i2d::CAffine2d& transformation);

	void SetArgumentArea(const i2d::CRectangle* areaPtr);
	void SetResultArea(const i2d::CRectangle* areaPtr);
	void SetArgumentUnitInfo(const imath::IUnitInfo* unitInfoPtr);
	void SetResultUnitInfo(const imath::IUnitInfo* unitInfoPtr);

	// reimplemented (i2d::ICalibration2d)
	virtual const i2d::CRectangle* GetArgumentArea() const override;
	virtual const i2d::CRectangle* GetResultArea() const override;
	virtual const imath::IUnitInfo* GetArgumentUnitInfo() const override;
	virtual const imath::IUnitInfo* GetResultUnitInfo() const override;
	virtual istd::TUniqueInterfacePtr<i2d::ICalibration2d> CreateCombinedCalibration(const i2d::ITransformation2d& transformation) const override;

	// reimplemented (istd::IChangeable)
	virtual int GetSupportedOperations() const override;
	virtual bool CopyFrom(const istd::IChangeable& object, CompatibilityMode mode = CM_WITHOUT_REFS) override;
	virtual istd::IChangeableUniquePtr CloneMe(CompatibilityMode mode = CM_WITHOUT_REFS) const override;
	virtual bool ResetData(CompatibilityMode mode = CM_WITHOUT_REFS) override;

private:
	const i2d::CRectangle* m_argumentAreaPtr;
	const i2d::CRectangle* m_resultAreaPtr;
	const imath::IUnitInfo* m_argumentUnitInfoPtr;
	const imath::IUnitInfo* m_resultUnitInfoPtr;
};


} // namespace icalib


#endif // !icalib_CAffineTransformation2d_included


