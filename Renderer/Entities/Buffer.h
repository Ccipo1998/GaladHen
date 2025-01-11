
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

	enum class BufferAccessType
	{
		StaticRead,
		StaticWrite
	};

	class Buffer : public IGPUResource
	{
	public:

		Buffer();

		Buffer(const Buffer& source);
		Buffer& operator=(Buffer& source);
		Buffer(Buffer&& source) noexcept;
		Buffer& operator=(Buffer&& source) noexcept;

		const std::vector<std::shared_ptr<void>>& GetDatas() const;
		const std::vector<size_t>& GetSizes() const;
		BufferType GetType() const;
		BufferAccessType GetAccessType() const;

		void SetType(BufferType type);
		void SetAccessType(BufferAccessType accessType);

		template <class T>
		void AddData(const T& data)
		{
			T* newData = new T{};
			*newData = data;
			Datas.emplace_back(std::shared_ptr<void>(newData));
			Sizes.emplace_back(sizeof(T));

			InvalidateResource();
		}
		template <class T>
		void AddData(std::shared_ptr<T> data)
		{
			Datas.emplace_back(data);
			Sizes.emplace_back(sizeof(T));

			InvalidateResource();
		}
		template <class T>
		void InsertData(const T& data, unsigned int index)
		{
			T* newData = new T{};
			*newData = data;
			Datas.insert(Datas.begin() + index, std::shared_ptr<void>(newData));
			Sizes.insert(Sizes.begin() + index, sizeof(T));

			InvalidateResource();
		}
		template <class T>
		void InsertData(std::shared_ptr<T> data, unsigned int index)
		{
			Datas.insert(Datas.begin() + index, data);
			Sizes.insert(Sizes.begin() + index, sizeof(T));

			InvalidateResource();
		}
		template <class T>
		void SetDataAt(const T& data, unsigned int index)
		{
			if (index >= Datas.size())
				return;

			T* newData = new T{};
			*newData = data;
			Datas[index] = std::shared_ptr<void>(newData);
			Sizes[index] = sizeof(T);

			InvalidateResource();
		}

		void RemoveData(unsigned int index);

	protected:

		BufferType Type;
		BufferAccessType AccessType;

		std::vector<std::shared_ptr<void>> Datas;
		std::vector<size_t> Sizes;
	};
}
