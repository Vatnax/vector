#pragma once

#include <iostream>

namespace err
{
	const char* const increment_end = "Cannot increment the end iterator!";
	const char* const decrement_begin = "Cannot decrement the begin iterator!";
	const char* const deref_end = "Cannot dereference the end iterator!";
	const char* const increment_rend = "Cannot increment the rend iterator!";
	const char* const decrement_rbegin = "Cannot decrement the rbegin iterator!";
	const char* const deref_rend = "Cannot dereference the rend iterator!";
	const char* const pop_empty_vector = "Cannot pop an empty list!";
	const char* const front_empty_vector = "front() called on an empty vector!";
	const char* const back_empty_vector = "back() called on an empty vector!";
	const char* const diff_vectors = "Iterators are from different Vectors!";
	const char* const traversed_vector = "Vector traversed!";
	const char* const subscript_out_of_range = "Vector subscript out of range!";

	void exit_if(bool cnd, const char* msg);
}