// AirTransportView.h : interface of the CAirTransportView class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include <atldlgs.h>

#include <qr/bitmap.h>
#include <qr/bitstream.h>
#include <qr/code.h>
#include <qr/data.h>
#include <qr/version.h>
#include <qr/parse.h>
#include <atlfile.h>
#include <vector> 
class CAirTransportView : public CWindowImpl<CAirTransportView>
{
	CComBSTR m_bstrFileName;
	CAtlFile m_File;
	std::vector<char> m_vecBytes;
	int m_Max,m_n; 
	std::vector<COLORREF> m_Bitmap;
	int m_w,m_h;
public:
	DECLARE_WND_CLASS(NULL)

	BOOL PreTranslateMessage(MSG* pMsg);

	BEGIN_MSG_MAP(CAirTransportView)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_TIMER, OnTime)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnTime(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	void OpenData();
};
