#ifndef DNS_PACKET_H
#define DNS_PACKET_H

#include <cstdint>
#include <vector>

class DnsPacket
{
	public:
		enum QueryResponse
		{
			QUERY = 0,
			RESPONSE = 1
		};

		enum Opcode
		{
			QUERY = 0,
			IQUERY = 1,
			STATUS = 2,
			NOTIFY = 4,
			UPDATE = 5
		};

		enum AuthoritativeAnswer
		{
			NOT_AUTHORITATIVE = 0,
			AUTHORITATIVE = 1
		};

		enum Truncated
		{
			NOT_TRUNCATED = 0,
			TRUNCATED = 1
		};

		enum RecursionDesired
		{
			RECURSION_NOT_DESIRED = 0,
			RECURSION_DESIRED = 1
		};

		enum RecursionAvailable
		{
			RECURSION_NOT_AVAILABLE = 0,
			RECURSION_AVAILABLE = 1
		};

		enum AuthenticatedData
		{
			NOT_AUTHENTICATED = 0,
			AUTHENTICATED = 1
		};

		enum CheckingDisabled
		{
			CHECKING_ENABLED = 0,
			CHECKING_DISABLED = 1
		};

		enum ReturnCode
		{
			NO_ERROR = 0,
			FORMAT_ERROR = 1,
			SERVER_FAILURE = 2,
			NAME_ERROR = 3,
			NOT_IMPLEMENTED = 4,
			REFUSED = 5,
			YXDOMAIN = 6,
			YXRRSET = 7,
			NXRRSET = 8,
			NOT_AUTHORITATIVE = 9,
			NOT_WITHIN_ZONE = 10,
			BAD_VERSION = 16,
			BAD_KEY = 17,
			BAD_TIME = 18,
			BAD_MODE = 10,
			BAD_NAME = 20,
			BAD_ALGORITHM = 21,
			BAD_TRUNCATION = 22
		};

		DnsPacket(std::uint16_t identification, QueryResponse queryResponse,
		          Opcode opcode, AuthoritativeAnswer authoritativeAnswer,
		          Truncated truncated, RecursionDesired recursionDesired,
		          RecursionAvailable recursionAvailable,
		          AuthenticatedData authenticatedData,
		          CheckingDisabled checkingDisabled, ReturnCode returnCode,
		          const std::vector<Query> &questions,
		          const std::vector<ResourceRecord> &answerRecords,
		          const std::vector<ResourceRecord> &authorityRecords,
		          const std::vector<ResourceRecord> &additionalRecords);

};

#endif // DNS_PACKET_H
