#ifndef ANDROID_REF_BASE_H
#define ANDROID_REF_BASE_H

#include "yytmp.h"

#pragma once


class ReferenceConverterBase {
public:
    virtual size_t getReferenceTypeSize() const = 0;
    virtual void* getReferenceBase(void const*) const = 0;
    inline virtual ~ReferenceConverterBase() { }
};

class RefBase
{
//public:
//	RefBase(void);
//	~RefBase(void);
public:
	void            incStrong(const void* id) const;
	void            decStrong(const void* id) const;

	void            forceIncStrong(const void* id) const;

	//! DEBUGGING ONLY: Get current strong ref count.
	int32_t         getStrongCount() const;

	class weakref_type
	{
	public:
		RefBase*            refBase() const;

		void                incWeak(const void* id);
		void                decWeak(const void* id);

		// acquires a strong reference if there is already one.
		bool                attemptIncStrong(const void* id);

		// acquires a weak reference if there is already one.
		// This is not always safe. see ProcessState.cpp and BpBinder.cpp
		// for proper use.
		bool                attemptIncWeak(const void* id);

		//! DEBUGGING ONLY: Get current weak ref count.
		int32_t             getWeakCount() const;

		//! DEBUGGING ONLY: Print references held on object.
		void                printRefs() const;

		//! DEBUGGING ONLY: Enable tracking for this object.
		// enable -- enable/disable tracking
		// retain -- when tracking is enable, if true, then we save a stack trace
		//           for each reference and dereference; when retain == false, we
		//           match up references and dereferences and keep only the 
		//           outstanding ones.

		void                trackMe(bool enable, bool retain);
	};

	weakref_type*   createWeak(const void* id) const;

	weakref_type*   getWeakRefs() const;

	//! DEBUGGING ONLY: Print references held on object.
	inline  void            printRefs() const { getWeakRefs()->printRefs(); }

	//! DEBUGGING ONLY: Enable tracking of object.
	inline  void            trackMe(bool enable, bool retain)
	{ 
		getWeakRefs()->trackMe(enable, retain); 
	}

	typedef RefBase basetype;

protected:
	RefBase();
	virtual                 ~RefBase();

	//! Flags for extendObjectLifetime()
	enum {
		OBJECT_LIFETIME_STRONG  = 0x0000,
		OBJECT_LIFETIME_WEAK    = 0x0001,
		OBJECT_LIFETIME_MASK    = 0x0001
	};

	void            extendObjectLifetime(int32_t mode);

	//! Flags for onIncStrongAttempted()
	enum {
		FIRST_INC_STRONG = 0x0001
	};

	virtual void            onFirstRef();
	virtual void            onLastStrongRef(const void* id);
	virtual bool            onIncStrongAttempted(uint32_t flags, const void* id);
	virtual void            onLastWeakRef(const void* id);

private:
	friend class ReferenceMover;
	static void moveReferences(void* d, void const* s, size_t n,
		const ReferenceConverterBase& caster);

private:
	friend class weakref_type;
	class weakref_impl;

	RefBase(const RefBase& o);
	RefBase&        operator=(const RefBase& o);

	weakref_impl* const mRefs;

};


#endif