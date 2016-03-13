#ifndef AAAA_RECORD_DATA_H
#define AAAA_RECORD_DATA_H

#include <string>

#include <boost/asio/ip/address_v6.hpp>

#include "record_type.h"
#include "record_data/record_data.h"

class AAAARecordData: public RecordData
{
	public:
		AAAARecordData(const std::string &ipAddress);
		AAAARecordData(const std::vector<std::uint8_t>::const_iterator &begin,
		               const std::vector<std::uint8_t>::const_iterator &end);

		virtual RecordType type() const override;
		virtual std::uint16_t size() const override;

		virtual void toBuffer(
		        std::vector<std::uint8_t> &buffer) const override;

		boost::asio::ip::address_v6 ipAddress() const;

		bool operator==(const AAAARecordData &other) const;

	private:
		boost::asio::ip::address_v6 m_ipAddress;
};

#endif // AAAA_RECORD_DATA_H
