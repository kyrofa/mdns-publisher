#ifndef A_RECORD_DATA_H
#define A_RECORD_DATA_H

#include <string>

#include <boost/asio/ip/address_v4.hpp>

#include "record_type.h"
#include "record_data/record_data.h"

class ARecordData: public RecordData
{
	public:
		ARecordData(const std::string &ipAddress);
		ARecordData(const std::vector<std::uint8_t> &buffer);

		virtual RecordType type() const override;

		virtual void toBuffer(
		        std::vector<std::uint8_t> &buffer) const override;

		boost::asio::ip::address_v4 ipAddress() const;

	private:
		boost::asio::ip::address_v4 m_ipAddress;
};

#endif // A_RECORD_DATA_H
