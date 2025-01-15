#include "ServerNetwork.h"

namespace XAL {

	ServerNetwork::ServerNetwork(int port) : m_Acceptor(m_IO, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)) 
	{
		m_Acceptor.non_blocking(true);
		std::printf("Server Initialized\n");
	}

	void ServerNetwork::OnUpdate()
	{
		try {
			auto socket = m_Acceptor.accept();
			if (socket.is_open()) 
			{
				std::printf("Client Connected!\n");
				ClientInfo cInfo(std::move(socket));

				m_Clients.push_back(std::move(cInfo));
			}
		}
		catch (const std::system_error& e) {
			if (e.code() != asio::error::would_block) 
			{
				std::printf("Accept error: %s\n", e.what());
			}
		}

		ReceiveClientData();

		m_IO.run_one();
	}

	void ServerNetwork::OnDetach()
	{
		for (auto& client : m_Clients) 
		{
			if (client.socket.is_open())
				client.socket.close();
		}

		m_Acceptor.close();
		m_IO.stop();
	}

	void ServerNetwork::ReceiveClientData()
	{
		char buffer[1024];
		auto it = m_Clients.begin();
		while (it != m_Clients.end()) 
		{
			try {
				it->socket.receive(asio::buffer(buffer, 1), asio::ip::tcp::socket::message_peek);

				if (!it->socket.is_open()) 
				{
					std::printf("Client disconnected: %s:%d\n", it->address.c_str(), it->port);
					it = m_Clients.erase(it);
					continue;
				}

				while (true) 
				{
					size_t available = it->socket.available();
					if (available == 0) break;

					size_t bytes = it->socket.receive(asio::buffer(buffer, available));
					if (bytes > 0) 
					{
						std::string message(buffer, bytes);
						std::printf("Received: %s from socket: %s\n", message.c_str(), it->address.c_str());
					}
				}

				++it;
			}
			catch (const std::exception& e) {
				std::printf("Client disconnected: %s:%d\n",
					it->address.c_str(), it->port);
				it = m_Clients.erase(it);
			}
		}
	}

}