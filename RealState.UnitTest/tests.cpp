#include "CppUnitTest.h"
#include "..\RealState\utility.h"
#include "..\RealState\sha256.h"

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

		TEST_METHOD(Hash256Test)
		{
			Assert::IsTrue(hash256_hex_string(string("asqhar")) == "9eaefa62fda3808d722e9c3c463a7f3ab361315bfeae34366aea0c164c790440");
		}
	};
}