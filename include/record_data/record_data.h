#ifndef RECORD_DATA_H
#define RECORD_DATA_H

#include <cstdint>
#include <vector>

#include "record_type.h"
#include "record_data/record_data.h"

class RecordData
{
	public:
		virtual ~RecordData();

		virtual RecordType type() const = 0;
		virtual std::uint16_t size() const = 0;
		virtual void toBuffer(std::vector<std::uint8_t> &buffer) const = 0;
};

#endif // RECORD_DATA_H
