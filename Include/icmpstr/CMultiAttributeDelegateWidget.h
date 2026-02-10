// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#ifndef icmpstr_CMultiAttributeDelegateWidget_included
#define icmpstr_CMultiAttributeDelegateWidget_included


// Qt includes
#include <QtGui/QIcon>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QWidget>
#include <QtWidgets/QItemDelegate>
#else
#include <QtGui/QWidget>
#include <QtGui/QItemDelegate>
#endif

// ACF-Solutions includes
#include <icmpstr/CElementSelectionInfoManagerBase.h>


namespace icmpstr
{


class CMultiAttributeDelegateWidget: public QWidget
{
	Q_OBJECT
public:
	typedef QWidget BaseClass;

public:
	CMultiAttributeDelegateWidget(
				QItemDelegate& itemDelegate,
				const CElementSelectionInfoManagerBase& elementSelectionInfoManager,
				QWidget* parentWidget,
				const QByteArray& attributeId,
				int attributeFlags);

	QString GetText() const;
	void SetText(const QString& text);

protected Q_SLOTS:
	void OnShowDialog();

private:
	QItemDelegate& m_itemDelegate;
	QLineEdit* m_textEditor;
	QByteArray m_attributeId;
	int m_attributeFlags;

	const CElementSelectionInfoManagerBase& m_elementSelectionInfoManager;
};


} // namespace icmpstr


#endif // !icmpstr_CMultiAttributeDelegateWidget_included

