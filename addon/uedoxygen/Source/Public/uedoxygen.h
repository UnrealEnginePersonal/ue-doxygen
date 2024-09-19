//
// Created by User on 19/09/2024.
//

#ifndef UEDOXYGEN_H
#define UEDOXYGEN_H

#include <string>

struct RunOptions;

namespace doxygen::unreal
{
	class uedoxygen
	{
	public:
		static void runDoxygen(const RunOptions& runOptions);
	};

} // unreal
// doxygen

#endif //UEDOXYGEN_H
