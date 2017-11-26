#ifndef iipr_CMorphologicalProcessorComp_included
#define iipr_CMorphologicalProcessorComp_included


// ACF includes
#include <iprm/COptionsManager.h>
#include <imod/TModelWrap.h>


// ACF-Solutions includes
#include <iipr/CImageRegionProcessorCompBase.h>


namespace iipr
{


/**	
	Processor for image cropping.
*/
class CMorphologicalProcessorComp: public CImageRegionProcessorCompBase
{
public:
	typedef CImageRegionProcessorCompBase BaseClass;

	enum ProcessingMode
	{
		PM_FIRST,
		PM_EROSION = PM_FIRST,
		PM_DILATATION,
		PM_OPENING,
		PM_CLOSING,
		PM_WHITE_TOP_HAT,
		PM_BLACK_TOP_HAT,
		PM_MORPHO_GRADIENT,
		PM_LAST = PM_MORPHO_GRADIENT
	};
	
	enum KernelType
	{
		KT_FIRST,
		KT_RECT = KT_FIRST,
		KT_CIRCLE,
		KT_LAST = KT_CIRCLE
	};

	I_BEGIN_COMPONENT(CMorphologicalProcessorComp);
		I_REGISTER_SUBELEMENT(ProcessingModes);
		I_REGISTER_SUBELEMENT_INTERFACE(ProcessingModes, istd::IChangeable, ExtractProcessingModes);
		I_REGISTER_SUBELEMENT_INTERFACE(ProcessingModes, iprm::IOptionsList, ExtractProcessingModes);
		I_REGISTER_SUBELEMENT(FilterForms);
		I_REGISTER_SUBELEMENT_INTERFACE(FilterForms, istd::IChangeable, ExtractFilterForms);
		I_REGISTER_SUBELEMENT_INTERFACE(FilterForms, iprm::IOptionsList, ExtractFilterForms);
		I_ASSIGN(m_filterSizeParamsIdAttrPtr, "FilterSizeParamsId", "ID of the filter dimension parameter in the processing parameter set", true, "FilterSizeParamsId");
		I_ASSIGN(m_defaultProcessingModeAttrPtr, "ProcessingMode", "Filter processing mode\n0 - Erosion\n1 - Dilatation\n2 - Opening\n3 - Closing\n4 - White Top Hat\n5 - Black Top Hat\n6 - Morphological Gradient", true, PM_FIRST);
		I_ASSIGN(m_processingModeIdAttrPtr, "ProcessingModeId", "ID of the the procesing mode parameter in the processing parameter set", false, "ProcessingMode");
		I_ASSIGN(m_defaultFilterFormTypeAttrPtr, "FilterFormType", "Type of filter form:\n0 - Rectangular([n, m])\n1 - Circular([n, n])", true, 0);
		I_ASSIGN(m_filterFormTypeIdAttrPtr, "FilterFormTypeParamId", "ID of the filter form type parameter in the processing parameter set", false, "FilterFormType");
	I_END_COMPONENT;

protected:
	// reimplemented (CImageRegionProcessorCompBase)
	virtual bool ProcessImageRegion(
				const iimg::IBitmap& inputBitmap,
				const iprm::IParamsSet* paramsPtr,
				const i2d::IObject2d* aoiPtr,
				istd::IChangeable* outputPtr) const;

	// reimplemented (icomp::CComponentBase)
	void OnComponentCreated();

private:
	ProcessingMode GetProcessingMode(const iprm::IParamsSet* paramsPtr) const;
	KernelType GetKernelType(const iprm::IParamsSet* paramsPtr) const;

	template <class InterfaceType>
	static InterfaceType* ExtractProcessingModes(CMorphologicalProcessorComp& component)
	{
		return &component.m_processingModes;
	}

	template <class InterfaceType>
	static InterfaceType* ExtractFilterForms(CMorphologicalProcessorComp& component)
	{
		return &component.m_filterForms;
	}

private:
	I_ATTR(QByteArray, m_filterSizeParamsIdAttrPtr);
	I_ATTR(int, m_defaultProcessingModeAttrPtr);
	I_ATTR(QByteArray, m_processingModeIdAttrPtr);

	I_ATTR(int, m_defaultFilterFormTypeAttrPtr);
	I_ATTR(QByteArray, m_filterFormTypeIdAttrPtr);

	imod::TModelWrap<iprm::COptionsManager> m_processingModes;
	imod::TModelWrap<iprm::COptionsManager> m_filterForms;
};


} // namespace iipr


#endif // !iipr_CMorphologicalProcessorComp_included

