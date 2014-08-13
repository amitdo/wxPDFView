/////////////////////////////////////////////////////////////////////////////
// Name:        include/PDFView.h
// Purpose:     wxPDFView class
// Author:      Tobias Taschner
// Created:     2014-08-07
// Copyright:   (c) 2014 Tobias Taschner
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef PDFVIEW_H
#define PDFVIEW_H

#include <wx/wx.h>
#include <wx/sharedptr.h>
#include <wx/vector.h>
#include <wx/print.h>
#include <istream>

class wxPDFViewImpl;

/**
   Type of zoom used in wxPDFView
*/
enum wxPDFViewZoomType
{
	/// Free zoom set by SetZoom. Not affected by resizing
	wxPDFVIEW_ZOOM_TYPE_FREE,
	wxPDFVIEW_ZOOM_TYPE_FIT_PAGE,
	wxPDFVIEW_ZOOM_TYPE_PAGE_WIDTH
};

enum wxPDFViewPageNavigation
{
	wxPDFVIEW_PAGE_NAV_NEXT,
	wxPDFVIEW_PAGE_NAV_PREV,
	wxPDFVIEW_PAGE_NAV_FIRST,
	wxPDFVIEW_PAGE_NAV_LAST
};

enum wxPDFViewFindFlags
{
	wxPDFVIEW_FIND_MATCH_CASE = 0x0001,
	wxPDFVIEW_FIND_BACKWARDS = 0x0002,
	wxPDFVIEW_FIND_DEFAULT = 0
};

class wxPDFViewBookmark;

/**
   This control encapsulates display an loading of a PDF document. Use LoadFile
   or LoadStream to load content into the view. This control can be used alone
   or together with the other controls from the wxPDFView library.
   
   For a complete PDF reader you can use wxPDFViewDocumentFrame which combines
   all the libraries classes into a fully function PDF reader ready to be used 
   in your application.

   @see wxPDFViewBookmarksCtrl, wxPDFViewThumbnailListBox
*/
class wxPDFView: public wxScrolledCanvas
{
public:
	wxPDFView() { }

	wxPDFView(wxWindow *parent,
		wxWindowID winid = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxScrolledWindowStyle,
		const wxString& name = wxPanelNameStr);

	virtual ~wxPDFView();

	bool Create(wxWindow *parent,
		wxWindowID winid = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxScrolledWindowStyle,
		const wxString& name = wxPanelNameStr);

	void NavigateToPage(wxPDFViewPageNavigation pageNavigation);

	/**
	   Retrieves the number of pages in the current document
	*/
	int GetPageCount() const;

	/**
	   Returns the root bookmark if the document contains bookmarks
	*/
	const wxPDFViewBookmark* GetRootBookmark() const;

	void SetCurrentPage(int pageIndex);

	int GetCurrentPage() const;

	/**
	   Set the zoom level as float value 1.0 is the original size. Setting the 
	   zoom resets the zoom type to @a wxPDFVIEW_ZOOM_TYPE_FREE

	   @see SetZoomType()
	*/
	void SetZoom(double zoom);

	double GetZoom() const;

	double GetMaxZoom() const;

	double GetMinZoom() const;

	/**
	   Activates a new zoom type

	   @param zoomType One of three supported zoom types

	   @see SetZoom()
	*/
	void SetZoomType(wxPDFViewZoomType zoomType);

	wxPDFViewZoomType GetZoomType() const;

	// Scrolls next result into view and returns the number of matches or wxNOT_FOUND
	long Find(const wxString& text, int flags = wxPDFVIEW_FIND_DEFAULT);

	bool IsPrintAllowed() const;

	// Creates a printout object (returns NULL if printing is not allowed)
	wxPrintout* CreatePrintout() const;

	const wxString& GetDocumentTitle() const;

	/**
	   Load specified file path into the view. If a password protected file is
	   loaded but no password is specified the user will be asked for one.

	   @see LoadStream()
	*/
	bool LoadFile(const wxString& fileName, const wxString& password = wxString());

	/**
	   Use LoadStream to load a PDF from a custom source. The specified stream 
	   muste be seekable. Local files should be loaded via LoadFile()

	   @see LoadFile()
	*/
	bool LoadStream(wxSharedPtr<std::istream> pStream, const wxString& password = wxString());

	/**
	   Closes the current loaded document

	   @see LoadFile(), LoadStream()
	*/
	void CloseDocument();

	wxPDFViewImpl* GetImpl() const { return m_impl; };

	virtual void ScrollWindow(int dx, int dy, const wxRect *rect = NULL);

private:
	wxPDFViewImpl* m_impl;

	friend class wxPDFViewImpl;
};

/**
   PDF Document Bookmark

   If defined in the PDF document can be used to query bookmarks in the document.
   This could be used for display of the bookmarks in the UI. The wxPDFViewBookmarksCtrl 
   control uses this to display it's bookmark tree.

   @see wxPDFViewBookmarksCtrl
*/
class wxPDFViewBookmark: public wxVector< wxSharedPtr<wxPDFViewBookmark> >
{
public:
	/// Get the bookmarks title
	virtual wxString GetTitle() const = 0;

	/** 
	   Navigate to the bookmark in the specified PDF view.

	   @param pdfView wxPDFView in which this bookmark should be navigated to

	   @see wxPDFView
	*/
	virtual void Navigate(wxPDFView* pdfView) = 0;

	virtual ~wxPDFViewBookmark() { }
};

wxDECLARE_EVENT(wxEVT_PDFVIEW_DOCUMENT_READY, wxCommandEvent);
wxDECLARE_EVENT(wxEVT_PDFVIEW_PAGE_CHANGED, wxCommandEvent);
wxDECLARE_EVENT(wxEVT_PDFVIEW_ZOOM_CHANGED, wxCommandEvent);
wxDECLARE_EVENT(wxEVT_PDFVIEW_ZOOM_TYPE_CHANGED, wxCommandEvent);
wxDECLARE_EVENT(wxEVT_PDFVIEW_URL_CLICKED, wxCommandEvent);

#endif // PDFVIEW_H
