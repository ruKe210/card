#include "UndoModel.h"
#include "cocos2d.h"

using namespace cocos2d;

UndoModel::UndoModel()
{
}

UndoModel::~UndoModel()
{
	clearUndoRecords();
}

void UndoModel::addUndoRecord(const UndoRecord& record)
{
	// 存储堆上副本，所有权由 UndoModel 管理
	UndoRecord* rec = new UndoRecord(record);
	_undoRecords.push_back(rec);
}

UndoRecord* UndoModel::popUndoRecord()
{
	if (_undoRecords.empty()) {
		return nullptr;
	}
	UndoRecord* rec = _undoRecords.back();
	_undoRecords.pop_back();
	return rec; // 所有权移交给调用者
}

bool UndoModel::hasUndoRecords() const
{
	return !_undoRecords.empty();
}

void UndoModel::clearUndoRecords()
{
	for (UndoRecord* rec : _undoRecords) {
		delete rec;
	}
	_undoRecords.clear();
}

