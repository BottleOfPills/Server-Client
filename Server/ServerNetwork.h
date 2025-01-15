#pragma once

#include <vector>
#include <asio.hpp>

namespace XAL {

	struct ClientInfo {
		asio::ip::tcp::socket socket;
		std::string address{};
		int port{};

		ClientInfo(asio::ip::tcp::socket&& sock)
			: socket(std::move(sock))
		{
			auto endpoint = socket.remote_endpoint();
			address = endpoint.address().to_string();
			port = endpoint.port();
		}
	};

	class ServerNetwork {
	public:
		ServerNetwork(int port);
		~ServerNetwork() = default;

		void OnUpdate();
		void OnDetach();
	private:
		void ReceiveClientData();
	private:
		asio::io_context m_IO;
		asio::ip::tcp::acceptor m_Acceptor;
		std::vector<ClientInfo> m_Clients;
	};

}