// ImageprocessDoc.cpp : implementation of the CImageprocessDoc class
//

#include "stdafx.h"
#include "Imageprocess.h"

#include "ImageprocessDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CImageprocessDoc

IMPLEMENT_DYNCREATE(CImageprocessDoc, CDocument)

BEGIN_MESSAGE_MAP(CImageprocessDoc, CDocument)
	//{{AFX_MSG_MAP(CImageprocessDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImageprocessDoc construction/destruction

CImageprocessDoc::CImageprocessDoc()
{
	// TODO: add one-time construction code here

}

CImageprocessDoc::~CImageprocessDoc()
{
}

BOOL CImageprocessDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CImageprocessDoc serialization

void CImageprocessDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CImageprocessDoc diagnostics

#ifdef _DEBUG
void CImageprocessDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CImageprocessDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CImageprocessDoc commands
