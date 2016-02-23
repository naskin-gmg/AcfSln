#ifndef imeas_CRelativeUnitSelectorComp_included
#define imeas_CRelativeUnitSelectorComp_included


// ACF includes
#include "iser/ISerializable.h"
#include "icomp/CComponentBase.h"

// ACF-Solutions includes
#include "imath/IUnitInfo.h"
#include "iprm/IOptionsList.h"
#include "iprm/ISelectionParam.h"
#include "imath/CDoubleManip.h"


namespace imeas
{


/**
	Unit info selection for relative values.	
*/
class CRelativeUnitSelectorComp:
			public icomp::CComponentBase,
			virtual public imath::IUnitInfo,
			virtual public iprm::ISelectionParam,
			virtual protected iprm::IOptionsList
{
public:
	typedef icomp::CComponentBase BaseClass;

	enum DisplayMode
	{
		/**
			No multiplication factor should be used for displaying value.
		*/
		DM_NONE,

		/**
			Percent should be used for displaying value.
		*/
		DM_PERCENT,

		/**
			Per-mille should be used for displaying value.
		*/
		DM_PERMILLE,

		DM_LAST_MODE = DM_PERMILLE
	};

	I_DECLARE_ENUM(DisplayMode, DM_NONE, DM_PERCENT, DM_PERMILLE);


	I_BEGIN_COMPONENT(CRelativeUnitSelectorComp);
		I_REGISTER_INTERFACE(iprm::ISelectionParam);
		I_REGISTER_INTERFACE(iprm::IOptionsList);
		I_REGISTER_INTERFACE(imath::IUnitInfo);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_REGISTER_INTERFACE(istd::IChangeable);
		I_ASSIGN(m_precisionAttrPtr, "Precision", "Typical values: 0 - integer, 15 - double", true, 15);
	I_END_COMPONENT;

	CRelativeUnitSelectorComp();

	// reimplemented (iprm::ISelectionParam)
	virtual const iprm::IOptionsList* GetSelectionConstraints() const;
	virtual int GetSelectedOptionIndex() const;
	virtual bool SetSelectedOptionIndex(int index);
	virtual iprm::ISelectionParam* GetSubselection(int index) const;

	// reimplemented (iprm::IOptionsList)
	virtual int GetOptionsFlags() const;
	virtual int GetOptionsCount() const;
	virtual QString GetOptionName(int index) const;
	virtual QString GetOptionDescription(int index) const;
	virtual QByteArray GetOptionId(int index) const;
	virtual bool IsOptionEnabled(int index) const;

	// reimplemented (imath::IUnitInfo)
	virtual int GetUnitType() const;
	virtual QString GetUnitName() const;
	virtual double GetDisplayMultiplicationFactor() const;
	virtual istd::CRange GetValueRange() const;
	virtual const imath::IDoubleManip& GetValueManip() const;

	// reimplemented (imath::IDoubleManip)
	virtual int GetPrecision() const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

protected:
	// reimplemented (ibase::CComponentBase)
	virtual void OnComponentCreated();

private:
	I_ATTR(int, m_precisionAttrPtr);

	int m_precision;

	DisplayMode m_displayMode;

	imath::CDoubleManip m_doubleManip;
};


} // namespace imeas


#endif // !imeas_CRelativeUnitSelectorComp_included


