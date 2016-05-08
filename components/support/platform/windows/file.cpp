#include <Windows.h>

#include <support/platform/file.hpp>

namespace Kiaro
{
	namespace Platform
	{
		namespace File
		{
			struct InternalStructure
			{
				HANDLE mFile;
				HANDLE mMapping;
			};

			void MemoryMappedFile::open(void)
			{
				// First, attempt to actually map the file
				HANDLE file = CreateFile(mFilePath.data(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
				HANDLE mapping = CreateFileMapping(file, NULL, PAGE_READWRITE, mLength, mOffset, NULL);
				mAddress = MapViewOfFile(mapping, FILE_MAP_ALL_ACCESS, mLength, mOffset, mLength);

				// If everything worked, alloc the structure and go
				InternalStructure* structure = new InternalStructure();
				structure->mFile = file;
				structure->mMapping = mapping;

				// Keep track of the platform data
				mData = reinterpret_cast<void*>(structure);
			}

			void MemoryMappedFile::close(void)
			{
				if (mData)
				{
					UnmapViewOfFile(mAddress);

					InternalStructure* structure = reinterpret_cast<InternalStructure*>(mData);
					CloseHandle(structure->mMapping);
					CloseHandle(structure->mFile);

					delete mData;
					mData = nullptr;
				}
			}
		}
	}
}