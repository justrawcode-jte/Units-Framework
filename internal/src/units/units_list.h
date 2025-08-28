#pragma once
#include <list>
#include "./units_macros.h"


namespace Units {
	namespace list {
		template<typename Type_t>
		UNITS_NODISCARD inline typename std::list<Type_t>::iterator GetListIteratorFromPointer( std::list<Type_t>* list_ptr, const Type_t* item_ptr ) {
			return std::find_if(
				list_ptr->begin(),
				list_ptr->end(),
				[item_ptr]( const Type_t& item ){
					return &item == item_ptr;
				}
			);
		}
	} // namespace list
} // namespace Units
