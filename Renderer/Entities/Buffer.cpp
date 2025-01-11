
#include "Buffer.h"

#include <utility>

namespace GaladHen
{
	Buffer::Buffer()
		: Type(BufferType::Uniform)
		, AccessType(BufferAccessType::StaticRead)
	{}

	Buffer::Buffer(const Buffer& source)
	{
		Type = source.Type;
		AccessType = source.AccessType;
		Datas = source.Datas;
		Sizes = source.Sizes;

		InvalidateResource();
	}

	Buffer& Buffer::operator=(Buffer& source)
	{
		Type = source.Type;
		AccessType = source.AccessType;
		Datas = source.Datas;
		Sizes = source.Sizes;

		InvalidateResource();

		return *this;
	}

	Buffer::Buffer(Buffer&& source) noexcept
	{
		Type = source.Type;
		AccessType = source.AccessType;
		Datas = std::move(source.Datas);
		Sizes = std::move(source.Sizes);

		InvalidateResource();
	}

	Buffer& Buffer::operator=(Buffer&& source) noexcept
	{
		Type = source.Type;
		AccessType = source.AccessType;
		Datas = std::move(source.Datas);
		Sizes = std::move(source.Sizes);

		InvalidateResource();

		return *this;
	}

	const std::vector<std::shared_ptr<void>>& Buffer::GetDatas() const
	{
		return Datas;
	}

	const std::vector<size_t>& Buffer::GetSizes() const
	{
		return Sizes;
	}

	BufferType Buffer::GetType() const
	{
		return Type;
	}

	BufferAccessType Buffer::GetAccessType() const
	{
		return AccessType;
	}

	void Buffer::SetType(BufferType type)
	{
		Type = type;

		InvalidateResource();
	}

	void Buffer::SetAccessType(BufferAccessType accessType)
	{
		AccessType = accessType;

		InvalidateResource();
	}

	/*void Buffer::AddData(std::shared_ptr<void> data, size_t size)
	{
		Datas.push_back(data);
		Sizes.push_back(size);

		InvalidateResource();
	}

	void Buffer::InsertData(std::shared_ptr<void> data, size_t size, unsigned int index)
	{
		Datas.insert(Datas.begin(), index, data);
		Sizes.insert(Sizes.begin(), index, size);

		InvalidateResource();
	}*/

	void Buffer::RemoveData(unsigned int index)
	{
		Datas.erase(std::next(Datas.begin(), index));
		Sizes.erase(std::next(Sizes.begin(), index));

		InvalidateResource();
	}
}
