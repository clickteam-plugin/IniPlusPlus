#pragma once

class SuperINI;

class UndoElement {
public:
	virtual void perform( SuperINI* ini ) = 0;
	virtual ~UndoElement() { }
};

class UndoCompound : public UndoElement {

	typedef std::list<UndoElement*> UndoList;
	UndoList elements;

public:
	UndoCompound() { }
	virtual void perform( SuperINI* ini ) {
		while ( !elements.empty() ) {
			elements.back()->perform(ini);
			delete elements.back();
			elements.pop_back();
		}
	}
	void add( UndoElement* x ) {
		elements.push_back(x);
	}
	bool empty() {
		return elements.empty();
	}
	~UndoCompound() {
		while( !elements.empty() )
		{
			delete elements.back();
			elements.pop_back();
		}
	}
};

typedef std::list<UndoCompound*> UndoStack;
