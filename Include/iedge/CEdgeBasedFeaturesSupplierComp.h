#ifndef iedge_CEdgeBasedFeaturesSupplierComp_included
#define iedge_CEdgeBasedFeaturesSupplierComp_included


// Qt includes
#include <QtCore/QPair>

// ACF includes
#include <i2d/IMultiCalibrationProvider.h>
#include <i2d/ICalibrationProvider.h>
#include <i2d/CVector2d.h>
#include <icalib/CAffineCalibration2d.h>
#include <iprm/IParamsSet.h>
#include <istd/IInformationProvider.h>

// ACF-Solutions includes
#include <iproc/IProcessor.h>
#include <iinsp/ISupplier.h>
#include <iinsp/TSupplierCompWrap.h>
#include <iipr/CFeaturesContainer.h>
#include <iedge/IEdgeLinesProvider.h>


namespace iedge
{


class CEdgeBasedFeaturesSupplierComp:
	public iinsp::TSupplierCompWrap< QPair< iipr::CFeaturesContainer, std::vector<icalib::CAffineCalibration2d> > >,
			virtual public iipr::IFeaturesProvider,
			virtual public i2d::IMultiCalibrationProvider,
			virtual public i2d::ICalibrationProvider,
			virtual public istd::IInformationProvider
{
public:
	typedef iinsp::TSupplierCompWrap< QPair< iipr::CFeaturesContainer, std::vector<icalib::CAffineCalibration2d> > > BaseClass;

	I_BEGIN_COMPONENT(CEdgeBasedFeaturesSupplierComp);
		I_REGISTER_INTERFACE(iipr::IFeaturesProvider);
		I_REGISTER_INTERFACE(i2d::IMultiCalibrationProvider);
		I_REGISTER_INTERFACE(i2d::ICalibrationProvider);
		I_REGISTER_INTERFACE(istd::IInformationProvider);
		I_ASSIGN(m_calibrationProviderCompPtr, "CalibrationProvider", "Provide input calibration", true, "EdgeLinesProvider");
		I_ASSIGN_TO(m_calibrationSupplierCompPtr, m_calibrationProviderCompPtr, false);
		I_ASSIGN_TO(m_calibrationProviderModelCompPtr, m_calibrationProviderCompPtr, false);
		I_ASSIGN(m_edgeLinesProviderCompPtr, "EdgeLinesProvider", "Provide list of edge list to processing", true, "EdgeLinesProvider");
		I_ASSIGN_TO(m_edgeLinesSupplierCompPtr, m_edgeLinesProviderCompPtr, false);
		I_ASSIGN_TO(m_edgeLinesProviderModelCompPtr, m_edgeLinesProviderCompPtr, false);
		I_ASSIGN(m_searchProcessorCompPtr, "Processor", "Calculate model positions in the image", true, "Processor");
		I_ASSIGN(m_slaveInformationProviderCompPtr, "SlaveInformationProvider", "Used to provide the status if set", false, "SlaveInformationProvider");
		I_ASSIGN(m_searchParamsManagerParamIdAttrPtr, "SearchParamsManagerParameterId", "ID of the parameter manager used for multi search", false, "SearchParamsManagerParamId");
		I_ASSIGN(m_searchParamsIdAttrPtr, "SearchParamsId", "ID of standard search parameters", true, "SearchParamsId");
	I_END_COMPONENT;

	CEdgeBasedFeaturesSupplierComp();

	// reimplemented (i2d::IMultiCalibrationProvider)
	virtual const iprm::IOptionsList* GetCalibrationSelectionContraints() const;
	virtual int GetCalibrationsCount() const;
	virtual const i2d::ICalibration2d* GetCalibration(int calibrationIndex) const;

	// reimplemented (i2d::ICalibrationProvider)
	virtual const i2d::ICalibration2d* GetCalibration() const;

	// reimplemented (iipr::IFeaturesProvider)
	virtual int GetFeaturesCount() const;
	virtual const imeas::INumericValue& GetFeature(int index) const;

	// reimplemented (istd::IInformationProvider)
	virtual QDateTime GetInformationTimeStamp() const;
	virtual InformationCategory GetInformationCategory() const;
	virtual int GetInformationId() const;
	virtual QString GetInformationDescription() const;
	virtual QString GetInformationSource() const;
	virtual int GetInformationFlags() const;

protected:
	// reimplemented (iinsp::TSupplierCompWrap)
	virtual bool InitializeWork();
	virtual int ProduceObject(ProductType& result) const;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

private:
	I_REF(i2d::ICalibrationProvider, m_calibrationProviderCompPtr);
	I_REF(iinsp::ISupplier, m_calibrationSupplierCompPtr);
	I_REF(imod::IModel, m_calibrationProviderModelCompPtr);
	I_REF(IEdgeLinesProvider, m_edgeLinesProviderCompPtr);
	I_REF(iinsp::ISupplier, m_edgeLinesSupplierCompPtr);
	I_REF(imod::IModel, m_edgeLinesProviderModelCompPtr);
	I_REF(iproc::IProcessor, m_searchProcessorCompPtr);
	I_REF(istd::IInformationProvider, m_slaveInformationProviderCompPtr);
	I_ATTR(QByteArray, m_searchParamsManagerParamIdAttrPtr);
	I_ATTR(QByteArray, m_searchParamsIdAttrPtr);

	mutable istd::IInformationProvider::InformationCategory m_defaultInformationCategory;
};


} // namespace iedge


#endif // !iedge_CEdgeBasedFeaturesSupplierComp_included


