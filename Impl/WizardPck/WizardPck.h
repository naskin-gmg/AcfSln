// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#ifndef WizardPck_included
#define WizardPck_included


#include <icomp/TModelCompWrap.h>


#include <iwiz/CWizardDocumentComp.h>
#include <iwiz/CComposedStateControllerComp.h>
#include <iwiz/CSelectionConsistencyControllerComp.h>


/**
	Wizard controller package.
*/
namespace WizardPck
{


typedef icomp::TModelCompWrap<iwiz::CWizardDocumentComp> WizardDocument;
typedef icomp::TModelCompWrap<iwiz::CComposedStateControllerComp> ComposedStateController;
typedef icomp::TModelCompWrap<iwiz::CSelectionConsistencyControllerComp> SelectionConsistencyController;


} // namespace WizardPck


#endif // !WizardPck_included


