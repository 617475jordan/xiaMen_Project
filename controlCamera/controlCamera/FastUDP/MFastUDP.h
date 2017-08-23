#pragma once
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>
#include <string>
#include <vector>
#include <map>
#include "MParamReader.h"
using std::vector;
using namespace std;
using boost::asio::ip::udp;
#define max_len 2000
#define max_message_len 2048
//#define send_port 1111

class MFastUDP
{
public:

	/*
	\breif
	Define message structure
	*/
#pragma pack(1)
	typedef struct _MessageData 
	{
		//Total message length
		int m_iMessageLength;
		//Send message buffer
		char m_messageBuffer[max_len];

		/*
		\breif
		Initialize the send message data, include: client sender ID, client receiver ID, input message length and message data buffer
		@param pInputBuffer, the input message data
		@param iLength, the input message length
		*/
		bool Initialize(const char * pInputBuffer, int iLength)
		{
			//memset(m_messageBuffer, '\0', max_len);
			m_iMessageLength = iLength;
			//m_iMessageLength = GetSize();

			//And then store input message data into buffer
			memcpy((char*)(&m_messageBuffer[0]), pInputBuffer, iLength);

			return true;
		}

		void SetSize(int iLength)
		{
			m_iMessageLength = iLength;
		}

		int GetMessageLen()
		{
			return m_iMessageLength; 
		}

		char* GetMessageBuffer()
		{
			return m_messageBuffer;
		}

	}MMessageData, *PMMessageData;
#pragma pack()

	MFastUDP(void):
	m_socket_send(m_io_service_send),
	m_socket_receive(m_io_service_receive)
	{}

	~MFastUDP(void)
	{}

	bool Initialize(const char * fileName);

	/*
	\breif
	Send the data from client to another client or group send message to all client
	@param lID, the client ID number, id ID is -1, group send
	@param pInputBuffer, the input message buffer
	@param iLength, the input message length
	*/
	int SendMessage(const char * pInputBuffer, int iLength);

	/*
	\breif
	Get the message from server
	@param pOutputBuffer, the message buffer
	@param lID, the client ID number
	*/
	int GetResultMessage(char * pOutputBuffer, int iMaxLength);

	void GetMessageThread();

	//vector<int>GetPortList();

	/*
	\breif
	Method to clear
	*/
	void Clear();


public:

	boost::asio::io_service m_io_service_send;//UDP io_service
	boost::asio::io_service m_io_service_receive;//UDP io_service
	udp::socket m_socket_send;//UDP socket send
	udp::socket m_socket_receive;//UDP socket receive

	udp::endpoint m_local_endpoint;//UDP remote endpoint
	vector<udp::endpoint> m_endPointVector;//All UDP client endpoint

	boost::mutex m_io_mutex;
	vector<MMessageData> m_vec_message;

	vector<string> m_vecIP;
	vector<int> m_vecPort;

	MParamReader m_reader;
};
