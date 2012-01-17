#ifndef isig_CTriggerParamsComp_included
#define isig_CTriggerParamsComp_included


#include <vector>

#include "icomp/CComponentBase.h"

#include "isig/ITriggerConstraints.h"
#include "isig/ITriggerParams.h"

#include "iprm/ISelectionParam.h"
#include "iprm/ISelectionConstraints.h"


namespace isig
{


/**
	Implementation of standard trigger parameters set.
*/
class CTriggerParamsComp: 
			public icomp::CComponentBase, 
			virtual public isig::ITriggerParams,
			virtual public iprm::ISelectionParam,
			virtual protected iprm::ISelectionConstraints
{
public:
	typedef icomp::CComponentBase BaseClass;

	CTriggerParamsComp();

	I_BEGIN_COMPONENT(CTriggerParamsComp);
		I_REGISTER_INTERFACE(isig::ITriggerParams);
		I_REGISTER_INTERFACE(iprm::ISelectionParam);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_ASSIGN(m_triggerConstraintsCompPtr, "Constraints", "Describing allowed parameter state, typically implemented in camera component", false, "Constraints");
		I_ASSIGN(m_triggerModeAttrPtr, "TriggerMode", "Set trigger mode:\n\t0 - None\n\t1 - Continuous\n\t2 - Rising edge\n\t3 - Falling edge\n\t4 - Positive level\n\t5 - Negative level\n\t6 - Software", true, 0);
	I_END_COMPONENT;

	// reimplemented (isig::ITriggerParams)
	virtual const ITriggerConstraints* GetTriggerConstraints() const;
	virtual int GetTriggerMode() const;
	virtual void SetTriggerMode(int triggerMode);

	// reimplemented (iprm::ISelectionParam)
	virtual const iprm::ISelectionConstraints* GetSelectionConstraints() const;
	virtual int GetSelectedOptionIndex() const;
	virtual bool SetSelectedOptionIndex(int index);
	virtual iprm::ISelectionParam* GetActiveSubselection() const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

protected:
	void InitializeSelectionList();

	// reimplemented (iprm::ISelectionConstraints)
	virtual int GetConstraintsFlags() const;
	virtual int GetOptionsCount() const;
	virtual istd::CString GetOptionName(int index) const;
	virtual istd::CString GetOptionDescription(int index) const;
	virtual std::string GetOptionId(int index) const;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

private:
	int m_triggerMode;

	struct SelectionEntry
	{
		istd::CString name;
		istd::CString description;
		int triggerMode;
	};
	typedef std::vector<SelectionEntry> SelectionList;
	SelectionList m_selectionList;

	I_REF(isig::ITriggerConstraints, m_triggerConstraintsCompPtr);
	I_ATTR(int, m_triggerModeAttrPtr);
};


} // namespace isig


#endif // !isig_CTriggerParamsComp_included


