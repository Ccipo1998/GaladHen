
#pragma once

#include <vector>
#include <memory>

#include "IGPUResource.h"

namespace GaladHen
{
	enum class BufferType
	{
		Uniform,
		ShaderStorage
	};

	enum class BufferAllocationType
	{
		Fixed,
		Dynamic
	};

	enum class BufferAccessType
	{
		StaticRead,
		StaticWrite
	};

	class IBuffer : public IGPUResource
	{
	public:

		IBuffer()
		: Type(BufferType::Uniform)
		, AccessType(BufferAccessType::StaticRead)
		{}
;
		BufferType GetType() const
		{
			return Type;
		}

		BufferAllocationType GetAllocationType()
		{
			return AllocationType;
		}

		BufferAccessType GetAccessType() const
		{
			return AccessType;
		}

		void SetType(BufferType type)
		{
			Type = type;
		}

		void SetAccessType(BufferAccessType accessType)
		{
			AccessType = accessType;
		}

		virtual const void* GetData() const = 0;
		virtual size_t GetBytesSize() const = 0;

	protected:

		BufferType Type;
		BufferAllocationType AllocationType;
		BufferAccessType AccessType;
	};

	template<class T, unsigned int ItemNumber>
	class FixedBuffer : public IBuffer
	{
	public:

		FixedBuffer()
		{
			AllocationType = BufferAllocationType::Fixed;
		}

		FixedBuffer(const FixedBuffer& source)
		{
			Type = source.Type;
			AccessType = source.AccessType;
			Data = source.Data;
		}
		FixedBuffer& operator=(FixedBuffer& source)
		{
			Type = source.Type;
			AccessType = source.AccessType;
			Data = source.Data;

			return *this;
		}
		FixedBuffer(FixedBuffer&& source) noexcept
		{
			Type = source.Type;
			AccessType = source.AccessType;
			Data = source.Data;
		}
		FixedBuffer& operator=(FixedBuffer&& source) noexcept
		{
			Type = source.Type;
			AccessType = source.AccessType;
			Data = source.Data;

			return *this;
		}

		virtual const void* GetData() const override
		{
			return &Data;
		}

		virtual size_t GetBytesSize() const override
		{
			return sizeof(T) * ItemNumber;
		}

		void SetData(T data, unsigned int index)
		{
			Data[index] = data;

			InvalidateResource();
		}

	protected:

		T Data[ItemNumber];
	};

	template<class T>
	class DynamicBuffer : public IBuffer
	{
	public:

		DynamicBuffer()
		{
			AllocationType = BufferAllocationType::Dynamic;
		}

		DynamicBuffer(const DynamicBuffer& source)
		{
			Type = source.Type;
			AccessType = source.AccessType;
			Data = source.Data;
		}
		DynamicBuffer& operator=(DynamicBuffer& source)
		{
			Type = source.Type;
			AccessType = source.AccessType;
			Data = source.Data;

			return *this;
		}
		DynamicBuffer(DynamicBuffer&& source) noexcept
		{
			Type = source.Type;
			AccessType = source.AccessType;
			Data = source.Data;
		}
		DynamicBuffer& operator=(DynamicBuffer&& source) noexcept
		{
			Type = source.Type;
			AccessType = source.AccessType;
			Data = source.Data;

			return *this;
		}

		virtual const void* GetData() const override
		{
			return Data.data();
		}

		virtual size_t GetBytesSize() const override
		{
			return sizeof(T) * Data.size();
		}

		void SetData(T data, unsigned int index)
		{
			Data[index] = data;

			InvalidateResource();
		}

		void AddData(T data)
		{
			Data.push_back(data);
		}

		void InsertData(T data, unsigned int index)
		{
			Data.insert(Data.begin() + index, data);
		}

	protected:

		std::vector<T> Data;
	};
}
