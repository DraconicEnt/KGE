/**
 *  @file Array.h
 *  @brief Include file defining and providing logic for the Kiaro::Support::Array template class.
 *
 *  This software is licensed under the GNU Lesser General Public License version 3.
 *  Please refer to gpl.txt and lgpl.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @version 0.0.0.19
 *  @date 3/19/2014
 *  @copyright (c) 2014 Draconic Entertainment
 */

#ifndef _INCLUDE_KIARO_SUPPORT_ARRAY_H_
#define _INCLUDE_KIARO_SUPPORT_ARRAY_H_

#include <engine/Common.hpp>

namespace Kiaro
{
	namespace Support
	{
		template <class T>
		//! Class utilized for high-level arrays that are less error prone but are slower than raw arrays.
		class Array
		{
			public:
			//! Standard constructor.
			Array(void) : element_count(0), elements_array(0x00)
			{

			}

			/**
			 *	@brief Constructor accepting initial element count.
			 *	@param length A Common::Common::U32 representing the initial element count.
			 */
			Array(Kiaro::Common::U32 length) : element_count(0), elements_array(0x00)
			{
				this->Resize(length);
			}

			//! Standard destructor.
			~Array(void)
			{
				//if (this->elements_array != 0x00)
				//	delete this->elements_array;
			}

			/**
			 *	@brief Returns the length of the Array in element count.
			 *	@return A Kiaro::Common::U32 representing the current length of the Array.
			 */
			Kiaro::Common::U32 Len(void)
			{
				return element_count;
			}

			/**
			 *	@brief Appends an element to the end of the Array.
			 *	@param element The element to append to the Array.
			 */
			void Append(const T &element)
			{
				this->Insert(element, this->element_count);
			}

			/**
			 *	@brief Prepends an element to the start of the Array.
			 *	@param element The element to prepend to the Array.
			 */
			void Prepend(const T &element)
			{
				this->Insert(element, 0);
			}

			/**
			 *	@brief Inserts an element into the Array at the given index.
			 *	@param element The element to insert at the given index.
			 *	@param index A Kiaro::Common::U32 representing the index to insert the element at.
			 */
			 void Insert(const T &element, Kiaro::Common::U32 index)
			 {
				Kiaro::Common::U32 after_index = index + 1;
				Kiaro::Common::U32 before_index = index - 1;

				if (index >= this->element_count)
				    after_index = this->element_count;
				else if (index == 0)
				    before_index = 1;

				T *new_elements_array = new T[this->element_count + 1];
				if (this->elements_array != 0x00)
				{
				    memcpy(&new_elements_array[before_index], &this->elements_array[0], sizeof(T) * index);
				    memcpy(&new_elements_array[after_index], &this->elements_array[index], sizeof(T) * (this->element_count - index));

				    delete[] this->elements_array;
				}
				new_elements_array[index] = element;

				this->elements_array = new_elements_array;
				this->element_count++;
			}

			/**
			 *	@brief Removes an element from the Array at the given index.
			 *	@param index A Kiaro::Common::U32 representing the index to remove.
			 *	@bug This function does not work at all as of the time of this writing.
			 */
			void Remove(Kiaro::Common::U32 index)
			{
				Kiaro::Common::U32 after_index = index + 1;
				Kiaro::Common::U32 before_index = index - 1;

				if (index >= this->element_count)
				    after_index = this->element_count;
				else if (index == 0)
				    before_index = 1;

				T *new_elements_array = new T[this->element_count - 1];
				if (this->elements_array)
				{
				     memcpy(&new_elements_array[before_index - 1], &this->elements_array[before_index], sizeof(T) * index);
				     delete[] this->elements_array;
				}

				this->elements_array = new_elements_array;
				this->element_count--;
			}

			/**
			 *	@brief Resizes the Array.
			 *	@param size A Kiaro::Common::U32 representing the new size of the Array in elements.
			 *	@warning If size is lower than the current size of the Array, then all elements after that index will be lost
			 *	during the memory reallocation process.
			 */
			void Resize(Kiaro::Common::U32 size)
			{
				T *new_elements_array = new T[size];
				memcpy(new_elements_array, &this->elements_array[0], this->element_count * sizeof(T));
				if (this->elements_array)
				    delete this->elements_array;

				this->elements_array = new_elements_array;
				this->element_count = size;
			}

			/**
			*	@brief Overloaded array operator.
			*	@param index A Kiaro::U32 representing the index to access.
			*/
			T& operator [](Kiaro::Common::U32 index)
			{
				return this->elements_array[index];
			}

			private:
			//! A Kiaro::Common::U32 representing the total number of elements stored on this Array.
			Kiaro::Common::U32 element_count;
			//! The actual internal array storing our elements.
			T *elements_array;
		};
	} // End NameSpace Support
} // End NameSpace Kiaro
#endif // _INCLUDE_KIARO_SUPPORT_ARRAY_H_
