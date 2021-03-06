#include "UdpSocket.hpp"

#if defined(ACID_BUILD_WINDOWS)
#include <winsock2.h>
#else
#include <netinet/in.h>
#endif
#include <algorithm>
#include "Engine/Log.hpp"
#include "Network/IpAddress.hpp"
#include "Network/Packet.hpp"

namespace acid
{
	const uint32_t UdpSocket::MAX_DATAGRAM_SIZE = 65507;

	UdpSocket::UdpSocket() :
		Socket(SOCKET_TYPE_UDP),
		m_buffer(MAX_DATAGRAM_SIZE)
	{
	}

	unsigned short UdpSocket::GetLocalPort() const
	{
		if (GetHandle() != Socket::InvalidSocketHandle())
		{
			// Retrieve informations about the local end of the socket.
			sockaddr_in address;
			SocketAddrLength size = sizeof(address);

			if (getsockname(GetHandle(), reinterpret_cast<sockaddr *>(&address), &size) != -1)
			{
				return ntohs(address.sin_port);
			}
		}

		// We failed to retrieve the port.
		return 0;
	}

	SocketStatus UdpSocket::Bind(unsigned short port, const IpAddress &address)
	{
		// Close the socket if it is already bound.
		Close();

		// Create the internal socket if it doesn't exist.
		Create();

		// Check if the address is valid/
		if ((address == IpAddress::NONE) || (address == IpAddress::BROADCAST))
		{
			return SOCKET_STATUS_ERROR;
		}

		// Bind the socket/
		sockaddr_in addr = Socket::CreateAddress(address.ToInteger(), port);

		if (bind(GetHandle(), reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) == -1)
		{
			Log::Error("Failed to bind socket to port %i\n", port);
			return SOCKET_STATUS_ERROR;
		}

		return SOCKET_STATUS_DONE;
	}

	void UdpSocket::Unbind()
	{
		// Simply close the socket.
		Close();
	}

	SocketStatus
	UdpSocket::Send(const void *data, std::size_t size, const IpAddress &remoteAddress, unsigned short remotePort)
	{
		// Create the internal socket if it doesn't exist.
		Create();

		// Make sure that all the data will fit in one datagram.
		if (size > MAX_DATAGRAM_SIZE)
		{
			Log::Error(
				"Cannot send data over the network (the number of bytes to send is greater than UdpSocket::MAX_DATAGRAM_SIZE)\n");
			return SOCKET_STATUS_ERROR;
		}

		// Build the target address.
		sockaddr_in address = Socket::CreateAddress(remoteAddress.ToInteger(), remotePort);

		// Send the data (unlike TCP, all the data is always sent in one call).
		int sent = sendto(GetHandle(), static_cast<const char *>(data), static_cast<int>(size), 0,
		                  reinterpret_cast<sockaddr *>(&address), sizeof(address));

		// Check for errors.
		if (sent < 0)
		{
			return Socket::GetErrorStatus();
		}

		return SOCKET_STATUS_DONE;
	}

	SocketStatus UdpSocket::Receive(void *data, std::size_t size, std::size_t &received, IpAddress &remoteAddress,
	                                unsigned short &remotePort)
	{
		// First clear the variables to fill.
		received = 0;
		remoteAddress = IpAddress();
		remotePort = 0;

		// Check the destination buffer.
		if (!data)
		{
			Log::Error("Cannot receive data from the network (the destination buffer is invalid)\n");
			return SOCKET_STATUS_ERROR;
		}

		// Data that will be filled with the other computer's address.
		sockaddr_in address = Socket::CreateAddress(INADDR_ANY, 0);

		// Receive a chunk of bytes.
		SocketAddrLength addressSize = sizeof(address);
		int sizeReceived = recvfrom(GetHandle(), static_cast<char *>(data), static_cast<int>(size), 0,
		                            reinterpret_cast<sockaddr *>(&address), &addressSize);

		// Check for errors.
		if (sizeReceived < 0)
		{
			return Socket::GetErrorStatus();
		}

		// Fill the sender informations.
		received = static_cast<std::size_t>(sizeReceived);
		remoteAddress = IpAddress(ntohl(address.sin_addr.s_addr));
		remotePort = ntohs(address.sin_port);

		return SOCKET_STATUS_DONE;
	}

	SocketStatus UdpSocket::Send(Packet &packet, const IpAddress &remoteAddress, unsigned short remotePort)
	{
		// UDP is a datagram-oriented protocol (as opposed to TCP which is a stream protocol).
		// Sending one datagram is almost safe: it may be lost but if it's received, then its data
		// is guaranteed to be ok. However, splitting a packet into multiple datagrams would be highly
		// unreliable, since datagrams may be reordered, dropped or mixed between different sources.
		// That's why a limit on packet size so that they can be sent in a single datagram.
		// This also removes the overhead associated to packets -- there's no size to send in addition
		// to the packet's data.

		// Get the data to send from the packet.
		std::size_t size = 0;
		const void *data = packet.OnSend(size);

		// Send it.
		return Send(data, size, remoteAddress, remotePort);
	}

	SocketStatus UdpSocket::Receive(Packet &packet, IpAddress &remoteAddress, unsigned short &remotePort)
	{
		// See the detailed comment in send(Packet) above.

		// Receive the datagram.
		std::size_t received = 0;
		SocketStatus status = Receive(&m_buffer[0], m_buffer.size(), received, remoteAddress, remotePort);

		// If we received valid data, we can copy it to the user packet.
		packet.Clear();

		if ((status == SOCKET_STATUS_DONE) && (received > 0))
		{
			packet.OnReceive(&m_buffer[0], received);
		}

		return status;
	}
}
