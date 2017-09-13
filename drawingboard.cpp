#include "drawingboard.h"

// DrawingBoard Class definitions
drawingBoard::drawingBoard(int h, int w, pixel *m_C)
{
	pixMap = new pixelMap(h,w);
	// Set masks for mouse events
    add_events(Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK | Gdk::POINTER_MOTION_MASK);
    //startvalues
    buttonPressed = false;
    mb_C = m_C;
}
drawingBoard::~drawingBoard()
{
}
bool drawingBoard::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
	cr->save();
	Gtk::Allocation allocation = get_allocation();
	const int width = allocation.get_width();
	const int height = allocation.get_height();

	Glib::RefPtr<Gdk::Pixbuf> image = Gdk::Pixbuf::create_from_data(pixMap->map, Gdk::COLORSPACE_RGB, false, 8, pixMap->get_width(), pixMap->get_height(), pixMap->get_stride());
	Gdk::Cairo::set_source_pixbuf(cr, image, 0, 0);
	cr->rectangle(0, 0, image->get_width(), image->get_height());
	cr->fill();
	
	cr->restore();
	
	return true;
}

// Mouse button press event
bool drawingBoard::on_button_press_event(GdkEventButton *event)
{
	// Check if the event is a left(1) button click.
    if( (event->type == GDK_BUTTON_PRESS) && (event->button == 1) )
    {
		buttonPressed = true;
		x1=event->x;
		y1=event->y;
    }
	// The event has been handled.
    return true;
}

bool drawingBoard::on_button_release_event (GdkEventButton* event)
{
	if(event->type == GDK_BUTTON_RELEASE)
    {
		buttonPressed = false;
		x2=event->x;
		y2=event->y;
	}
	if((x1!=x2) | (y1!=y2))
	{
		switch(toolSelected)
		{
			case Line:
				drawLine(x1,y1,x2,y2);
				break;
			case Circle:
				drawCircle(x1,y1,x2,y2);
				break;
			case Rectange:
				drawSquare(x1,y1,x2,y2);
				break;
		}
	}
	queue_draw();
	// The event has been handled.
    return true;
}

bool drawingBoard::on_motion_notify_event (GdkEventMotion* event)
{
	if(buttonPressed)
	{
		int x = event->x;
		int y = event->y;
		
		int pH = pixMap->get_height();
		int pW = pixMap->get_width();
		int pS = pixMap->get_stride();
		int th = 1;
		
		if(toolSelected == Pencil)
		{
			plotPoint(&x,&y,&pH,&pW,&pS,&th);
		}
		else if(toolSelected == Eraser)
		{
			th = 8;
			for(int yy = y-th; yy<(y+th); yy++)
			{
				for(int xx = x-th; xx<(x+th); xx++)
				{
					pixMap->map[(yy*pS + 3*xx + 0)] = 255;
					pixMap->map[(yy*pS + 3*xx + 1)] = 255;
					pixMap->map[(yy*pS + 3*xx + 2)] = 255;
				}
			}
		}
		queue_draw();
	}
	// The event has been handled.
    return true;
}

void drawingBoard::fill(pixel *_color)
{
	int pH = pixMap->get_height();
	int pW = pixMap->get_width();
	int pS = pixMap->get_stride();
	for(int h = 0; h<pH; h++)
	{
		for(int w = 0; w<pW; w++)
		{
			pixMap->map[(h*pS + 3*w + 0)] = _color->r();
			pixMap->map[(h*pS + 3*w + 1)] = _color->g();
			pixMap->map[(h*pS + 3*w + 2)] = _color->b();
		}
	}
}

void drawingBoard::drawLine(int x1,int y1,int x2,int y2)
{
	int pH = pixMap->get_height();
	int pW = pixMap->get_width();
	int pS = pixMap->get_stride();
	int th = 1;
	
	float m = (float(y2-y1)/(x2-x1));
	float b = y1-(m*x1);
	int pX,pY;
	
	int low,high;
	if(abs(x2-x1) > abs(y2-y1)){
		if(x2-x1 > 0){
			low = x1;
			high = x2;
		}
		else{
			low = x2;
			high = x1;
		}
		for(int x=low;x<high;x++)
		{
			pX = x;
			pY = int((m*x)+b);
			plotPoint(&pX,&pY,&pH,&pW,&pS,&th);
		}
	}
	else{
		if(y2-y1 > 0){
			low = y1;
			high = y2;
		}
		else{
			low = y2;
			high = y1;
		}
		for(int y=low;y<high;y++)
		{
			pY = y;
			if(x1 == x2) pX = x1;
			else pX = int((y-b)/m);
			plotPoint(&pX,&pY,&pH,&pW,&pS,&th);
		}
	}
}

void drawingBoard::drawCircle(int x1,int y1,int x2,int y2)
{
	int pH = pixMap->get_height();
	int pW = pixMap->get_width();
	int pS = pixMap->get_stride();
	int th = 1;
	
	float r = sqrt(((y1-y2)*(y1-y2))+((x1-x2)*(x1-x2)));
	int pX,pY;
	
	for(float a=0; a<6.28; a+=0.008)
	{
		cout<<a<<"\n";
		pX = x1+int(r*cos(a));
		pY = y1+int(r*sin(a));
		plotPoint(&pX,&pY,&pH,&pW,&pS,&th);
	}
}

void drawingBoard::drawSquare(int x1,int y1,int x2,int y2)
{
	drawLine(x1,y1,x2,y1);
	drawLine(x2,y1,x2,y2);
	drawLine(x2,y2,x1,y2);
	drawLine(x1,y2,x1,y1);
}

void drawingBoard::plotPoint(int *x, int*y, int *h, int *w, int*s, int*th)
{
	for(int yy = *y-*th; yy<(*y+*th); yy++)
	{
		for(int xx = *x-*th; xx<(*x+*th); xx++)
		{
			pixMap->map[(yy*(*s) + 3*xx + 0)] = mb_C->r();
			pixMap->map[(yy*(*s) + 3*xx + 1)] = mb_C->g();
			pixMap->map[(yy*(*s) + 3*xx + 2)] = mb_C->b();
		}
	}
}







// Pixel Class definitions
pixel::pixel(guint8 r,guint8 g,guint8 b)
{
	pixel::_r = r;
	pixel::_g = g;
	pixel::_b = b;
}
pixel::~pixel()
{
}
void pixel::r(guint8 _val){ pixel::_r = _val;}
guint8 pixel::r(){ return pixel::_r;}
void pixel::g(guint8 _val){ pixel::_g = _val;}
guint8 pixel::g(){ return pixel::_g;}
void pixel::b(guint8 _val){ pixel::_b = _val;}
guint8 pixel::b(){ return pixel::_b;}


// PixelMap Class definitions
pixelMap::pixelMap(int w, int h)
{
	pixelMap::_w = w;
	pixelMap::_h = h;
	pixelMap::_stride = w*3;
	map = new guint8[_w*3*_h]();
}
pixelMap::~pixelMap()
{
}
int pixelMap::get_height() {return _h;}
int pixelMap::get_width() {return _w;}
int pixelMap::get_stride() {return _stride;}


