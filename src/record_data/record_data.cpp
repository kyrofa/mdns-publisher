#include "record_data/record_data.h"

RecordData::~RecordData()
{
}

bool RecordData::operator==(const RecordData &other) const
{
	std::vector<std::uint8_t> buffer1, buffer2;

	toBuffer(buffer1);
	other.toBuffer(buffer2);

	return buffer1 == buffer2;
}
