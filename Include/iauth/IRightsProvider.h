#ifndef iauth_IRightsProvider_included
#define iauth_IRightsProvider_included


// Qt includes
#include <QtCore/QByteArray>

// ACF includes
#include "istd/IChangeable.h"


namespace iauth
{


class IRightsProvider: virtual public istd::IChangeable
{
public:
	/**
		Checks if user has rights for specific operation.
		\param	operationId	id of operation.
		\param	beQuiet if it is true, user can not be asked for password during this check.
	 */
	virtual bool HasRight(
				const QByteArray& operationId,
				bool beQuiet = false) const = 0;
};


} // namespace iauth


#endif // !iauth_IRightsProvider_included


