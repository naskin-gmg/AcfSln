#include "imeas/CRelativeUnitSelectorComp.h"


// ACF includes
#include "iser/CPrimitiveTypesSerializer.h"


namespace imeas
{


const istd::IChangeable::ChangeSet s_changeUnitChangeSet(iprm::ISelectionParam::CF_SELECTION_CHANGED, QObject::tr("Change units"));


// public methods

CRelativeUnitSelectorComp::CRelativeUnitSelectorComp()
	:m_displayMode(DM_PERCENT)
{
}


// reimplemented (iprm::ISelectionParam)

const iprm::IOptionsList* CRelativeUnitSelectorComp::GetSelectionConstraints() const
{
	return this;
}


int CRelativeUnitSelectorComp::GetSelectedOptionIndex() const
{
	return m_displayMode;
}


bool CRelativeUnitSelectorComp::SetSelectedOptionIndex(int index)
{
	if (index < 0){
		return false;
	}

	if (index > DM_LAST_MODE){
		return false;
	}

	if (index != m_displayMode)
	{
		istd::CChangeNotifier changeNotfiier(this, &s_changeUnitChangeSet);

		m_displayMode = DisplayMode(index);
	}

	return true;
}


iprm::ISelectionParam* CRelativeUnitSelectorComp::GetSubselection(int /*index*/) const
{
	return NULL;
}


// reimplemented (iprm::IOptionsList)

int CRelativeUnitSelectorComp::GetOptionsFlags() const
{
	return SCF_SUPPORT_UNIQUE_ID;
}


int CRelativeUnitSelectorComp::GetOptionsCount() const
{
	return DM_LAST_MODE + 1;
}


QString CRelativeUnitSelectorComp::GetOptionName(int index) const
{
	Q_ASSERT(index >= 0);
	Q_ASSERT(index < CRelativeUnitSelectorComp::GetOptionsCount());

	switch (index){
		case DM_NONE:
			return QObject::tr("None");
		case DM_PERCENT:
			return "%";
		case DM_PERMILLE:
			return QChar(0x2030);
		default:
			return QString();
	}
}


QString CRelativeUnitSelectorComp::GetOptionDescription(int /*index*/) const
{
	return QString();
}


QByteArray CRelativeUnitSelectorComp::GetOptionId(int index) const
{
	Q_ASSERT(index >= 0);
	Q_ASSERT(index < CRelativeUnitSelectorComp::GetOptionsCount());

	switch (index){
	case DM_NONE:
		return "none";

	case DM_PERCENT:
		return "percent";

	case DM_PERMILLE:
		return "permille";

	default:
		return QByteArray();
	}
}


bool CRelativeUnitSelectorComp::IsOptionEnabled(int /*index*/) const
{
	return true;
}


// reimplemented (imath::IUnitInfo)

int CRelativeUnitSelectorComp::GetUnitType() const
{
	return imath::IUnitInfo::UT_RELATIVE;
}


QString CRelativeUnitSelectorComp::GetUnitName() const
{
	switch (m_displayMode){
	case DM_PERCENT:
		return "%";

	case DM_PERMILLE:
		return QChar(0x2030);

	default:
		return QString();
	}
}


double CRelativeUnitSelectorComp::GetDisplayMultiplicationFactor() const
{
	switch (m_displayMode){
	case DM_PERCENT:
		return 100.0;

	case DM_PERMILLE:
		return 1000.0;

	default:
		return 1.0;
	}
}


istd::CRange CRelativeUnitSelectorComp::GetValueRange() const
{
	return istd::CRange(0, 1.0);
}


const imath::IDoubleManip& CRelativeUnitSelectorComp::GetValueManip() const
{
	return m_doubleManip;
}


// reimplemented (imath::IDoubleManip)

int CRelativeUnitSelectorComp::GetPrecision() const
{
	return m_precision;
}


// reimplemented (iser::ISerializable)

bool CRelativeUnitSelectorComp::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	istd::CChangeNotifier notifier(archive.IsStoring()? NULL: this);
	Q_UNUSED(notifier);

	static iser::CArchiveTag displayModeTag("DisplayMode", "Display mode", iser::CArchiveTag::TT_LEAF);
	retVal = retVal && archive.BeginTag(displayModeTag);
	retVal = I_SERIALIZE_ENUM(DisplayMode, archive, m_displayMode);
	retVal = retVal && archive.EndTag(displayModeTag);

	return retVal;
}


// protected methods

// reimplemented (ibase::CComponentBase)

void CRelativeUnitSelectorComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	m_precision = qMax(0, *m_precisionAttrPtr);
}


} // namespace imeas


