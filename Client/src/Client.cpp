#include "Client.h"

Client::Client() 
{
}

Client::Stage Client::getCurrentStage()
{
	return m_currentStage;
}

sf::UdpSocket & Client::getSocket()
{
	return m_socket;
}


