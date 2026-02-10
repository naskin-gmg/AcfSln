// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#ifndef iinsp_TSupplierCompWrap_included
#define iinsp_TSupplierCompWrap_included


// ACF-Solutions includes
#include <iinsp/CSupplierCompBase.h>


namespace iinsp
{


/**
	Standard implementation of interface iinsp::ISupplier with preparation for component implementation.
	During component initialization you should call \c RegisterSupplierInput for all suppliers used by this component as an input.
*/
template <class Product>
class TSupplierCompWrap: public CSupplierCompBase
{
public:
	typedef CSupplierCompBase BaseClass;
	typedef Product ProductType;

	I_BEGIN_BASE_COMPONENT(TSupplierCompWrap);
	I_END_COMPONENT;

	// reimplemented (iinsp::CSupplierCompBase)
	virtual iinsp::ISupplier::WorkStatus ProcessWorkOutput() override;

	// reimplemented (iinsp::ISupplier)
	virtual void ClearWorkResults();

protected:
	/**
		Get current work product, if work was done correctly.
	*/
	const Product* GetWorkProduct() const;

	// abstract methods
	/**
		Produce single object.
		\return	work status. \sa iinsp::WorkStatus
	*/
	virtual iinsp::ISupplier::WorkStatus ProduceObject(Product& result) const = 0;

protected:
	istd::TDelPtr<Product> m_productPtr;
};


// public methods

// reimplemented (iinsp::CSupplierCompBase)
template <class Product>
iinsp::ISupplier::WorkStatus TSupplierCompWrap<Product>::ProcessWorkOutput()
{
	if (!m_productPtr.IsValid()){
		m_productPtr.SetPtr(new Product());
	}

	return ProduceObject(*m_productPtr);
}


// reimplemented (iinsp::ISupplier)

template <class Product>
void TSupplierCompWrap<Product>::ClearWorkResults()
{
	if (BaseClass::GetWorkStatus() == WS_LOCKED){
		return;
	}

	ChangeSet changes(iinsp::ISupplier::CF_SUPPLIER_RESULTS);
	istd::CChangeNotifier changeNotifier(this, &changes);

	m_productPtr.Reset();

	BaseClass::ClearWorkResults();
}


// protected methods

template <class Product>
const Product* TSupplierCompWrap<Product>::GetWorkProduct() const
{
	const_cast< TSupplierCompWrap<Product>* >(this)->EnsureWorkFinished();

	if (BaseClass::GetWorkStatus() <= WS_OK){
		return m_productPtr.GetPtr();
	}
	else{
		return NULL;
	}
}


} // namespace iinsp


#endif // !iinsp_TSupplierCompWrap_included


