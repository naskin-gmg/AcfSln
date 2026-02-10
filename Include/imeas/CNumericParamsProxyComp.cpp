// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial﻿
#include <imeas/CNumericParamsProxyComp.h>


namespace imeas
{


// public methods

CNumericParamsProxyComp::CNumericParamsProxyComp()
	:BaseClass2(this)
{
}


// reimplemented (imeas::INumericConstraints)

const iprm::IOptionsList& CNumericParamsProxyComp::GetValueListInfo() const
{
	return *this;
}


const imath::IUnitInfo* CNumericParamsProxyComp::GetNumericValueUnitInfo(int index) const
{
	const imeas::INumericConstraints* constraintsPtr = nullptr;
	if (index >= 0 && index < GetOptionsCount()) {
		constraintsPtr = m_valuesPtr[index]->GetNumericConstraints();
	}

	return constraintsPtr ? constraintsPtr->GetNumericValueUnitInfo(m_valueIndexPtr[index]) : nullptr;
}


// reimplemented (imeas::INumericValue)

bool CNumericParamsProxyComp::IsValueTypeSupported(ValueTypeId /*valueTypeId*/) const
{
	return true;
}


const INumericConstraints* CNumericParamsProxyComp::GetNumericConstraints() const 
{
	return this;
}


imath::CVarVector CNumericParamsProxyComp::GetComponentValue(ValueTypeId /*valueTypeId*/) const 
{
	return {};
}


imath::CVarVector CNumericParamsProxyComp::GetValues() const
{
	int count = m_valuesPtr.GetCount();

	imath::CVarVector result(count);

	for (int i = 0; i < count; i++) {
		imeas::INumericValue* valPtr = m_valuesPtr[i];
		int valIndex = m_valueIndexPtr[i];

		auto values = valPtr->GetValues();
		auto value = valIndex >= 0 ? values[valIndex] : qQNaN();

		result.SetElement(i, value);
	}

	return result;
}


bool CNumericParamsProxyComp::SetValues(const imath::CVarVector& values)
{
	int count = std::min(m_valuesPtr.GetCount(), values.GetElementsCount());

	for (int i = 0; i < count; i++) {
		imeas::INumericValue* valPtr = m_valuesPtr[i];
		int valIndex = m_valueIndexPtr[i];
		if (valIndex < 0 || !valPtr)
			continue;

		auto numValues = valPtr->GetValues();
		numValues[valIndex] = values[i];

		valPtr->SetValues(numValues);
	}

	return true;
}


// reimplemented (iprm::IOptionsList)

int CNumericParamsProxyComp::GetOptionsFlags() const
{
	return 0;
}


int CNumericParamsProxyComp::GetOptionsCount() const
{
	return m_valuesPtr.GetCount();
}


QString CNumericParamsProxyComp::GetOptionName(int index) const
{
	const imeas::INumericConstraints* constraintsPtr = nullptr;
	if (index >= 0 && index < GetOptionsCount()) {
		constraintsPtr = m_valuesPtr[index]->GetNumericConstraints();
	}

	return constraintsPtr ? constraintsPtr->GetValueListInfo().GetOptionName(m_valueIndexPtr[index]) : QString();
}


QString CNumericParamsProxyComp::GetOptionDescription(int index) const
{
	const imeas::INumericConstraints* constraintsPtr = nullptr;
	if (index >= 0 && index < GetOptionsCount()) {
		constraintsPtr = m_valuesPtr[index]->GetNumericConstraints();
	}

	return constraintsPtr ? constraintsPtr->GetValueListInfo().GetOptionDescription(m_valueIndexPtr[index]) : QString();
}


QByteArray CNumericParamsProxyComp::GetOptionId(int index) const
{
	const imeas::INumericConstraints* constraintsPtr = nullptr;
	if (index >= 0 && index < GetOptionsCount()) {
		constraintsPtr = m_valuesPtr[index]->GetNumericConstraints();
	}

	return constraintsPtr ? constraintsPtr->GetValueListInfo().GetOptionId(m_valueIndexPtr[index]) : QByteArray();
}


bool CNumericParamsProxyComp::IsOptionEnabled(int index) const
{
	const imeas::INumericConstraints* constraintsPtr = nullptr;
	if (index >= 0 && index < GetOptionsCount()) {
		constraintsPtr = m_valuesPtr[index]->GetNumericConstraints();
	}

	return constraintsPtr ? constraintsPtr->GetValueListInfo().IsOptionEnabled(m_valueIndexPtr[index]) : true;
}



// reimplemented (iser::ISerializable)

bool CNumericParamsProxyComp::Serialize(iser::IArchive& /*archive*/)
{ 
	return true;	// proxy cannot be serialized
}	


// reimplemented (IChangeable)

bool CNumericParamsProxyComp::CopyFrom(const IChangeable& /*object*/, CompatibilityMode /*mode*/)
{
	return true;
}


// reimplemented (imeas::INumericValue)
//void OnUpdate(const istd::IChangeable::ChangeSet& changeSet) override;

// reimplemented (icomp::CComponentBase)

void CNumericParamsProxyComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	Q_ASSERT(m_valueModelsPtr.GetCount() == m_valueIndexPtr.GetCount());

	for (int i = 0; i < m_valueModelsPtr.GetCount(); i++) {
		Q_ASSERT(m_valueModelsPtr[i] != NULL);

		if (!m_valueModelsPtr[i]->IsAttached(this)) {
			bool isOk = m_valueModelsPtr[i]->AttachObserver(this);
			Q_UNUSED(isOk);
		}
	}
}


void CNumericParamsProxyComp::OnComponentDestroyed()
{
	EnsureModelsDetached();

	BaseClass::OnComponentDestroyed();
}


} // namespace imeas

