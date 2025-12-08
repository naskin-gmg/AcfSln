#pragma once


// ACF includes
#include <i2d/CPolygon.h>

// ACF-Solutions includes
#include <iipr/CObjectFeature.h>


namespace iblob
{


/**
	Implementation of the blob-based feature.
*/
class CBlobFeature: public iipr::CObjectFeature
{
public:
	typedef iipr::CObjectFeature BaseClass;

	CBlobFeature();

	CBlobFeature(
				double area,
				double perimeter,
				const i2d::CVector2d& position,
				double angle = 0.0,
				const i2d::CVector2d& scale = i2d::CVector2d(1.0, 1.0),
				const i2d::CPolygon& contour = i2d::CPolygon());

	double GetCircularity() const;
	double GetCompactness() const;
	double GetPerimeter() const;
	double GetArea() const;
	const i2d::CPolygon& GetContour() const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive) override;

	// reimplemented (istd::IChangeable)
	virtual bool CopyFrom(const IChangeable& object, CompatibilityMode mode = CM_WITHOUT_REFS) override;
	virtual istd::IChangeableUniquePtr CloneMe(CompatibilityMode mode = CM_WITHOUT_REFS) const override;

	// reimplemented (i2d::IObject2d)
	virtual void SetCalibration(const i2d::ICalibration2d* calibrationPtr, bool releaseFlag = false) override;
	virtual i2d::CRectangle GetBoundingBox() const override;
	virtual bool Transform(
		const i2d::ITransformation2d& transformation,
		i2d::ITransformation2d::ExactnessMode mode = i2d::ITransformation2d::EM_NONE,
		double* errorFactorPtr = NULL) override;
	virtual bool InvTransform(
		const i2d::ITransformation2d& transformation,
		i2d::ITransformation2d::ExactnessMode mode = i2d::ITransformation2d::EM_NONE,
		double* errorFactorPtr = NULL) override;

private:
	double m_area;
	double m_perimeter;
	i2d::CPolygon m_contour;
};


} // namespace iblob


