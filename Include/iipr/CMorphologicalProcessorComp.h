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
		PM_LAST = PM_CLOSING
	};
	
	I_BEGIN_COMPONENT(CMorphologicalProcessorComp);		
		I_REGISTER_SUBELEMENT(ProcessingModes);
		I_REGISTER_SUBELEMENT_INTERFACE(ProcessingModes, istd::IChangeable, GetProcessingModes);
		I_REGISTER_SUBELEMENT_INTERFACE(ProcessingModes, iprm::IOptionsList, GetProcessingModes);
		I_ASSIGN(m_filterSizeParamsIdAttrPtr, "FilterSizeParamsId", "ID of the filter dimension parameter set", true, "FilterSizeParamsId");
		I_ASSIGN(m_defaultProcessingModeAttrPtr, "ProcessingMode", "Filter processing mode\n0 - Erosion\n1 - Dilatation\n2 - Opening\n3 - Closing", true, 0);
		I_ASSIGN(m_processingModeIdAttrPtr, "ProcessingModeId", "Processing mode parameter Id in ParamsSet", true, "ProcessingMode");
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

	int GetProcessingMode(const iprm::IParamsSet* paramsPtr) const;


	template <class InterfaceType>
	static InterfaceType* GetProcessingModes(CMorphologicalProcessorComp& component)
	{
		return &component.m_processingModes;
	}

private:

	imod::TModelWrap<iprm::COptionsManager> m_processingModes;

	I_ATTR(QByteArray, m_filterSizeParamsIdAttrPtr);
	I_ATTR(int, m_defaultProcessingModeAttrPtr);
	I_ATTR(QByteArray, m_processingModeIdAttrPtr);
};


} // namespace iipr


#endif // !iipr_CMorphologicalProcessorComp_included

