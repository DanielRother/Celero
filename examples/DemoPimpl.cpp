#include <celero/Celero.h>
#include <celero/Utilities.h>

#include <thread>
#include <chrono>

///
/// This is the main(int argc, char** argv) for the entire celero program.
/// You can write your own, or use this macro to insert the standard one into the project.
///
CELERO_MAIN

//
// Baseline with a non-pimpl'd class.
// 

class NoImpl
{
	public:
		NoImpl() :
			value(0)
		{
		}

		int getValue() const
		{
			return this->value;
		}

	private:
		int value;
};

//
// Define an Impl class to use in several of our tests.
// 

class GenericImpl
{
	public:
		GenericImpl() :
			value(0)
		{
		}

		int value;
};

//
// Raw Pointer implementation 
// 

class RawPointer
{
	public:
		RawPointer() :
			pimpl(new GenericImpl)
		{
		}

		~RawPointer()
		{
			delete pimpl;
		}

		int getValue() const
		{
			return this->pimpl->value;
		}

	private:
		GenericImpl* pimpl;
};

//
// std::unique_ptr<> implementation 
// 

class UniquePointer
{
	public:
		UniquePointer() :
			pimpl(new GenericImpl)
		{
		}

		int getValue() const
		{
			return this->pimpl->value;
		}

	private:
		std::unique_ptr<GenericImpl> pimpl;
};

//
// Create a Herb Sutter'esque Pimpl Class
// 

#include <celero/Pimpl.h>

class SutterPointer
{
	public:
		int getValue() const;

	private:
		class Impl;
		celero::Pimpl<Impl> pimpl;
};

#include <celero/PimplImpl.h>

class SutterPointer::Impl
{
	public:
		Impl() :
			value(0)
		{
		}

		int value;
};

int SutterPointer::getValue() const
{
	return this->pimpl->value;
}

//
// A basic test fixture for our tests to share.
// 

class DemoFixture : public celero::TestFixture
{
	public:
		NoImpl noPtr;
		RawPointer rawPtr;
		UniquePointer uniquePtr;
		SutterPointer sutterPtr;
};

// The number of calls is very large because the operation is very quick.  We want to 
// look at a large enough chunk of time to reduce the reliance on the resolution of the 
// clock.  Then, do 30 samples of this large number of runs and keep the quickest.
BASELINE_F(DemoPimpl, Baseline, DemoFixture, 30, 1000000000)
{
	celero::DoNotOptimizeAway(noPtr.getValue());
}

BENCHMARK_F(DemoPimpl, RawPtr, DemoFixture, 30, 1000000000)
{
	celero::DoNotOptimizeAway(rawPtr.getValue());
}

BENCHMARK_F(DemoPimpl, UniquePtr, DemoFixture, 30, 1000000000)
{
	celero::DoNotOptimizeAway(uniquePtr.getValue());
}

BENCHMARK_F(DemoPimpl, SutterPtr, DemoFixture, 30, 1000000000)
{
	celero::DoNotOptimizeAway(sutterPtr.getValue());
}
