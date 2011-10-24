#ifndef iipr_CCircleFindProcessorComp_included
#define iipr_CCircleFindProcessorComp_included


// ACF includes
#include "i2d/CCircle.h"
#include "i2d/CAnnulus.h"
#include "icomp/CComponentBase.h"
#include "iproc/TSyncProcessorWrap.h"

#include "iipr/IImageToFeatureProcessor.h"
#include "iipr/IFeaturesProvider.h"
#include "iipr/IFeatureToImageMapper.h"
#include "iipr/ICircleFinderParams.h"
#include "iipr/TFeatureWrap.h"


namespace iipr
{


/**
	Processor finding circle on the image.
	It takes area of interests (AOI) from parameter set. As AOI can be used annulus, segment of annulus, rectangle and set of segments of annulus.
*/
class CCircleFindProcessorComp:
			public icomp::CComponentBase,
			public iproc::TSyncProcessorWrap<IImageToFeatureProcessor>
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CCircleFindProcessorComp);
		I_REGISTER_INTERFACE(IImageToFeatureProcessor);
		I_REGISTER_INTERFACE(iproc::IProcessor);
		I_ASSIGN(m_slaveProcessorCompPtr, "SlaveEdgeProcessor", "Slave edge finder processor converting image to list of found edges", true, "SlaveEdgeProcessor");
		I_ASSIGN(m_featuresMapperCompPtr, "FeaturesMapper", "Calculate position from caliper extracted features", true, "FeaturesMapper");
		I_ASSIGN(m_aoiParamsIdAttrPtr, "AoiParamsId", "ID of area of interest in parameter set", true, "AoiParams");
		I_ASSIGN(m_circleFinderParamsIdAttrPtr, "CircleFinderParamsId", "ID cirlcie finder parameters in parameter set", true, "CircleFinderParams");
		I_ASSIGN(m_slaveLineIdAttrPtr, "SlaveLineId", "ID of line parameter added by this processor to parameter set for slave edge processor", true, "LineParam");
		I_ASSIGN(m_searchForAnnulusAttrPtr, "SearchForAnnulus", "If it is true, annulus will be searched", true, false);
	I_END_COMPONENT;

	// reimplemented (iipr::IImageToFeatureProcessor)
	virtual int DoExtractFeatures(
				const iprm::IParamsSet* paramsPtr,
				const iimg::IBitmap& image,
				IFeaturesConsumer& results);

	// reimplemented (iproc::IProcessor)
	virtual int DoProcessing(
				const iprm::IParamsSet* paramsPtr,
				const istd::IPolymorphic* inputPtr,
				istd::IChangeable* outputPtr,
				iproc::IProgressManager* progressManagerPtr = NULL);

protected:
	typedef TFeatureWrap<i2d::CCircle> CircleFeature;
	typedef TFeatureWrap<i2d::CAnnulus> AnnulusFeature;

	struct Point
	{
		double weight;
		i2d::CVector2d position;
	};

	typedef std::vector<Point> Points;

	struct Ray
	{
		Ray(){usedIndex = -1;};
		Points points;
		int usedIndex;
	};

	typedef std::vector<Ray> Rays;

	bool AddAoiToRays(
				const istd::IChangeable& aoiObject,
				const iprm::IParamsSet& params,
				const iimg::IBitmap& image,
				const iipr::ICircleFinderParams& circleFinderParams,
				Rays& inRays,
				Rays& outRays,
				i2d::CLine2d& projectionLine,
				i2d::CVector2d& center);
	bool CalculateCircle(const i2d::CVector2d& center, bool removeOutliers, double minOutliersDistance, Rays& rays, CircleFeature& result);
	bool CalculateAnnulus(const i2d::CVector2d& center, Rays& inRays, Rays& outRays, AnnulusFeature& result);

	void AddProjectionResultsToRays(
				const iprm::IParamsSet& params,
				const IFeaturesProvider& container,
				Rays& inRays,
				Rays& outRays);

private:
	I_REF(iproc::IProcessor, m_slaveProcessorCompPtr);
	I_REF(iipr::IFeatureToImageMapper, m_featuresMapperCompPtr);
	I_ATTR(istd::CString, m_aoiParamsIdAttrPtr);
	I_ATTR(istd::CString, m_slaveLineIdAttrPtr);
	I_ATTR(istd::CString, m_circleFinderParamsIdAttrPtr);
	I_ATTR(bool, m_searchForAnnulusAttrPtr);
};


} // namespace iipr


#endif // !iipr_CCircleFindProcessorComp_included


