#ifndef iipr_CSearchBasedFeaturesSupplierComp_included
#define iipr_CSearchBasedFeaturesSupplierComp_included


// ACF includes
#include <i2d/IMultiCalibrationProvider.h>
#include <i2d/CVector2d.h>
#include <iprm/IParamsSet.h>
#include <istd/IInformationProvider.h>

// ACF-Solutions includes
#include <iproc/IProcessor.h>
#include <iinsp/ISupplier.h>
#include <iinsp/TSupplierCompWrap.h>
#include <iimg/IBitmapProvider.h>
#include <icalib/CAffineCalibration2d.h>
#include <iipr/IFeaturesProvider.h>
#include <iipr/CFeaturesContainer.h>


namespace iipr
{


class CSearchBasedFeaturesSupplierComp:
			public iinsp::TSupplierCompWrap<CFeaturesContainer>,
			virtual public IFeaturesProvider,
			virtual public i2d::IMultiCalibrationProvider,
			virtual public istd::IInformationProvider
{
public:
	typedef iinsp::TSupplierCompWrap<CFeaturesContainer> BaseClass;

	I_BEGIN_COMPONENT(CSearchBasedFeaturesSupplierComp);
		I_REGISTER_INTERFACE(iipr::IFeaturesProvider);
		I_REGISTER_INTERFACE(i2d::IMultiCalibrationProvider);
		I_REGISTER_INTERFACE(istd::IInformationProvider);
		I_ASSIGN(m_bitmapProviderCompPtr, "BitmapProvider", "Provide image to analyse", true, "BitmapProvider");
		I_ASSIGN_TO(m_bitmapSupplierCompPtr, m_bitmapProviderCompPtr, false);
		I_ASSIGN_TO(m_bitmapProviderModelCompPtr, m_bitmapProviderCompPtr, false);
		I_ASSIGN(m_searchProcessorCompPtr, "Processor", "Calculate model positions in the image", true, "Processor");
		I_ASSIGN(m_slaveInformationProviderCompPtr, "SlaveInformationProvider", "Used to provide the status if set", false, "SlaveInformationProvider");
		I_ASSIGN(m_searchParamsManagerParamIdAttrPtr, "SearchParamsManagerParameterId", "ID of the parameter manager used for multi search", false, "SearchParamsManagerParamId");
		I_ASSIGN(m_searchParamsIdAttrPtr, "SearchParamsId", "ID of standard search parameters", true, "SearchParamsId");
	I_END_COMPONENT;

	CSearchBasedFeaturesSupplierComp();

	// reimplemented (i2d::IMultiCalibrationProvider)
	virtual const iprm::IOptionsList* GetCalibrationSelectionContraints() const;
	virtual int GetCalibrationsCount() const;
	virtual const i2d::ICalibration2d* GetCalibration(int calibrationIndex) const;

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
	virtual int ProduceObject(CFeaturesContainer& result) const;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

private:

	class CalibrationList : virtual public iprm::IOptionsList
	{
	public:
		CalibrationList();

		void SetParentPtr(CSearchBasedFeaturesSupplierComp* parentPtr);

		// reimplemented (iprm::IOptionsList)
		virtual int GetOptionsFlags() const;
		virtual int GetOptionsCount() const;
		virtual QString GetOptionName(int index) const;
		virtual QString GetOptionDescription(int index) const;
		virtual QByteArray GetOptionId(int index) const;
		virtual bool IsOptionEnabled(int index) const;

	private:
		CSearchBasedFeaturesSupplierComp* m_parentPtr;
	};

private:
	I_REF(iimg::IBitmapProvider, m_bitmapProviderCompPtr);
	I_REF(iinsp::ISupplier, m_bitmapSupplierCompPtr);
	I_REF(imod::IModel, m_bitmapProviderModelCompPtr);
	I_REF(iproc::IProcessor, m_searchProcessorCompPtr);
	I_REF(istd::IInformationProvider, m_slaveInformationProviderCompPtr);
	I_ATTR(QByteArray, m_searchParamsManagerParamIdAttrPtr);
	I_ATTR(QByteArray, m_searchParamsIdAttrPtr);
	
	struct CalibrationInfo
	{
		icalib::CAffineCalibration2d calibration;
		QByteArray calibrationId;
		QString calibrationName;
	};

	typedef QVector<CalibrationInfo> Calibrations;
	
	mutable Calibrations m_calibrations;

	imod::TModelWrap<CalibrationList> m_calibrationList;

	mutable istd::IInformationProvider::InformationCategory m_defaultInformationCategory;
};


} // namespace iipr


#endif // !iipr_CSearchBasedFeaturesSupplierComp_included


