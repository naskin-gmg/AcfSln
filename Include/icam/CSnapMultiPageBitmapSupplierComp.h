// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#ifndef icam_CSnapMultiPageBitmapSupplierComp_included
#define icam_CSnapMultiPageBitmapSupplierComp_included


// Qt includes
#include <QtCore/QPair>

// ACF includes
#include <istd/TDelPtr.h>
#include <iimg/CMultiPageBitmapBase.h>
#include <idoc/IMultiPageDocumentProvider.h>

// ACF-Solutions includes
#include <iproc/IProcessor.h>
#include <iinsp/TSupplierCompWrap.h>
#include <imeas/INumericConstraints.h>
#include <imeas/CSimpleNumericValue.h>


namespace icam
{


/**
	Implementation of bitmap supplier based on image acquisition.
*/
class CSnapMultiPageBitmapSupplierComp:
	public iinsp::TSupplierCompWrap<iimg::IMultiBitmapProviderSharedPtr>,
			virtual public iimg::IMultiBitmapProvider,
			virtual public idoc::IMultiPageDocumentProvider
{
public:
	typedef iinsp::TSupplierCompWrap<iimg::IMultiBitmapProviderSharedPtr> BaseClass;

	I_BEGIN_COMPONENT(CSnapMultiPageBitmapSupplierComp);
		I_REGISTER_INTERFACE(iimg::IMultiBitmapProvider);
		I_REGISTER_INTERFACE(idoc::IMultiPageDocumentProvider);
		I_ASSIGN(m_bitmapCompFact, "BitmapFactory", "Use to create bitmap object", true, "BitmapFactory");
		I_ASSIGN(m_bitmapAcquisitionCompPtr, "BitmapAcquisition", "Bitmap acquisition object for image snap", true, "BitmapAcquisition");
	I_END_COMPONENT;

	// reimplemented (iimg::IMultiBitmapProvider)
	virtual int GetBitmapsCount() const;
	virtual const iimg::IBitmap* GetBitmap(int bitmapIndex) const;
	virtual const iprm::IOptionsList* GetBitmapListInfo() const;

	// reimplemented (idoc::IMultiPageDocumentProvider)
	virtual const idoc::IMultiPageDocument* GetDocument() const;

protected:
	// reimplemented (iinsp::TSupplierCompWrap)
	virtual bool InitializeWork();
	virtual iinsp::ISupplier::WorkStatus ProduceObject(ProductType& result) const;

private:
	I_FACT(iimg::IMultiBitmapProvider, m_bitmapCompFact);
	I_REF(iproc::IProcessor, m_bitmapAcquisitionCompPtr);
};


} // namespace icam


#endif // !icam_CSnapMultiPageBitmapSupplierComp_included


