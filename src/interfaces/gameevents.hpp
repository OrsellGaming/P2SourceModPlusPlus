/*******************************************************************
* @file   gameevents.hpp
* @brief  Source Engine game events interface.
* @author Orsell
* @date   07 2025
*********************************************************************/

#pragma once

#ifndef GAMEEVENT_HPP
#define GAMEEVENT_HPP

#include <cstdint>

class bf_write;

class IGameEventVisitor2
{
public:
	virtual bool VisitLocal(const char* name, const void* value) { return true; }
	virtual bool VisitString(const char* name, const char* value) { return true; }
	virtual bool VisitFloat(const char* name, float value) { return true; }
	virtual bool VisitInt(const char* name, int value) { return true; }
	virtual bool VisitUint64(const char* name, uint64_t value) { return true; }
	virtual bool VisitWString(const char* name, const wchar_t* value) { return true; }
	virtual bool VisitBool(const char* name, bool value) { return true; }
};

class IGameEvent
{
public:
	virtual ~IGameEvent() = default;
	virtual const char* GetName() const = 0;

	virtual bool  IsReliable() const = 0;
	virtual bool  IsLocal() const = 0;
	virtual bool  IsEmpty(const char* keyName = nullptr) const = 0;

	virtual bool			GetBool(const char* keyName = nullptr, bool defaultValue = false) const = 0;
	virtual int				GetInt(const char* keyName = nullptr, int defaultValue = 0) const = 0;
	virtual uint64_t		GetUint64(const char* keyName = nullptr, uint64_t defaultValue = 0) const = 0;
	virtual float			GetFloat(const char* keyName = nullptr, float defaultValue = 0.0f) const = 0;
	virtual const char*		GetString(const char* keyName = nullptr, const char* defaultValue = "") const = 0;
	virtual const wchar_t*	GetWString(const char* keyName = nullptr, const wchar_t* defaultValue = L"") const = 0;
	virtual const void*		GetPtr(const char* keyName = nullptr) const = 0;

	virtual void SetBool(const char* keyName, bool value) = 0;
	virtual void SetInt(const char* keyName, int value) = 0;
	virtual void SetUint64(const char* keyName, uint64_t value) = 0;
	virtual void SetFloat(const char* keyName, float value) = 0;
	virtual void SetString(const char* keyName, const char* value) = 0;
	virtual void SetWString(const char* keyName, const wchar_t* value) = 0;
	virtual void SetPtr(const char* keyName, const void* value) = 0;

	virtual bool ForEventData(IGameEventVisitor2* event) const = 0;
};

#define EVENT_DEBUG_ID_INIT			42
#define EVENT_DEBUG_ID_SHUTDOWN		13

class IGameEventListener2
{
public:
	virtual	~IGameEventListener2() = default;

	virtual void FireGameEvent(IGameEvent* event) = 0;
	virtual int	 GetEventDebugID() = 0;
};

class IGameEventManager2
{
public:
	virtual	~IGameEventManager2() = default;
	virtual int LoadEventsFromFile(const char* filename) = 0;
	virtual void  Reset() = 0;

	virtual bool AddListener(IGameEventListener2* listener, const char* name, bool bServerSide) = 0;
	virtual bool FindListener(IGameEventListener2* listener, const char* name) = 0;
	virtual void RemoveListener(IGameEventListener2* listener) = 0;
	virtual bool AddListenerGlobal(IGameEventListener2* listener, bool bServerSide) = 0;

	virtual IGameEvent* CreateEvent(const char* name, bool bForce = false, int* pCookie = nullptr) = 0;
	virtual bool FireEvent(IGameEvent* event, bool bDontBroadcast = false) = 0;
	virtual bool FireEventClientSide(IGameEvent* event) = 0;
	virtual IGameEvent* DuplicateEvent(IGameEvent* event) = 0;
	virtual void FreeEvent(IGameEvent* event) = 0;

	virtual bool SerializeEvent(IGameEvent* event, bf_write* eventMsg) = 0;
	virtual IGameEvent* UnserializeEvent(const bf_write& eventMsg) = 0;
};

#endif