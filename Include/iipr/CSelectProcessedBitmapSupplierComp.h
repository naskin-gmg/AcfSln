#pragma once

#include <iipr/CProcessedBitmapSupplierComp.h>


// Acf includes
#include <iprm/COptionsManager.h>


namespace iipr
{


/**
	Image supplier providing processed image from some other input image supplier.
	This implementation delegates the provided input calibration and has the role of the calibration provider.
	If the connected processor creates a new calibration, then she should be pushed into the 'OutputBitmapCalibration'. In this case it will be the provided calibration
*/
class CSelectProcessedBitmapSupplierComp: virtual public iipr::CProcessedBitmapSupplierComp

{
public:
	typedef iipr::CProcessedBitmapSupplierComp BaseClass;

	I_BEGIN_COMPONENT(CSelectProcessedBitmapSupplierComp);
		I_REGISTER_SUBELEMENT(ProcessorNames);
		I_REGISTER_SUBELEMENT_INTERFACE(ProcessorNames, istd::IChangeable, ExtractProcessorNames);
		I_REGISTER_SUBELEMENT_INTERFACE(ProcessorNames, iprm::IOptionsList, ExtractProcessorNames);

		I_ASSIGN_MULTI_0(m_processorNamesAttrPtr, "BitmapProcessorNames", "Names of processing options", true);
		I_ASSIGN_MULTI_0(m_processorIdsAttrPtr, "BitmapProcessorIds", "Ids of processing options", true);
		I_ASSIGN_MULTI_0(m_imageProcessorCompPtr, "BitmapProcessor", "Bitmap conversion processor (takes bitmap as input and output)", true);
		I_ASSIGN(m_algorithmManagerIdAttrPtr, "AlgorithmSelector", "Id of the algorithm selector manager", true, "AlgorithmSelector");
		I_ASSIGN(m_outputBitmapCalibrationCompPtr, "OutputBitmapCalibration", "Resulting calibration of the processor output", false, "OutputBitmapCalibration");
	I_END_COMPONENT;

	// reimplemented (iinsp::TSupplierCompWrap)
	virtual iinsp::ISupplier::WorkStatus ProduceObject(ProductType& result) const override;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated() override;
	virtual void OnComponentDestroyed() override;

protected:

	const int GetProcessorIdx(const QByteArray paramId) const;
	const iprm::IParamsSet* GetParametersSelected() const;
	// reimplemented iipr::CProcessedBitmapSupplierBase
	virtual iproc::IProcessor* GetImageProcessor() const override;

private:
	
	template <class InterfaceType>
	static InterfaceType* ExtractProcessorNames(CSelectProcessedBitmapSupplierComp& component)
	{
		return &component.m_processorNames;
	}

	I_REF(i2d::ICalibration2d, m_outputBitmapCalibrationCompPtr);
	I_MULTIREF(iproc::IProcessor, m_imageProcessorCompPtr);
	I_MULTIATTR(QByteArray, m_processorNamesAttrPtr);
	I_MULTIATTR(QByteArray, m_processorIdsAttrPtr);
	I_ATTR(QByteArray, m_algorithmManagerIdAttrPtr);

	imod::TModelWrap<iprm::COptionsManager> m_processorNames;
};



} // namespace iipr


