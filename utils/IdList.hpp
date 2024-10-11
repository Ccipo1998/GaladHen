
// A vector of objects having an id corresponding to the index inside the vector (id = 0 -> not valid)

#include <vector>

template <class T>
class IdList : public std::vector<T>
{
public:

	IdList()
		: NextId(0)
		{}

	unsigned int AddWithId()
	{
		if (NextId >= size())
		{
			push_back(T{});

			return ++NextId;
		}

		unsigned int next = *(unsigned int*)(&operator[](NextId));
		operator[](NextId) = T{};
		unsigned int old = NextId;
		NextId = next;

		return old + 1;
	}

	unsigned int AddWithId(T object)
	{
		if (NextId >= size())
		{
			push_back(object);

			return ++NextId;
		}

		unsigned int next = *(unsigned int*)(&operator[](NextId));
		operator[](NextId) = object;
		unsigned int old = NextId;
		NextId = next;

		return old + 1;
	}

	void RemoveWithId(unsigned int objectId)
	{
		T* object = ptr(operator[](objectId - 1));
		object->~T();

		*(unsigned int*)(&operator[](objectId - 1)) = NextId;
		NextId = objectId - 1;
	}

	T& GetObjectWithId(unsigned int objectId)
	{
		return operator[](objectId - 1);
	}

protected:

	T* ptr(T& object)
	{
		return &object;
	}

	T* ptr(T* object)
	{
		return object;
	}


	unsigned int NextId;

};
