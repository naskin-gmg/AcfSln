/********************************************************************************
**
**	Copyright (C) 2007-2017 Witold Gantzke & Kirill Lepskiy
**
**	This file is part of the ACF-Solutions Toolkit.
**
**	This file may be used under the terms of the GNU Lesser
**	General Public License version 2.1 as published by the Free Software
**	Foundation and appearing in the file LicenseLGPL.txt included in the
**	packaging of this file.  Please review the following information to
**	ensure the GNU Lesser General Public License version 2.1 requirements
**	will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
**	If you are unsure which license is appropriate for your use, please
**	contact us at info@imagingtools.de.
**
** 	See http://www.ilena.org or write info@imagingtools.de for further
** 	information about the ACF.
**
********************************************************************************/


#ifndef iipr_CMorphologicalProcessorComp_included
#define iipr_CMorphologicalProcessorComp_included


// ACF includes
#include <iprm/COptionsManager.h>
#include <imod/TModelWrap.h>


// ACF-Solutions includes
#include <iipr/CImageRegionProcessorCompBase.h>


namespace iipr
{

class CMorphologicalProcessorAttr: public CImageRegionProcessorCompBase
{
public:
	typedef CImageRegionProcessorCompBase BaseClass;

	I_BEGIN_BASE_COMPONENT(CMorphologicalProcessorAttr);
		I_ASSIGN(m_erosionEnableAttrPtr, "ErosionEnable", "If this attribute is true then erosion filter is available", false, true);
		I_ASSIGN(m_dilatationEnableAttrPtr, "DilatationEnable", "If this attribute is true then dilatation filter is available", false, true);
		I_ASSIGN(m_openingEnableAttrPtr, "OpeningEnable", "If this attribute is true then opening filter is available", false, true);
		I_ASSIGN(m_closingEnableAttrPtr, "ClosingEnable", "If this attribute is true then closing filter is available", false, true);
		I_ASSIGN(m_whiteTopEnableAttrPtr, "WhiteTopEnable", "If this attribute is true then white-top filter is available", false, true);
		I_ASSIGN(m_blackTopEnableAttrPtr, "BlackTopEnable", "If this attribute is true then black-top filter is available", false, true);
		I_ASSIGN(m_morthoGradientEnableAttrPtr, "MorthoGradientEnable", "If this attribute is true then morthological-gradient filter is available", false, true);
		I_ASSIGN(m_rectangleFormEnableAttrPtr, "RectangleFormEnable", "If this attribute is true then rectangle filter form is available", false, true);
		I_ASSIGN(m_circleFormEnableAttrPtr, "CircleFormEnable", "If this attribute is true then circle filter form is available", false, true);
	I_END_COMPONENT;
protected:
	I_ATTR(bool, m_erosionEnableAttrPtr);
	I_ATTR(bool, m_dilatationEnableAttrPtr);
	I_ATTR(bool, m_openingEnableAttrPtr);
	I_ATTR(bool, m_closingEnableAttrPtr);
	I_ATTR(bool, m_whiteTopEnableAttrPtr);
	I_ATTR(bool, m_blackTopEnableAttrPtr);
	I_ATTR(bool, m_morthoGradientEnableAttrPtr);
	I_ATTR(bool, m_rectangleFormEnableAttrPtr);
	I_ATTR(bool, m_circleFormEnableAttrPtr);
};

/**	
	Processor for image cropping.
*/
class CMorphologicalProcessorComp: public CMorphologicalProcessorAttr
{
public:
	typedef CMorphologicalProcessorAttr BaseClass;

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

	QMap<int, ProcessingMode> m_processingModeIndexMap;
	QMap<int, KernelType> m_filterFormIndexMap;
};


} // namespace iipr


#endif // !iipr_CMorphologicalProcessorComp_included

