#ifndef iblob_CBlobProcessorCompBase_included
#define iblob_CBlobProcessorCompBase_included


// ACF includes
#include <iprm/COptionsManager.h>
#include <iproc/TSyncProcessorCompBase.h>

// ACF-Solutions includes
#include <iipr/IImageToFeatureProcessor.h>
#include <iblob/IBlobFilterParams.h>


namespace iblob
{


class CBlobProcessorCompBase: public iproc::TSyncProcessorCompBase<iipr::IImageToFeatureProcessor>
{
public:
	typedef iproc::TSyncProcessorCompBase<iipr::IImageToFeatureProcessor> BaseClass;

	I_BEGIN_BASE_COMPONENT(CBlobProcessorCompBase);
		I_REGISTER_SUBELEMENT(FilterFeatureList);
		I_REGISTER_SUBELEMENT_INTERFACE(FilterFeatureList, iprm::IOptionsList, ExtractFilterFeatureList);
		I_ASSIGN(m_filterParamsIdAttrPtr, "FilterParamsId", "ID of blob filter parameters in the parameter set (iblob::IBlobFilterParams)", false, "FilterParams");
		I_ASSIGN(m_defaultFilterParamsCompPtr, "DefaultFilterParams", "Default blob filter parameters", false, "DefaultFilterParams");
	I_END_COMPONENT;

	// reimplemented (iipr::IImageToFeatureProcessor)
	virtual int DoExtractFeatures(
				const iprm::IParamsSet* paramsPtr,
				const iimg::IBitmap& image,
				iipr::IFeaturesConsumer& results,
				ibase::IProgressManager* progressManagerPtr = NULL);

	// reimplemented (iproc::IProcessor)
	virtual int DoProcessing(
				const iprm::IParamsSet* paramsPtr,
				const istd::IPolymorphic* inputPtr,
				istd::IChangeable* outputPtr,
				ibase::IProgressManager* progressManagerPtr = NULL);

protected:
	static bool IsBlobAcceptedByFilter(const iblob::IBlobFilterParams& filterParams, double area, double perimeter, double circularity);

	// abstract methods
	virtual bool CalculateBlobs(
				const iprm::IParamsSet* paramsPtr,
				const iblob::IBlobFilterParams* filterParamsPtr,
				const iimg::IBitmap& image,
				iipr::IFeaturesConsumer& result) = 0;

private:
	class FilterFeatureList: public iprm::COptionsManager
	{
	public:
		FilterFeatureList();
	};

	static bool IsValueAcceptedByFilter(const iblob::IBlobFilterParams::Filter& filter, double value);

	// static template methods for sub-element access
	template <class InterfaceType>
	static InterfaceType* ExtractFilterFeatureList(CBlobProcessorCompBase& component)
	{
		static FilterFeatureList s_filterFeatureList;
		return &component.m_filterFeatureList;
	}

	I_ATTR(QByteArray, m_filterParamsIdAttrPtr);
	I_REF(iblob::IBlobFilterParams, m_defaultFilterParamsCompPtr);

	FilterFeatureList m_filterFeatureList;
};


} // namespace iblob


#endif // !iblob_CBlobProcessorCompBase_included


