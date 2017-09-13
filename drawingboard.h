#ifndef _DRAWINGBOARD_H
#define _DRAWINGBOARD_H

#include <gtkmm.h>
#include <gdkmm.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>

using namespace std;

enum Tool {Pencil, Eraser, Line, Circle, Rectange};

// Pixel
class pixel
{
	public:
		pixel(guint8,guint8,guint8);
		virtual ~pixel();
		void r(guint8);
		guint8 r();
		void g(guint8);
		guint8 g();
		void b(guint8);
		guint8 b();
	protected:
		guint8 _r,_g,_b;
};


// PixelMap Class
class pixelMap
{
	public:
		pixelMap(int w, int h);
		virtual ~pixelMap();
		int get_height();
		int get_width();
		int get_stride();
		guint8 *map;
	protected:
		int _h,_w;
		int _stride;
};


// DrawingBoard Class
class drawingBoard : public Gtk::DrawingArea
{
	public:
		drawingBoard(int h, int w, pixel *m_C);
		virtual ~drawingBoard();
		void fill(pixel *_color);
		pixel *mb_C;
		Tool toolSelected;
	protected:
		// Override the draw function
		bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
		// Override mouse events
        bool on_button_press_event(GdkEventButton * event);
        bool on_button_release_event(GdkEventButton* event);
        bool on_motion_notify_event(GdkEventMotion* event);
        void drawLine(int x1,int y1,int x2,int y2);
        void drawCircle(int x1,int y1,int x2,int y2);
        void drawSquare(int x1,int y1,int x2,int y2);
        void plotPoint(int *x, int*y, int *h, int *w, int*s, int*th);
		pixelMap *pixMap;
	private:
        int x1;
        int y1;
        int x2;
        int y2;
        bool buttonPressed;
};

#endif //_DRAWINGBOARD_H
