#ifndef AAAA_RECORD_DATA_H
#define AAAA_RECORD_DATA_H

#include "record_type.h"
#include "record_data/record_data.h"

class AAAARecordData: public RecordData
{
	public:
		AAAARecordData(const std::vector<std::uint8_t> &buffer);

		virtual RecordType type() const override;

		virtual void toBuffer(
		        std::vector<std::uint8_t> &buffer) const override;
};

#endif // AAAA_RECORD_DATA_H
