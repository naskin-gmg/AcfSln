// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QVector>

// ACF includes
#include <istd/IPolymorphic.h>


namespace icomm
{


/**
	Base implementation of message reader using message unalignmented data packages.
*/
template <class Header>
class TPopCumulatorBase: virtual public istd::IPolymorphic
{
public:
	TPopCumulatorBase();

	virtual void ResetBuffer();
	/**
		Try to read buffer.
	*/
	virtual bool ForceRead();

protected:
	// abstract methods
	/**
		Read data to buffer.
		It is called to read the internal buffer.
		\param	bufferPtr	pointer to internal buffer.
		\param	bytes		number of bytes should be read.
		\return				number of bytes was really read to the buffer.
	*/
	virtual int ReadToBuffer(quint8* bufferPtr, int bytes) = 0;
	/**
		It calculates size of buffer using header.
	*/
	virtual quint32 CalculateMessageSize(const Header& header) = 0;
	/**
		It is called when buffer is read.
	*/
	virtual bool ProcessMessage(const Header& header, const quint8* dataPtr, int length) = 0;

	Header m_header;

	quint32 m_messageSize;
	int m_loadedBytes;

private:
	typedef QVector<quint8> Buffer;
	Buffer m_buffer;
};


template <class Header>
TPopCumulatorBase<Header>::TPopCumulatorBase()
{
	ResetBuffer();
}


template <class Header>
void TPopCumulatorBase<Header>::ResetBuffer()
{
	m_loadedBytes = -int(sizeof(m_header));
	m_buffer.clear();
}


template <class Header>
bool TPopCumulatorBase<Header>::ForceRead()
{
	bool retVal = true;

	while (retVal){
		if (m_loadedBytes < 0){
			int bytesRead = ReadToBuffer((quint8*)&m_header + m_loadedBytes + sizeof(m_header), -m_loadedBytes);
			if (bytesRead < 0){
				return false;
			}

			m_loadedBytes += bytesRead;
			Q_ASSERT(m_loadedBytes <= 0);

			if (m_loadedBytes < 0){
				break;
			}

			m_messageSize = CalculateMessageSize(m_header);
			m_buffer.resize(m_messageSize);
		}

		if 	(m_loadedBytes >= 0){
			int bytesRead = ReadToBuffer(&m_buffer[m_loadedBytes], int(m_messageSize) - m_loadedBytes);
			if (bytesRead < 0){
				return false;
			}

			m_loadedBytes += bytesRead;
			Q_ASSERT(m_loadedBytes <= int(m_messageSize));

			if (m_loadedBytes >= int(m_messageSize)){
				retVal = ProcessMessage(m_header, &m_buffer[0], m_loadedBytes);
				ResetBuffer();
			}
			else{
				break;
			}
		}
	}

	return retVal;
}


} // namespace icomm


