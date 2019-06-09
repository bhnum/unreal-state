#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\RealState\timeutility.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace RealStateUnitTest
{		
	TEST_CLASS(AllTests)
	{
	public:
		
		TEST_METHOD(TimeUtilityTest)
		{
			std::stringstream ss;
			ss << puttime(system_clock::now()) << " " << putdate(system_clock::now());
			time_point t, tt;
			ss >> gettime(t) >> getdate(tt);
			Assert::AreEqual(puttime(system_clock::now()), puttime(t));
			Assert::AreEqual(putdate(system_clock::now()), putdate(t));
		}
	};
}