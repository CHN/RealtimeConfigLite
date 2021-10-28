#pragma once

#include "Serializer.h"

#include <vector>
#include <string>
#include <map>
#include <functional>
#include <cstdint>
#include <memory>

class DataCache
{
public:

	using OnUpdateCallbackType = void();

	void AddScope(const std::string& scopeName);
	void AddOnUpdateCallbackToCurrentScope(const std::function<OnUpdateCallbackType>& callback);
	void AddPointerToCurrentScope(void* pointer, RTCL::SerializedType type);
	void AddPointerListToCurrentScope(const std::vector<void*>& pointers, SerializedType type);
	void EndScope();
	void EndAllScopes();

	void PrintData(); // TEST: Only for testing purposes, so not optimized and cleaned

private:

	using PointerListMapType = std::map<SerializedType, std::vector<void*>>;

	struct DataScope;

	using DataScopeVectorType = std::vector<std::shared_ptr<DataScope>>;

	struct DataScope
	{
		DataScopeVectorType children;
		std::weak_ptr<DataScope> parent;
		std::string name;
		std::vector<std::function<OnUpdateCallbackType>> onUpdateCallbacks;
		PointerListMapType pointerVectorMap;
	};

	DataScopeVectorType dataCache;

	std::weak_ptr<DataScope> currentScope;

	void PrintDataInternal(DataScope* scope, int indent); // TEST: Only for testing purposes, so not optimized and cleaned
	void PrintIndent(int indent); // TEST: Only for testing purposes, so not optimized and cleaned
};