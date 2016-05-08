#ifndef DNS_PACKET_H
#define DNS_PACKET_H

#include <cstdint>
#include <vector>

#include "query.h"
#include "resource_record.h"

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
			OPCODE_QUERY = 0,
			OPCODE_IQUERY = 1,
			OPCODE_STATUS = 2,
			OPCODE_NOTIFY = 4,
			OPCODE_UPDATE = 5
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
			RETURN_NO_ERROR = 0,
			RETURN_FORMAT_ERROR = 1,
			RETURN_SERVER_FAILURE = 2,
			RETURN_NAME_ERROR = 3,
			RETURN_NOT_IMPLEMENTED = 4,
			RETURN_REFUSED = 5,
			RETURN_YXDOMAIN = 6,
			RETURN_YXRRSET = 7,
			RETURN_NXRRSET = 8,
			RETURN_NOT_AUTHORITATIVE = 9,
			RETURN_NOT_WITHIN_ZONE = 10,
			RETURN_BAD_VERSION = 16,
			RETURN_BAD_KEY = 17,
			RETURN_BAD_TIME = 18,
			RETURN_BAD_MODE = 10,
			RETURN_BAD_NAME = 20,
			RETURN_BAD_ALGORITHM = 21,
			RETURN_BAD_TRUNCATION = 22
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

		void toBuffer(std::vector<std::uint8_t> &buffer);

		std::uint16_t id() const;
		QueryResponse queryResponse() const;
		Opcode opCode() const;
		AuthoritativeAnswer authoritativeAnswer() const;
		Truncated truncated() const;
		RecursionDesired recursionDesired() const;
		RecursionAvailable recursionAvailable() const;
		AuthenticatedData authenticatedData() const;
		CheckingDisabled checkingDisabled() const;
		ReturnCode returnCode() const;
		const std::vector<Query> &questions() const;
		const std::vector<ResourceRecord>& answerRecords() const;
		const std::vector<ResourceRecord>& authorityRecords() const;
		const std::vector<ResourceRecord>& additionalRecords() const;

	private:
		std::uint16_t m_id;
		QueryResponse m_queryResponse;
		Opcode m_opCode;
		AuthoritativeAnswer m_authoritativeAnswer;
		Truncated m_truncated;
		RecursionDesired m_recursionDesired;
		RecursionAvailable m_recursionAvailable;
		AuthenticatedData m_authenticatedData;
		CheckingDisabled m_checkingDisabled;
		ReturnCode m_returnCode;
		std::vector<Query> m_questions;
		std::vector<ResourceRecord> m_answerRecords;
		std::vector<ResourceRecord> m_authorityRecords;
		std::vector<ResourceRecord> m_additionalRecords;
};

#endif // DNS_PACKET_H
