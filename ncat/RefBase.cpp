//#include "StdAfx.h"
#include "RefBase.h"


RefBase::RefBase(void):mRefs(0)
{
	OutputDebugString("Hello");
}


RefBase::~RefBase(void)
{
}


//--------------------------
void RefBase::onFirstRef()
{
}
void RefBase::onLastStrongRef(const void* /*id*/)
{
}

bool RefBase::onIncStrongAttempted(uint32_t flags, const void* id)
{
    return (flags&FIRST_INC_STRONG) ? true : false;
}

void RefBase::onLastWeakRef(const void* /*id*/)
{
}