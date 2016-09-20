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

	// reimplemented (iinsp::ISupplier)
	virtual void EnsureWorkFinished();
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
	virtual int ProduceObject(Product& result) const = 0;

protected:
	istd::TDelPtr<Product> m_productPtr;
};


// public methods

// reimplemented (iinsp::ISupplier)

template <class Product>
void TSupplierCompWrap<Product>::EnsureWorkFinished()
{
	if (m_workStatus == WS_INIT){
		m_workStatus = WS_LOCKED;

		if (!m_productPtr.IsValid()){
			m_productPtr.SetPtr(new Product());
		}

		m_workStatus = ProduceObject(*m_productPtr);
		Q_ASSERT(m_workStatus >= WS_OK);	// No initial states are possible

		m_productChangeNotifierPtr.Reset();
	}
}


template <class Product>
void TSupplierCompWrap<Product>::ClearWorkResults()
{
	if (m_workStatus == WS_LOCKED){
		return;
	}

	m_productPtr.Reset();

	BaseClass::ClearWorkResults();
}


// protected methods

template <class Product>
const Product* TSupplierCompWrap<Product>::GetWorkProduct() const
{
	const_cast< TSupplierCompWrap<Product>* >(this)->EnsureWorkFinished();

	if (m_workStatus <= WS_OK){
		return m_productPtr.GetPtr();
	}
	else{
		return NULL;
	}
}


} // namespace iinsp


#endif // !iinsp_TSupplierCompWrap_included


