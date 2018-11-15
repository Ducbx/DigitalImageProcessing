; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CImageprocessView
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "Imageprocess.h"
LastPage=0

ClassCount=10
Class1=CImageprocessApp
Class2=CImageprocessDoc
Class3=CImageprocessView
Class4=CMainFrame

ResourceCount=7
Resource1=IDD_ABOUTBOX
Class5=CAboutDlg
Resource2=IDD_DIALOG1
Class6=CDialog1
Resource3=IDD_DIALOG3
Class7=CDialog2
Resource4=IDD_DIALOG4
Class8=CDialog3
Resource5=IDD_DIALOG2
Class9=CDialog4
Resource6=IDR_MAINFRAME
Class10=CDialog5
Resource7=IDD_DIALOG5

[CLS:CImageprocessApp]
Type=0
HeaderFile=Imageprocess.h
ImplementationFile=Imageprocess.cpp
Filter=N

[CLS:CImageprocessDoc]
Type=0
HeaderFile=ImageprocessDoc.h
ImplementationFile=ImageprocessDoc.cpp
Filter=N

[CLS:CImageprocessView]
Type=0
HeaderFile=ImageprocessView.h
ImplementationFile=ImageprocessView.cpp
Filter=C
BaseClass=CView
VirtualFilter=VWC
LastObject=CImageprocessView


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
LastObject=ID_FFT




[CLS:CAboutDlg]
Type=0
HeaderFile=Imageprocess.cpp
ImplementationFile=Imageprocess.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_OPEN
Command2=ID_APP_EXIT
Command3=ID_Gray
Command4=ID_FFT
Command5=ID_IFFT
CommandCount=5

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE
CommandCount=14

[TB:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
Command8=ID_APP_ABOUT
CommandCount=8

[DLG:IDD_DIALOG1]
Type=1
Class=CDialog1
ControlCount=7
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_EDIT1,edit,1350631552
Control6=IDC_STATIC,static,1342308352
Control7=IDC_EDIT2,edit,1350631552

[CLS:CDialog1]
Type=0
HeaderFile=Dialog1.h
ImplementationFile=Dialog1.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CDialog1

[DLG:IDD_DIALOG2]
Type=1
Class=CDialog2
ControlCount=10
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_EDIT1,edit,1350631552
Control6=IDC_STATIC,static,1342308352
Control7=IDC_EDIT2,edit,1350631552
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342308352
Control10=IDC_EDIT3,edit,1350631552

[CLS:CDialog2]
Type=0
HeaderFile=Dialog2.h
ImplementationFile=Dialog2.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_EDIT1

[DLG:IDD_DIALOG3]
Type=1
Class=CDialog3
ControlCount=7
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_EDIT1,edit,1350631552
Control6=IDC_STATIC,static,1342308352
Control7=IDC_EDIT2,edit,1350631552

[CLS:CDialog3]
Type=0
HeaderFile=Dialog3.h
ImplementationFile=Dialog3.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CDialog3

[DLG:IDD_DIALOG4]
Type=1
Class=CDialog4
ControlCount=10
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_EDIT1,edit,1350631552
Control6=IDC_STATIC,static,1342308352
Control7=IDC_EDIT2,edit,1350631552
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342308352
Control10=IDC_EDIT3,edit,1350631552

[CLS:CDialog4]
Type=0
HeaderFile=Dialog4.h
ImplementationFile=Dialog4.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CDialog4

[DLG:IDD_DIALOG5]
Type=1
Class=CDialog5
ControlCount=12
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_EDIT1,edit,1350631552
Control6=IDC_STATIC,static,1342308352
Control7=IDC_EDIT2,edit,1350631552
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342308352
Control10=IDC_EDIT3,edit,1350631552
Control11=IDC_STATIC,static,1342308352
Control12=IDC_EDIT4,edit,1350631552

[CLS:CDialog5]
Type=0
HeaderFile=Dialog5.h
ImplementationFile=Dialog5.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CDialog5

