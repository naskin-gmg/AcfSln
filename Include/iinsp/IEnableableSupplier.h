// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#ifndef iinsp_IEnableableSupplier_included
#define iinsp_IEnableableSupplier_included


// ACF includes
#include <iinsp/ISupplier.h>


namespace iinsp
{


/**
	Extension of ISupplier interface with possiblity to activate/deactivate supplier's producing logic.
*/
class IEnableableSupplier: virtual public ISupplier
{
public:
	/**
		Data model change notification flags.
	*/
	enum ChangeFlags
	{
		/**
			Supplier's activation state was changed.
		*/
		CF_SUPPLIER_ACTIVATION = 0x6326416
	};

	/**
		Set supplier's activation state.
		If the state of supplier cannot be changed, the method will failed.
		\sa IsStateFixed
	*/
	virtual bool SetSupplierEnabled(bool isEnabled = true) = 0;

	/**
		Get activation state of the supplier.
	*/
	virtual bool IsSupplierEnabled() const = 0;

	/**
		Get \c true if the state of the supplier is fixed and cannot be changed.
	*/
	virtual bool IsStateFixed() const = 0;
};


} // namespace iinsp


#endif // !iinsp_IEnableableSupplier_included


