// AirTransportView.cpp : implementation of the CAirTransportView class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "AirTransportView.h"

BOOL CAirTransportView::PreTranslateMessage(MSG* pMsg)
{
	pMsg;
	return FALSE;
}
#include <atlimage.h>
void CAirTransportView::OpenData()
{
	/*CImage img;
	img.Load(L"d:\\qr.png");
	
	void* pBit = img.GetBits();
	int nPitch = img.GetPitch();
	qr_data* pData = NULL;
	int n = qr_code_parse(pBit,abs(nPitch) * 8,abs(nPitch),abs( img.GetHeight() ),&pData);*/


	CShellFileOpenDialog dlg;
	dlg.GetPtr()->SetTitle(L"MyFileOpenDialog");
	
	if(dlg.DoModal() != IDOK)
		return ;
	TCHAR path[1024];
	HRESULT hr = dlg.GetFilePath(path,1024);
	if(hr != S_OK)
		return;

	m_bstrFileName = path;
	this->Invalidate();
	m_File.Create(m_bstrFileName,FILE_GENERIC_READ,FILE_SHARE_READ ,OPEN_EXISTING);
	ULONGLONG N;
	m_File.GetSize(N);
	m_Max = N / 1024;
	m_n = 0;

	SetTimer(10010,1000); 

}

void Fill(std::vector<COLORREF>& dc,qr_bitmap * bmp,int cell)
{
		unsigned char * row;
        int x, y;
		//int cell = 3;
        
        row = bmp->bits;
		dc.resize(bmp->height * bmp->width * cell * cell);
		dc.assign(dc.size(),RGB(255,255,255));
		
        for (y = -4; y < (int)bmp->height + 4; ++y) {

                if (y < 0 || y >= (int)bmp->height) {
                        for (x = 0; x < (int)bmp->width + 8; ++x)
						{
						}
                        continue;
                }


                for (x = 0; x < (int)bmp->width; ++x) {

                        int mask = 1 << x % CHAR_BIT;
                        int byte = row[x / CHAR_BIT];
						if(byte & mask) 
						{
							for(int xx =0;xx<cell;xx++)
							{
								for(int yy =0;yy<cell;yy++)
								{
									
									int i = (x * cell + xx) + (y * cell + yy) * bmp->width * cell;
									dc[i] = 0;
									//dc.SetPixel(tx + x * cell + xx,ty + y * cell + yy,0);
								}
							}
						} 
                }
 
                row += bmp->stride;
        }
}
LRESULT CAirTransportView::OnTime(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	//1274
	int nLen = 1024;
	m_vecBytes.resize(nLen);
	DWORD nRead;
	m_File.Read(&m_vecBytes[0],nLen,nRead);
	if(nRead < m_vecBytes.size())
		m_vecBytes.resize(nRead);
	if(m_vecBytes.empty())
	{
		KillTimer(10010);
		return S_OK;
	}
	
	m_n++;
	//this->Invalidate(FALSE);

	qr_data* pData = qr_data_create(0,QR_EC_LEVEL_L,QR_DATA_8BIT,&m_vecBytes[0],m_vecBytes.size());
	if(NULL != pData)
	{
		qr_code* pCode = qr_code_create(pData);
		Fill(m_Bitmap,pCode->modules,3);
		m_w = pCode->modules->width * 3;
		m_h = pCode->modules->height * 3;

		qr_data* pDataParser = NULL;
		int n = qr_code_parse(pCode->modules->bits,pCode->modules->width ,pCode->modules->width* 8,pCode->modules->height,&pDataParser);


		qr_data_destroy(pData);
		qr_code_destroy(pCode);
		this->Invalidate(FALSE);
	}

	return S_OK;
}
 
void Write(CPaintDC& dc,int tx,int ty,qr_bitmap * bmp)
{
		unsigned char * row;
        int x, y;
		int cell = 3;
        
        row = bmp->bits;

        for (y = -4; y < (int)bmp->height + 4; ++y) {

                if (y < 0 || y >= (int)bmp->height) {
                        for (x = 0; x < (int)bmp->width + 8; ++x)
						{
						}
                        continue;
                }


                for (x = 0; x < (int)bmp->width; ++x) {

                        int mask = 1 << x % CHAR_BIT;
                        int byte = row[x / CHAR_BIT];
						if(byte & mask) 
						{
							for(int xx =0;xx<cell;xx++)
							{
								for(int yy =0;yy<cell;yy++)
								{
									dc.SetPixel(tx + x * cell + xx,ty + y * cell + yy,0);
								}
							}
						}
                       // fprintf(file, "%c ", (byte & mask) ? '1' : '0');
                }

                //fputs("0 0 0 0\n", file);
                row += bmp->stride;
        }
}
#include <atltypes.h>
LRESULT CAirTransportView::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{ 
	CPaintDC dc(m_hWnd); 
	CBrush brush;
	brush.CreateSolidBrush(RGB(255,255,255));
	
	CRect rect;
	GetClientRect(&rect);

	dc.FillRect(&rect,brush);
	//TODO: Add your drawing code here
	if(m_bstrFileName.Length() >0)
	{
		dc.TextOut(0,0,m_bstrFileName);
		char buff[1024];
		sprintf(buff,"%d/%d",m_n,m_Max);

		dc.TextOut(0,30,CComBSTR(buff));
	}
	if(!m_vecBytes.empty())
	{
		BITMAPINFO  bmi;
		 memset(&bmi, 0, sizeof(bmi));
		bmi.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
		bmi.bmiHeader.biWidth       = m_w;
		bmi.bmiHeader.biHeight      = m_h; // top-down image
		bmi.bmiHeader.biPlanes      = 1;
		bmi.bmiHeader.biBitCount    = 32;
		bmi.bmiHeader.biCompression = BI_RGB;
		bmi.bmiHeader.biSizeImage   = m_Bitmap.size() * 4;


		SetDIBitsToDevice(dc,50,50,m_w,m_h,0,0,0,m_h,&m_Bitmap[0],&bmi,0);

		
		/*qr_data* pData = qr_data_create(0,QR_EC_LEVEL_L,QR_DATA_8BIT,&m_vecBytes[0],m_vecBytes.size());
		if(NULL != pData)
		{
			qr_code* pCode = qr_code_create(pData);
			Write(dc,100,100,pCode->modules);
			
			qr_data_destroy(pData);
			qr_code_destroy(pCode);
		}*/
	}

	return 0;
}
