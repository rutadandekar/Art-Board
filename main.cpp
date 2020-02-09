#include "drawingboard.h"

static Glib::RefPtr<Gtk::Application> app;
static Glib::RefPtr<Gtk::Application> app2;

Gtk::Window* pWindow = nullptr;
Gtk::Box* pBox = nullptr;
Gtk::ColorChooserDialog* pColor = nullptr;
Gtk::Button* pButton_choose_color = nullptr;
Gtk::Button* pButton_draw_pencil = nullptr;
Gtk::Button* pButton_draw_line = nullptr;
Gtk::Button* pButton_draw_square = nullptr;
Gtk::Button* pButton_draw_circle = nullptr;
Gtk::Button* pButton_draw_eraser = nullptr;

drawingBoard *myBoard;
static Gdk::RGBA m_Color;
static pixel mb_Color(0,0,0);

void choose_color_clicked_cb()
{
	Gtk::ColorChooserDialog dialog("Please choose a color");
	dialog.set_transient_for(*pWindow);

	//Get the previously selected color:
	dialog.set_rgba(m_Color);

	const int result = dialog.run();

	//Handle the response:
	switch(result)
	{
		case Gtk::RESPONSE_OK:
		{
		  //Store the chosen color:
		  m_Color = dialog.get_rgba();
		  break;
		}
		case Gtk::RESPONSE_CANCEL:
		{
		  break;
		}
		default:
		{
		  std::cout << "Unexpected button clicked: " << result << std::endl;
		  break;
		}
	}
	mb_Color.r(m_Color.get_red_u()/256);
	mb_Color.g(m_Color.get_green_u()/256);
	mb_Color.b(m_Color.get_blue_u()/256);
}

void draw_pencil_clicked_cb()
{
	myBoard->toolSelected = Pencil;
}

void draw_line_clicked_cb()
{
	myBoard->toolSelected = Line;
}

void draw_square_clicked_cb()
{
	myBoard->toolSelected = Rectangle;
}

void draw_circle_clicked_cb()
{
	myBoard->toolSelected = Circle;
}

void draw_eraser_clicked_cb()
{
	myBoard->toolSelected = Eraser;
}

int main(int argc, char* argv[])
{	
	app = Gtk::Application::create(argc, argv, "paint.by.ruta");
	app2 = Gtk::Application::create(argc, argv, "paint.by.ruta2");
	Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_file("window.glade");
	builder->get_widget("window1", pWindow);
	builder->get_widget("box2", pBox);
	builder->get_widget("choose_color", pButton_choose_color);
	builder->get_widget("draw_pencil", pButton_draw_pencil);
	builder->get_widget("draw_line", pButton_draw_line);
	builder->get_widget("draw_square", pButton_draw_square);
	builder->get_widget("draw_circle", pButton_draw_circle);
	builder->get_widget("draw_eraser", pButton_draw_eraser);
	
	pButton_choose_color->signal_clicked().connect(sigc::ptr_fun(&choose_color_clicked_cb));
	pButton_draw_pencil->signal_clicked().connect(sigc::ptr_fun(&draw_pencil_clicked_cb));
	pButton_draw_line->signal_clicked().connect(sigc::ptr_fun(&draw_line_clicked_cb));
	pButton_draw_square->signal_clicked().connect(sigc::ptr_fun(&draw_square_clicked_cb));
	pButton_draw_circle->signal_clicked().connect(sigc::ptr_fun(&draw_circle_clicked_cb));
	pButton_draw_eraser->signal_clicked().connect(sigc::ptr_fun(&draw_eraser_clicked_cb));
		
	myBoard = new drawingBoard(640,480,&mb_Color);
	pixel white(255,255,255);
	myBoard->fill(&white);
	pBox->add(*myBoard);
	
	 // Size of the window
    pWindow->resize(1300,490);
	myBoard->show();
	app->run(*pWindow);
	
	return 0;
}
