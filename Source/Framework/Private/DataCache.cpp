#include "DataCache.h"

#include "Serializer.h"

#include <memory>
#include <iostream>
#include <algorithm>

void DataCache::AddScope(const std::string& scopeName)
{
	std::shared_ptr<DataScope> newScope = std::make_shared<DataScope>();
	newScope->parent = currentScope;
	newScope->name = scopeName;

	if (currentScope.expired())
	{
		dataCache.emplace_back(newScope);
	}
	else
	{
		currentScope.lock()->children.emplace_back(newScope);
	}

	currentScope = newScope;
}

void DataCache::AddOnUpdateCallbackToCurrentScope(const std::function<OnUpdateCallbackType>& callback)
{
	currentScope.lock()->onUpdateCallbacks.push_back(callback);
}

void DataCache::AddPointerToCurrentScope(RTCL::Serializer::PointerType* pointer, RTCL::SerializedType type)
{
	currentScope.lock()->pointerVectorMap[type].push_back(pointer);
}

void DataCache::AddPointerListToCurrentScope(const std::vector<RTCL::Serializer::PointerType*>& pointers, RTCL::SerializedType type)
{
	auto& pointerVector = currentScope.lock()->pointerVectorMap[type];
	pointerVector.insert(pointerVector.end(), pointers.begin(), pointers.end());
}

void DataCache::EndScope()
{
	// add asserts

	currentScope = currentScope.lock()->parent;
}

void DataCache::EndAllScopes()
{
	// add asserts

	currentScope.reset();
}

// TEST: Only for testing purposes, so not optimized and cleaned
void DataCache::PrintData()
{
	for (auto& parent : dataCache)
	{
		PrintDataInternal(parent.get(), 0);
	}
}

// TEST: Only for testing purposes, so not optimized and cleaned
void DataCache::PrintDataInternal(DataScope* scope, int indent)
{
	PrintIndent(indent);
	std::cout << "Scope: " << scope->name << std::endl;

	for (const auto& ptrPair : scope->pointerVectorMap)
	{
		PrintIndent(indent + 1);
		std::cout << "Type: " << static_cast<int>(ptrPair.first) << std::endl;

		for (auto* ptr : ptrPair.second)
		{
			PrintIndent(indent + 2);
			std::cout << "Pointer: " << (unsigned long long)ptr << std::endl;

			PrintIndent(indent + 2);
			std::cout << "Serialized Value: ";

			auto serializedValue = RTCL::Serializer::SerializeByEnumType(ptrPair.first, ptr);

			std::for_each(serializedValue.begin(), serializedValue.end(), [](char value)
				{
					std::cout << (int)value << "/";
				});

			std::cout << std::endl;
		}
	}

	std::cout << std::endl;

	for (const auto& child : scope->children)
	{
		PrintDataInternal(child.get(), indent + 3);
	}
}

// TEST: Only for testing purposes, so not optimized and cleaned
void DataCache::PrintIndent(int indent)
{
	for (int i = 0; i < indent; ++i)
	{
		std::cout << "\t";
	}
}
