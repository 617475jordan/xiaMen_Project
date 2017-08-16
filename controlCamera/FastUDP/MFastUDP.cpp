#include "MFastUDP.h"

bool MFastUDP::Initialize(const char * fileName)
{
	if (!m_reader.Initialize(fileName))
	{
		return false;
	}
	
	int iClientIndex = atoi(m_reader.m_list[0].c_str());
	int iClientNum = m_reader.m_list.size() - 1;

	//Save all client port information
	int i, j;
	for (i = 0; i < iClientNum; i++)
	{
		string szInfo = m_reader.m_list[i + 1];
		string szIP;
		string szPort;
		int iPort;
		for (j = 0; j < szInfo.length(); j++)
		{
			if (szInfo[j] == ' ')
			{
				szIP = szInfo.substr(0, j);
				szPort = szInfo.substr(j + 1, szInfo.length() - j - 1);
				iPort = atoi(szPort.c_str());
				//First create udp endpoint with ip address and port number
				udp::endpoint temp_end_point(boost::asio::ip::address::from_string(szIP), iPort);
				m_vecIP.push_back(szIP);
				m_vecPort.push_back(iPort);
				//Then save the endpoint with its' port number into map
				m_endPointVector.push_back(temp_end_point);
				break;
			}
		}
	}

	//Create self endpoint
	boost::system::error_code ec;
	udp::endpoint self_end_point = m_endPointVector[iClientIndex];
	m_endPointVector.erase(m_endPointVector.begin() + iClientIndex);

	//Receive socket
	m_socket_receive.open(boost::asio::ip::udp::v4());
	m_socket_receive.bind(self_end_point, ec);
	if (ec)
	{
		string sz = "本机IP地址接受端错误, 请重新设置!";
		MessageBoxA(NULL, sz.c_str(), "Warning", IDOK);
		return false;
	}

	//Send socket
	string szSendIP = m_vecIP[iClientIndex];
	int iPort_send = m_vecPort[iClientIndex] * 2;
	udp::endpoint send_end_point(boost::asio::ip::address::from_string(szSendIP), iPort_send);
	m_socket_send.open(boost::asio::ip::udp::v4());
	m_socket_send.bind(send_end_point, ec);
	if (ec)
	{
		string sz = "本机IP地址发送端错误, 请重新设置!";
		MessageBoxA(NULL, sz.c_str(), "Warning", IDOK);
		return false;
	}

	boost::thread t_receive(boost::bind(&MFastUDP::GetMessageThread, this));

	return true;
}

int MFastUDP::SendMessage(const char * pInputBuffer, int iLength)
{
	//Define error
	boost::system::error_code ignored_error;
	//Define message data and initial it
	MMessageData message;
	message.Initialize(pInputBuffer, iLength);
	size_t stSend = 0;

	if (m_endPointVector.empty())
	{
		return 0;
	}
	else
	{
		//Use map to send message
		int i;
		for (i = 0; i < m_endPointVector.size(); i++)
		{
			//Send to every udp endpoint client
			stSend = m_socket_send.send_to(boost::asio::buffer(message.GetMessageBuffer(), iLength),
				m_endPointVector[i], 0, ignored_error);

			//If error
			if (ignored_error)
			{
				return 0;
			}
		}

		//Return length
		return (int)stSend;
	}
}

void MFastUDP::GetMessageThread()
{
	for (;;)
	{
		//Receive message by local endpoint
		boost::system::error_code error;
		MMessageData message;
		size_t stReceive;

		//fd_set fdWrite;
		fd_set fdRead;
		FD_ZERO(&fdRead);
		FD_SET(m_socket_receive.native(), &fdRead);

		//wait 5 seconds
		timeval tv = {0, 1}; 
		if (select(0, &fdRead, NULL, NULL, &tv) <= 0 || !FD_ISSET(m_socket_receive.native(), &fdRead))
		{
			//return 0;
		}
		else
		{
			//First receive message length and body
			stReceive = m_socket_receive.receive_from(boost::asio::buffer(message.GetMessageBuffer(), max_message_len), 
				m_local_endpoint, 0, error);

			if (stReceive <= max_message_len)
			{
				//m_bErase = false;
				//Copy received buffer to result buffer
				//memcpy(pOutputBuffer, message.m_messageBuffer, stReceive);
				message.SetSize(stReceive);
				//////////////////////////////////////////////////////////////
				boost::mutex::scoped_lock lock(m_io_mutex);

				if (m_vec_message.size() > 500)
				{
					m_vec_message.erase(m_vec_message.begin());
				}
				m_vec_message.push_back(message);
				//////////////////////////////////////////////////////////////
				//Return correct received message length
				//return (int)stReceive;
				//m_bErase = true;
			}
		}
		Sleep(1);
	}
}

int MFastUDP::GetResultMessage(char * pOutputBuffer, int iMaxLength)
{
	if (m_vec_message.size())
	{
		memcpy(pOutputBuffer, m_vec_message[0].GetMessageBuffer(), m_vec_message[0].GetMessageLen());
		int iLength = m_vec_message[0].GetMessageLen();
		boost::mutex::scoped_lock lock(m_io_mutex);
		m_vec_message.erase(m_vec_message.begin());
	
		return iLength;
	}
	else
	{
		return 0;
	}
}

void MFastUDP::Clear()
{
	m_reader.Clear();
	m_endPointVector.clear();
	m_socket_send.close();
	m_socket_receive.close();
}