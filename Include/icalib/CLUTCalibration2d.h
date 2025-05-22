#pragma once


// ACF includes
#include <i2d/ICalibration2d.h>
#include <iimg/CGeneralBitmap.h>
#include <imath/TKdTree.h>


namespace icalib
{


/**
	Definition of an affine transformation for 2D-spaces.
*/
class CLUTCalibration2d: virtual public i2d::ICalibration2d
{
public:
	CLUTCalibration2d();

	const iimg::CGeneralBitmap& GetLut() const;
	void SetLut(const iimg::IBitmap& lut);

	void SetArgumentArea(const i2d::CRectangle* areaPtr);
	void SetResultArea(const i2d::CRectangle* areaPtr);
	void SetArgumentUnitInfo(const imath::IUnitInfo* unitInfoPtr);
	void SetResultUnitInfo(const imath::IUnitInfo* unitInfoPtr);

	// reimplemented (i2d::ICalibration2d)
	virtual const i2d::CRectangle* GetArgumentArea() const override;
	virtual const i2d::CRectangle* GetResultArea() const override;
	virtual const imath::IUnitInfo* GetArgumentUnitInfo() const override;
	virtual const imath::IUnitInfo* GetResultUnitInfo() const override;
	virtual const i2d::ICalibration2d* CreateCombinedCalibration(const i2d::ITransformation2d& transformation) const override;

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

	// reimplemented (istd::IChangeable)
	virtual int GetSupportedOperations() const override;
	virtual bool CopyFrom(const istd::IChangeable& object, CompatibilityMode mode = CM_WITHOUT_REFS) override;
	virtual istd::IChangeable* CloneMe(CompatibilityMode mode = CM_WITHOUT_REFS) const override;
	virtual bool ResetData(CompatibilityMode mode = CM_WITHOUT_REFS) override;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive) override;

private:
	const i2d::CRectangle* m_argumentAreaPtr;
	const i2d::CRectangle* m_resultAreaPtr;
	const imath::IUnitInfo* m_argumentUnitInfoPtr;
	const imath::IUnitInfo* m_resultUnitInfoPtr;

	iimg::CGeneralBitmap m_lut;

	struct MetricPoint {
		i2d::CVector2d point;
		istd::CIndex2d pixel;
	};

	std::vector<MetricPoint> m_metricPoints;
	imath::TKdTree<size_t, 2> m_metricPointsTree;
};


} // namespace icalib


