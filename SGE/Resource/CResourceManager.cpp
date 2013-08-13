 //===================================================================================================
// Filename:	CResourceManager.cpp
// Created by:	Peter Chan
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "CResourceManager.h"

#include "CResource.h"

//====================================================================================================
// Statics
//====================================================================================================

CResourceManager* CResourceManager::s_pInstance = NULL;

//====================================================================================================
// Class Definitions
//====================================================================================================

CResourceManager* CResourceManager::Get(void)
{
	// If we do not yet have an instance created
	if (NULL == s_pInstance)
	{
		// Create a new instance
		s_pInstance = new CResourceManager;
	}

	return s_pInstance;
}

//----------------------------------------------------------------------------------------------------

CResourceManager::CResourceManager(void) :
	mInitialized(false)
{
	// Empty
}

//----------------------------------------------------------------------------------------------------

CResourceManager::~CResourceManager(void)
{
	// Terminate the resource manager
	Terminate();
}

//----------------------------------------------------------------------------------------------------

void CResourceManager::Initialize(int iPoolSize)
{
	// Check if we have already initialized the manager
	if (mInitialized)
	{
		// Write to log
		CLog::Get()->Write(ELogMessageType_WARNING, "[ResourceManager] Manager already initialized.");
		return;
	}

	// Write to log
	CLog::Get()->Write(ELogMessageType_MESSAGE, "[ResourceManager] Initializing...");

	// Initialize resource pool size
	mResourceList.reserve(iPoolSize);

	// Set flag
	mInitialized = true;

	// Write to log
	CLog::Get()->Write(ELogMessageType_MESSAGE, "[ResourceManager] Manager initialized.");
}

//----------------------------------------------------------------------------------------------------

void CResourceManager::Terminate(void)
{
	// Check if we have already terminated the manager
	if (!mInitialized)
	{
		// Write to log
		CLog::Get()->Write(ELogMessageType_WARNING, "[ResourceManager] Manager already terminated.");
		return;
	}

	// Write to log
	CLog::Get()->Write(ELogMessageType_MESSAGE, "[ResourceManager] Terminating...");

	// Clear the list
	const int kNumResources = mResourceList.size();
	for (int i = 0; i < kNumResources; ++i)
	{
		if (NULL != mResourceList[i])
		{
			delete mResourceList[i];
			mResourceList[i] = NULL;
		}
	}
	mResourceList.clear();

	// Reset flag
	mInitialized = false;

	// Write to log
	CLog::Get()->Write(ELogMessageType_MESSAGE, "[ResourceManager] Manager terminated");
}

//----------------------------------------------------------------------------------------------------

void CResourceManager::Destroy(CResource* pResource)
{
	// Check if the manager is initialized
	if (!mInitialized)
	{
		// Write to log
		CLog::Get()->Write(ELogMessageType_ERROR, "[ResourceManager] Failed to destroy resource. Manager not initialized.");
		return;
	}

	// Get the resource ID
	Utility::Key kID = pResource->GetResourceID();

	// Check if this resource is in the loaded list
	for (std::vector<CResource*>::iterator itr = mResourceList.begin();
		 itr != mResourceList.end();
		 ++itr)
	{
		// If we find it, decrement the ref count
		if (NULL != (*itr) && kID == (*itr)->GetResourceID())
		{
			(*itr)->DecrementRefCount();
			
			// If there are no more reference to it, then we will delete it
			if ((*itr)->GetRefCount() <= 0)
			{
				delete (*itr);
				mResourceList.erase(itr);
				return;
			}
		}
	}
}
