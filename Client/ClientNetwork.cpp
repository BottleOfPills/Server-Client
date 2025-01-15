#include "ClientNetwork.h"

namespace XAL {

	ClientNetwork::ClientNetwork() : m_Socket(m_IO), m_ConnectionAccepted(false) {}

	ClientNetwork::~ClientNetwork()
	{
		OnDetach();
	}

	void ClientNetwork::OnAttach()
	{
		try {
			auto endpoint = asio::ip::tcp::endpoint(
				asio::ip::address::from_string("127.0.0.1"),
				8192
			);

			m_Socket.connect(endpoint);
			if (m_Socket.is_open()) {
				std::printf("Socket Connected!\n");
				m_ConnectionAccepted = true;
			}
		}
		catch (const std::exception& e) {
			std::printf("Connection failed: %s\n", e.what());
			m_ConnectionAccepted = false;
		}
	}

	void ClientNetwork::OnDetach()
	{
		Send("Disconnecting!");
		m_ConnectionAccepted = false;

		if (m_Socket.is_open())
			m_Socket.close();

		m_IO.stop();
	}

	void ClientNetwork::OnUpdate()
	{
		if (m_ConnectionAccepted) {
			m_IO.run();
		}
	}

	void ClientNetwork::Send(const std::string& message)
	{
		if (m_ConnectionAccepted) {
			try {
				m_Socket.send(asio::buffer(message));
				std::printf("Sent: %s\n", message.c_str());
			}
			catch (const std::exception& e) {
				std::printf("Send Error: %s\n", e.what());
			}
		}
	}

}