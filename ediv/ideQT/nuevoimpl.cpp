#include "nuevoimpl.h"
#include "ideqtwindow.h"
#include "nuevo_propiedades.h"
#include "main.h"

#include <stdio.h>

#include <qlayout.h>
#include <qlistbox.h>
#include <qwhatsthis.h>
#include <qlabel.h>
#include <qgroupbox.h>
#include <qcombobox.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qfiledialog.h>
#include <qmessagebox.h>
#include <qcheckbox.h>
#include <qmultilineedit.h>
#include <qradiobutton.h>
#include <qvgroupbox.h>

#include "gif/b_file.h"
#include "gif/proyecto.h"
#include "gif/PRG.h"
#include "gif/MAP.h"
#include "gif/FPG.h"
#include "gif/PAL.h"
#include "gif/FNT.h"
#include "gif/WLD.h"
#include "gif/MOD.h"
#include "gif/WAV.h"
#include "gif/FLI.h"

#include "gif/PCX.h"
#include "gif/PNG.h"
#include "gif/BMP.h"
#include "gif/JPG.h"

#include "gif/MIDI.h"
#include "gif/IT.h"
#include "gif/XM.h"
#include "gif/S3M.h"

#include "gif/OGG.h"
#include "gif/MP3.h"
#include "gif/WMA.h"
#include "gif/PCM.h"

#include "gif/FLC.h"
#include "gif/MPG.h"
#include "gif/MOV.h"
#include "gif/AVI.h"

#include "gif/ayuda.h"


/* 
 *  Constructs a Nuevo which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
Nuevo::Nuevo( QWidget* parent,  const char* name, bool modal, WFlags fl, QWidget* padre )
    : QDialog( parent, name, modal, fl )
{
	setCaption("Crear archivo nuevo");
	setFixedSize(500,350);

	QPixmap *icono=pillagif(b_file_gif);
	setIcon(*icono);

	if(padre)
		move(padre->x()+padre->width()/2-width()/2,padre->y()+padre->height()/2-height()/2);
	
	QHBoxLayout* hbox1 = new QHBoxLayout(this);

	QListBox* lista = new QListBox(this);
	hbox1->addWidget(lista);
	
	QWhatsThis::add(lista,"Elige en esta lista qué tipo de archivo quieres añadir a tu proyecto.");	

	icono=pillagif(proyecto_gif);
	lista->insertItem(*icono,"Proyecto");
	icono=pillagif(PRG_gif);
	lista->insertItem(*icono,"Programa");
	icono=pillagif(MAP_gif);
	lista->insertItem(*icono,"Mapa");
	icono=pillagif(FPG_gif);
	lista->insertItem(*icono,"Fichero para gráficos");
	icono=pillagif(PAL_gif);
	lista->insertItem(*icono,"Paleta de 8 bits");
	icono=pillagif(FNT_gif);
	lista->insertItem(*icono,"Fuente de letra");
	icono=pillagif(WLD_gif);
	lista->insertItem(*icono,"Mundo 3D");
	icono=pillagif(MOD_gif);
	lista->insertItem(*icono,"Fichero musical");
	icono=pillagif(WAV_gif);
	lista->insertItem(*icono,"Fichero de sonido");
	icono=pillagif(FLI_gif);
	lista->insertItem(*icono,"Vídeo");

	lista->setMinimumWidth(200);
	lista->setSizePolicy(QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Expanding));
	QVBoxLayout* vbox1 = new QVBoxLayout(this);
	hbox1->addLayout(vbox1);

	QGroupBox* box_descripcion = new QGroupBox(1,Qt::Vertical,"Descripción",this);
	vbox1->addWidget(box_descripcion);
	vbox1->setSpacing(6);
	vbox1->setMargin(6);

	descripcion = new QLabel("Elige el tipo de archivo que quieres añadir a tu proyecto.",box_descripcion);
	QWhatsThis::add(descripcion,"Aquí se muestra una descripción del elemento que eliges en la lista de la izquierda.");
	descripcion->setAlignment(Qt::AlignLeft|Qt::AlignTop);
	descripcion->setIndent(2);
	descripcion->setTextFormat(Qt::RichText);
	connect(lista,SIGNAL(highlighted(int)),this,SLOT(ListaClick(int)));

	box_opciones = new QGroupBox(this);
	box_opciones->setTitle("Opciones");
    box_opciones->setColumnLayout(0, Qt::Vertical );

	vbox1->addWidget(box_opciones);

	QVBoxLayout* vbox2 = new QVBoxLayout(box_opciones->layout());

	label1 = new QLabel("Tipo de archivo:",box_opciones);
	label1->setEnabled(FALSE);
	vbox2->addWidget(label1);

	combo_tipo = new QComboBox(box_opciones);
	combo_tipo->setEnabled(FALSE);
	vbox2->addWidget(combo_tipo);
	QWhatsThis::add(combo_tipo,"Si el tipo de recurso que has elegido permite almacenarse en distintos formatos, puedes elegir en esta lista el que más te convenga.");
	connect(combo_tipo,SIGNAL(highlighted(int)),this,SLOT(combo_tipo_selecciona(int)));

	label2 = new QLabel("Ruta y nombre del archivo:",box_opciones);
	label2->setEnabled(FALSE);
	vbox2->addWidget(label2);

	edit_ruta = new QLineEdit(box_opciones);
	edit_ruta->setEnabled(FALSE);
	vbox2->addWidget(edit_ruta);
	QWhatsThis::add(edit_ruta,"Escribe una ruta en esta caja si quieres que el archivo se guarde directamente en el disco nada más crearlo. Con algunos tipos de archivo esto es opcional.");

	QHBoxLayout* hbox2 = new QHBoxLayout(box_opciones);
	//hbox2->setSpacing(6);
	hbox2->setMargin(4);
	vbox2->addLayout(hbox2);

	QSpacerItem* spacer1 = new QSpacerItem(20,20,QSizePolicy::Expanding,QSizePolicy::Minimum);
	hbox2->addItem(spacer1);

	boton_examinar = new QPushButton("Examinar...",box_opciones);
	boton_examinar->setEnabled(FALSE);
	hbox2->addWidget(boton_examinar);
	QWhatsThis::add(boton_examinar,"Pulsa este botón para buscar una ruta donde guardar el archivo.");
	connect(boton_examinar,SIGNAL(clicked()),this,SLOT(Examinar()));

	QHBoxLayout* hbox3 = new QHBoxLayout(this);
	vbox1->addLayout(hbox3);
	hbox3->setSpacing(12);
	hbox3->setMargin(5);

	QSpacerItem* spacer2 = new QSpacerItem(20,20,QSizePolicy::Expanding,QSizePolicy::Minimum);
	hbox3->addItem(spacer2);

	boton_aceptar = new QPushButton("Aceptar",this);
	boton_aceptar->setEnabled(FALSE);
	boton_aceptar->setDefault(TRUE);
	hbox3->addWidget(boton_aceptar);
	connect(boton_aceptar,SIGNAL(clicked()),this,SLOT(Aceptar()));
	QWhatsThis::add(boton_aceptar,"Pulsa este botón para pasar al siguiente paso.");

	QPushButton* boton_cancelar = new QPushButton("Cancelar",this);
	hbox3->addWidget(boton_cancelar);
	connect(boton_cancelar,SIGNAL(clicked()),this,SLOT(Cancelar()));
	QWhatsThis::add(boton_cancelar,"Pulsa este botón para cerrar esta ventana sin crear un nuevo archivo.");
	boton_cancelar->setAccel(Key_Escape);

}


void Nuevo::ListaClick(int index)
{
	QPixmap* icono;
	label1->setEnabled(TRUE);
	combo_tipo->setEnabled(TRUE);
	label2->setEnabled(TRUE);
	edit_ruta->setEnabled(TRUE);
	boton_examinar->setEnabled(TRUE);
	boton_aceptar->setEnabled(TRUE);
	combo_tipo->clear();

	QString directorio;
	if(!Main.proyecto_actual.nombre)
		directorio=Main.personal_folder;
	else
		directorio=Main.proyecto_actual.directorio;
	edit_ruta->setText(directorio + "/<nombre_del_fichero>");
	edit_ruta->setSelection(edit_ruta->text().length()-20,20);
	edit_ruta->setFocus();
	switch(index) {
		case 0: // Proyecto (EPJ)
			set_descripcion(0,0);
			label1->setEnabled(FALSE);
			combo_tipo->setEnabled(FALSE);
			edit_ruta->setText(Main.projects_folder);
			break;
		case 1:	// PRG
			set_descripcion(1,0);
			label1->setEnabled(FALSE);
			combo_tipo->setEnabled(FALSE);
			break;
		case 2:	// MAP
			set_descripcion(2,0);
			icono=pillagif(MAP_gif);			
			combo_tipo->insertItem(*icono,"MAP (Mapa de DIV Games Studio)");
			icono=pillagif(PCX_gif);
			combo_tipo->insertItem(*icono,"PCX (PCSoft PCX)");
			icono=pillagif(PNG_gif);
			combo_tipo->insertItem(*icono,"PNG (Portable Network Graphics)");
			icono=pillagif(BMP_gif);
			combo_tipo->insertItem(*icono,"BMP (Mapa de bits de Windows)");
			icono=pillagif(JPG_gif);
			combo_tipo->insertItem(*icono,"JPG (JPEG Independent Group)");
			break;
		case 3:	// FPG
			set_descripcion(3,0);
			label1->setEnabled(FALSE);
			combo_tipo->setEnabled(FALSE);
			break;
		case 4:	// PAL
			set_descripcion(4,0);
			label1->setEnabled(FALSE);
			combo_tipo->setEnabled(FALSE);
			break;
		case 5: // FNT
			set_descripcion(5,0);
			label1->setEnabled(FALSE);
			combo_tipo->setEnabled(FALSE);
			break;
		case 6: // WLD
			set_descripcion(6,0);
			icono=pillagif(WLD_gif);
			combo_tipo->insertItem(*icono,"WLD (Mapa 3D de DIV Games Studio 2)");
			combo_tipo->insertItem(*icono,"WLD2 (Mapa reducido de M8Designer)");
			break;
		case 7: // MOD/MIDI
			set_descripcion(7,0);
			icono=pillagif(MIDI_gif);
			combo_tipo->insertItem(*icono,"MID (Archivo MIDI)");
			icono=pillagif(IT_gif);
			combo_tipo->insertItem(*icono,"IT (Impulse Tracker)");
			icono=pillagif(XM_gif);
			combo_tipo->insertItem(*icono,"XM (Fast Tracker)");
			icono=pillagif(S3M_gif);
			combo_tipo->insertItem(*icono,"S3M (Scream Tracker)");
			icono=pillagif(MOD_gif);
			combo_tipo->insertItem(*icono,"MOD (ProTracker)");
			break;
		case 8: // WAV
			set_descripcion(8,0);
			icono=pillagif(WAV_gif);
			combo_tipo->insertItem(*icono,"WAV (Microsoft ADPCM)");
			icono=pillagif(OGG_gif);
			combo_tipo->insertItem(*icono,"OGG (OGG Vorbis)");
			icono=pillagif(MP3_gif);
			combo_tipo->insertItem(*icono,"MP3 (Fraunhoffer MP3)");
			icono=pillagif(WMA_gif);
			combo_tipo->insertItem(*icono,"WMA (Windows Media Audio)");
			icono=pillagif(PCM_gif);
			combo_tipo->insertItem(*icono,"PCM (DIV Games Studio 1)");
			break;
		case 9: // FLI
			set_descripcion(9,0);
			icono=pillagif(FLI_gif);
			combo_tipo->insertItem(*icono,"FLI (Autodesk Animator)");
			icono=pillagif(FLC_gif);
			combo_tipo->insertItem(*icono,"FLC (Autodesk Animator Pro)");
			icono=pillagif(MPG_gif);
			combo_tipo->insertItem(*icono,"MPG (Motion Picture Engineering Group)");
			icono=pillagif(MOV_gif);
			combo_tipo->insertItem(*icono,"MOV (Apple QuickTime)");
			icono=pillagif(AVI_gif);
			combo_tipo->insertItem(*icono,"AVI (Audio & Video Interlaced)");
	}
}

void Nuevo::Aceptar()
{
	int ret;

	QDir d;
	
	if(_categoria!=0) {
		QFileInfo f(edit_ruta->text());
		d=f.dir();
	}
	else {
		d.setPath(edit_ruta->text());
	}
		
	if(edit_ruta->text().contains('<') || edit_ruta->text().contains('>') || !d.exists()) {
		if(_categoria==0 || _categoria==1) {
			QMessageBox mb("Ruta o nombre de archivo incorrecto",
				"Para crear un proyecto o un programa debes introducir la ruta correcta.\nLos programas y proyectos deben guardarse en disco al crearlos.",
				QMessageBox::NoIcon,
				QMessageBox::Ok | QMessageBox::Default,
				QMessageBox::NoButton,
				QMessageBox::NoButton);
			mb.exec();
			return;
		}
		else {
			while(1) {
				QMessageBox mb( "¿Crear archivo sin guardar?",
					"No has introducido un nombre de archivo correcto.\n¿Deseas crear el archivo sin guardarlo en disco?",
					QMessageBox::NoIcon,
					QMessageBox::Yes | QMessageBox::Default,
					QMessageBox::Cancel | QMessageBox::Escape,
					QMessageBox::Retry);
				mb.setButtonText(QMessageBox::Yes,"Sí");
				mb.setButtonText(QMessageBox::Cancel,"Cancelar");
				mb.setButtonText(QMessageBox::Retry,"Ayuda");
				ret=mb.exec();
				//if( ret == QMessageBox::Cancel ) { return; }
				if( ret == QMessageBox::Yes ) break;
				else if( ret == QMessageBox::Retry ) {
					QMessageBox mb2( "Ayuda",
						"Si creas el archivo sin guardarlo en disco, el archivo se creará en una ventana del escritorio de eDIV sin escribirse en el disco. La única diferencia será que el archivo no estará realmente guardado en disco hasta que no uses la opción <b>Guardar</b> o <b>Guardar como...</b> del menú <b>Archivo</b>. Es útil no guardar los archivos en disco para usarlos como temporales, por ejemplo, un mapa que sólo vayamos a usar para tratar algunas imágenes y luego copiarlas a otros mapas. Pero ten presente que un juego no podrá acceder a un archivo hasta que esté guardado en disco.",
						QMessageBox::Information,
						QMessageBox::Ok | QMessageBox::Default,
						QMessageBox::NoButton,
						QMessageBox::NoButton);
					QPixmap* icono=pillagif(ayuda_gif);
					mb2.setIconPixmap(*icono);
					mb2.exec();
				}
				else return;
			}
			hide();
			Nuevo_Propiedades* prop = new Nuevo_Propiedades(_categoria,_tipo,edit_ruta->text(),TRUE,this,0,TRUE);
			prop->exec();
		}
	}
	else {
		hide();
		Nuevo_Propiedades* prop = new Nuevo_Propiedades(_categoria,_tipo,edit_ruta->text(),FALSE,this,0,TRUE);
		prop->exec();
	}

	delete this;

}

void Nuevo::Cancelar()
{
	delete this;
}

class ExaminarProyecto : public QFileDialog
{
public:
	ExaminarProyecto(QWidget * parent=0, const char * name = 0, bool modal = FALSE)
		: QFileDialog(parent, name, modal)
	{
		QLabel* labelinfo1=new QLabel("Dentro del directorio que selecciones aquí se creará una nueva carpeta con\nel nombre del proyecto, donde se guardarán los archivos del proyecto.",this);
		//labelinfo1->setTextFormat(Qt::RichText);
		labelinfo1->setBackgroundColor(QColor(255,255,170));
		labelinfo1->setMargin(5);
		labelinfo1->setFrameShape(QFrame::Box);
		QLabel* labelinfo2=new QLabel(this);
		QPixmap* imagen=pillagif(ayuda_gif);
		labelinfo2->setPixmap(*imagen);
		labelinfo2->setAlignment(Qt::AlignHCenter|Qt::AlignTop);
		addWidgets(labelinfo2,labelinfo1,0);
		setCaption("Directorio de proyecto");
		setMode(QFileDialog::DirectoryOnly);
	}
};

void Nuevo::Examinar()
{
	if(_categoria==0) {
		ExaminarProyecto* dialogo = new ExaminarProyecto(this,0,TRUE);
//		dialogo->setDir(Main.projects_folder);
		dialogo->setDir(edit_ruta->text());
		dialogo->setSelection("");
		dialogo->exec();
		edit_ruta->setText(dialogo->selectedFile());
	}
	else {
		QFileDialog* dialogo = new QFileDialog(this,0,TRUE);
		dialogo->setMode(QFileDialog::AnyFile);
		dialogo->setDir(edit_ruta->text());
		switch(_categoria) {
			case 1: dialogo->setFilter("Programas (*.prg)"); break;
			case 2: switch(_tipo) {
						case 1:	dialogo->setFilter("Mapa de DIV Games Studio (*.map)"); break;
						case 2: dialogo->setFilter("PCSoft PCX (*.pcx)"); break;
						case 3: dialogo->setFilter("Portable Network Graphics (*.png)"); break;
						case 4: dialogo->setFilter("Mapa de bits de Windows (*.bmp)"); break;
						case 5: dialogo->setFilter("JPEG Independent Group (*.jpg *.jpeg *.jpe)");
					}; break;
			case 3: dialogo->setFilter("Fichero para gráficos (*.fpg)"); break;
			case 4: dialogo->setFilter("Paleta de 8 bits (*.pal)"); break;
			case 5: dialogo->setFilter("Fuente de letra (*.fnt)"); break;
			case 6: dialogo->setFilter("Mapa 3D (*.wld)"); break;
			case 7: switch(_tipo) {
						case 1: dialogo->setFilter("Archivo MIDI (*.mid *.midi)"); break;
						case 2: dialogo->setFilter("Módulo de Impulse Tracker (*.it)"); break;
						case 3: dialogo->setFilter("Módulo de Fast Tracker 2 (*.xm)"); break;
						case 4: dialogo->setFilter("Módulo de Scream Tracker (*.s3m)"); break;
						case 5: dialogo->setFilter("Módulo de ProTracker (*.mod)");
					}; break;
			case 8: switch(_tipo) {
						case 1: dialogo->setFilter("Microsoft ADPCM (*.wav)"); break;
						case 2: dialogo->setFilter("OGG Vorbis (*.ogg)"); break;
						case 3: dialogo->setFilter("Fraunhoffer MP3 (*.mp3)"); break;
						case 4: dialogo->setFilter("Windows Media Audio (*.wma)"); break;
						case 5: dialogo->setFilter("Sonido de Div Games Studio 1 (*.pcm)");
					}; break;
			case 9: switch(_tipo) {
						case 1: dialogo->setFilter("Autodesk Animator (*.fli)"); break;
						case 2: dialogo->setFilter("Autodesk Animator Pro (*.flc)"); break;
						case 3: dialogo->setFilter("Motion Picture Engineering Group (*.mpg *.mpeg *.mpe)"); break;
						case 4: dialogo->setFilter("Apple QuickTime (*.mov)"); break;
						case 5: dialogo->setFilter("Audio & Video Interlaced (*.avi)"); break;
					}
		}
		dialogo->exec();
		edit_ruta->setText(dialogo->selectedFile());
	}
}

void Nuevo::set_descripcion(int categoria, int tipo)
{

	_categoria=categoria;
	_tipo=(tipo==0)?1:tipo;
	switch(categoria) {
		case 0: // Proyecto (EPJ)
			descripcion->setText("Elige esta opción para comenzar un nuevo proyecto con eDIV. En el directorio indicado en la caja de abajo (por defecto, tu directorio \"Mis proyectos de eDIV\") se creará una nueva carpeta con el nombre del proyecto, donde se guardarán el propio fichero de proyecto (*.epj) y los distintos archivos de recursos que se le vayan añadiendo.");
			break;
		case 1:	// PRG
			descripcion->setText("Los archivos PRG son ficheros fuente en los que se escribe el código del juego.");
			break;
		case 2: // MAP
			descripcion->setText("Los mapas contienen gráficos que pueden cargarse, mostrarse y modificarse en un juego.");
			switch(tipo) {
				case 1:
					descripcion->setText(descripcion->text() + "<hr>Los MAP son los mapas clásicos de DIV Games Studio. No tienen compresión, pero permiten colocar puntos de control.");
					break;
				case 2:
					descripcion->setText(descripcion->text() + "<hr>Los PCX no tienen pérdida de calidad y tienen una pequeña compresión.");
					break;
				case 3:
					descripcion->setText(descripcion->text() + "<hr>Los PNG sólo pueden usarse en modos de 16 bits o más y no tienen pérdida de calidad.");
					break;
				case 4:
					descripcion->setText(descripcion->text() + "<hr>Los BMP no tienen pérdida de calidad, pero tampoco tienen compresión. Sin embargo, es un formato muy extendido.");
					break;
				case 5:
					descripcion->setText(descripcion->text() + "<hr>Los JPG son conocidos por su algoritmo de compresión, que hace que ocupen muy poco, a costa de una pequeña pérdida de calidad. Sólo pueden usarse en modos de 16 bits o más.");
					break;
			}
			break;
		case 3: // FPG
			descripcion->setText("Los ficheros para gráficos (FPG) contienen varios mapas en un mismo fichero. Son útiles para tener los mapas agrupados y ordenados en pocos ficheros.");
			break;
		case 4: // PAL
			descripcion->setText("Las paletas sólo se usan en modos de 8 bits (256 colores). Un juego carga paletas según necesite una tabla de colores u otra.");
			break;
		case 5: // FNT
			descripcion->setText("Las fuentes de letra se usan para escribir textos en pantalla con distintos estilos con las funciones <code>write()</code>, <code>write_int()</code> y <code>write_in_map()</code>.");
			break;
		case 6: // WLD
			descripcion->setText("Los mundos 3D se utilizan en el modo 8. Se crean dibujando regiones 2D que luego en el juego aparecen como paredes, suelos y techos en 3D.");
			switch(tipo) {
				case 1:
					descripcion->setText(descripcion->text() + "<hr>El WLD es el formato clásido de DIV Games Studio 2.");
					break;
				case 2:
					descripcion->setText(descripcion->text() + "<hr>El WLD2 es similar al WLD, con la diferencia de que ocupa menos. No puede abrirse desde el editor de DIV 2, pero sí pueden leerlo los juegos hechos con él.");
					break;
			}
			break;
		case 7: // MOD/MIDI
			descripcion->setText("Los ficheros musicales (módulos o MIDI) contienen una composición musical. Se diferencian de los ficheros de sonido en que se componen mediante una partitura o un secuenciador, y ocupan mucha menos memoria.");
			switch(tipo) {
				case 1:
					descripcion->setText(descripcion->text() + "<hr>Los archivos MIDI son realmente pequeños y sencillos de componer. La desventaja es que no suenan con la misma calidad en todas las tarjetas de sonido.");
					break;
				case 2:
					descripcion->setText(descripcion->text() + "<hr>Los archivos IT, superiores a los XM, son módulos de música formada por formas de onda secuenciadas. Pueden llegar a tener gran calidad.");
					break;
				case 3:
					descripcion->setText(descripcion->text() + "<hr>Los archivos XM, superiores a los S3M, son módulos de música formada por formas de onda secuenciadas. Pueden llegar a tener gran calidad.");
					break;
				case 4:
					descripcion->setText(descripcion->text() + "<hr>Los archivos S3M, son módulos de música formada por formas de onda secuenciadas. Pueden llegar a tener gran calidad.");
					break;
				case 5:
					descripcion->setText(descripcion->text() + "<hr>Los archivos MOD, son módulos de música formada por formas de onda secuenciadas. Este formato en particular es antiguo y sólo soporta 4 canales de sonido.");
			}
			break;
		case 8: // WAV
			descripcion->setText("Los ficheros de sonido se usan para los efectos de sonido de los juegos o para música con calidad digital.");
			switch(tipo) {
				case 1:
					descripcion->setText(descripcion->text() + "<hr>Los WAV alcanzan una gran calidad, y no tienen un nivel de compresión muy alto pero se leen rápidamente, por ello son ideales para los efectos de sonido que se repiten frecuentemente.");
					break;
				case 2:
					descripcion->setText(descripcion->text() + "<hr>Los OGG tienen una gran compresión y una calidad prácticamente a nivel de CD Audio. Son ideales para música digital.");
					break;
				case 3:
					descripcion->setText(descripcion->text() + "<hr>Los MP3 son, posiblemente, los archivos de audio digital más extendidos a través de Internet. Tienen una compresión y calidad muy altas.<br><b>NOTA: Este formato no es gratuito. Consulta la documentación para conocer los detalles acerca del MP3.");
					break;
				case 4:
					descripcion->setText(descripcion->text() + "<hr>Los WMA son el contraataque de Microsoft a los MP3. Tienen una calidad y compresión similares, pero a diferencia del MP3, es un formato libre.");
					break;
				case 5:
					descripcion->setText(descripcion->text() + "<hr>Los PCM son un formato muy antiguo cuyo soporte sólo incluimos por razones de compatibilidad.");
			}
			break;
		case 9: // FLI
			descripcion->setText("Los ficheros de animación y de vídeo se usan para insertar películas o secuencias cinemáticas no interactivas (como videos de introducción, etc).");
			switch(tipo) {
				case 1:
					descripcion->setText(descripcion->text() + "<hr>El formato FLI es antiguo y sólo soporta 256 colores, pero es rápido y sencillo de crear y de manejar.");
					break;
				case 2:
					descripcion->setText(descripcion->text() + "<hr>El FLC es similar al FLI, pero con la diferencia de que admite tamaños superiores a 320x200 pixels.");
					break;
				case 3:
					descripcion->setText(descripcion->text() + "<hr>El MPG es un formato con calidad aceptable y compresión alta. Es muy utilizado en videojuegos profesionales.");
					break;
				case 4:
					descripcion->setText(descripcion->text() + "<hr>El formato MOV tiene gran calidad aunque no tanta compresión como el MPG. Es más recomendado para grandes resoluciones.");
					break;
				case 5:
					descripcion->setText(descripcion->text() + "<hr>TODO: ¿ké codec de AVI metemos?");
					break;
			}
	}
}

void Nuevo::combo_tipo_selecciona(int index)
{
	set_descripcion(_categoria,index+1);
}

