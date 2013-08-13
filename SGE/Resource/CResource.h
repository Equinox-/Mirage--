#ifndef INCLUDED_RESOURCE_H
#define INCLUDED_RESOURCE_H

//====================================================================================================
// Filename:	CResource.h
// Created by:	Peter Chan
// Description:	Base class for reference counted resource. Each resource is assigned a unique ID. To
//			  make use of the reference counting mechanism, the resource must be created/destroyed
//			  by CResourceManager, which is a friend class to CResource.
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "../Utility/Key.h"

//====================================================================================================
// Class Declarations
//====================================================================================================

class CResource
{
	// We declare CResourceManager as a friend to grant it access to private functions in
	// the CResource class.
	friend class CResourceManager;

public:
	// Constructor
	CResource(void);

	// Destructor
	virtual ~CResource(void);

	// Functions to create/destroy the resource. All sub-classes need to specify how they should
	// be created and destroyed.
	virtual void Load(const char* pFilename) = 0;
	virtual void Unload(void) = 0;

	// Function to return the ID of this resource
	Utility::Key GetResourceID(void) const;

	// Function to return the reference count of the resource
	int GetRefCount(void) const;

private:
	// Function to assign an ID to this resource
	void SetResourceID(Utility::Key kID);

	// Function to increment the reference count of the resource. This is called whenever
	// CResourceManager returns a pointer to an instance of CResource.
	void IncrementRefCount(void);

	// Function to decrement the reference count of the resource. This is called whenever
	// CResourceManager tries to destroy a resource.
	void DecrementRefCount(void);

	// Members
	Utility::Key	mResourceID;   // "Unique" resource id generated from a string
	int			 mRefCount;	 // Reference count of the resource
};

#endif // #ifndef INCLUDED_RESOURCE_H
