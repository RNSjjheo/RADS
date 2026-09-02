//---------------------------------------------------------------------------

#ifndef RBaseH
#define RBaseH

#include <vcl.h>
#include "../UnitDM.h"
#include "../Define.h"

#include <System.SysUtils.hpp>

//---------------------------------------------------------------------------
class TRBase
{
public: 	// Attribute

public:		// Method
	TRBase();
	~TRBase();
	virtual void Clear() = 0;
	virtual bool Insert() = 0;
	virtual bool Update() = 0;
	virtual bool Delete() = 0;
	virtual bool IsExist() = 0;
	virtual bool Check(int iAction) = 0;
	virtual bool GetData() = 0;
};

#endif
