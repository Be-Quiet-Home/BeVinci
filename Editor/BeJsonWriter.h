#ifndef BEVINCI_BE_JSON_WRITER_H
#define BEVINCI_BE_JSON_WRITER_H

#include <stddef.h>
#include <stdint.h>

#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

class BeJsonWriter
{
public:
	BeJsonWriter()
		:
		fBuffer(),
		fWriter(fBuffer)
	{
	}

	bool StartObject()
	{
		return fWriter.StartObject();
	}

	bool EndObject()
	{
		return fWriter.EndObject();
	}

	bool StartArray()
	{
		return fWriter.StartArray();
	}

	bool EndArray()
	{
		return fWriter.EndArray();
	}

	bool Key(const char *name)
	{
		return fWriter.Key(name != nullptr ? name : "");
	}

	bool String(const char *value)
	{
		return fWriter.String(value != nullptr ? value : "");
	}

	bool Bool(bool value)
	{
		return fWriter.Bool(value);
	}

	bool Int(int value)
	{
		return fWriter.Int(value);
	}

	bool Uint(unsigned int value)
	{
		return fWriter.Uint(value);
	}

	bool Int64(int64_t value)
	{
		return fWriter.Int64(value);
	}

	bool Uint64(uint64_t value)
	{
		return fWriter.Uint64(value);
	}

	bool Double(double value)
	{
		return fWriter.Double(value);
	}

	const char *Data() const
	{
		return fBuffer.GetString();
	}

	size_t Size() const
	{
		return fBuffer.GetSize();
	}

private:
	typedef rapidjson::Writer<rapidjson::StringBuffer> WriterType;

	BeJsonWriter(const BeJsonWriter&);
	BeJsonWriter& operator=(const BeJsonWriter&);

	rapidjson::StringBuffer	fBuffer;
	WriterType				fWriter;
};

#endif
