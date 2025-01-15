#pragma once

#include <asio.hpp>

namespace XAL {

	class ClientNetwork {
	public:
		ClientNetwork();
		~ClientNetwork();

		void OnAttach();
		void OnDetach();
		void OnUpdate();
		
	public:
		bool IsConnected() const { return m_ConnectionAccepted; };
		void Send(const std::string& message);
	private:
		asio::io_context m_IO;
		asio::ip::tcp::socket m_Socket;
		bool m_ConnectionAccepted;
	};

}